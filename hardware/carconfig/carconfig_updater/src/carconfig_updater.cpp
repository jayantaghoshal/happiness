#include <ivi-logging.h>
#include <QCoreApplication>
#include <set>
#include <sys/stat.h>
#include <fstream>
#include <chrono>
#include <dataelements/Application_dataelement.h>
#include "carconfig_parameterlist.h"
#include "diagnostics_client.h"
#include "carconfig_updater.h"
#include "restart_client.h"

using namespace ApplicationDataElement;
using namespace autosar;
using namespace CarConfigParamList;
using namespace std::chrono;

// Define a context for the IVI-logging library to use.
typedef logging::DefaultLogContext LogContext;

LOG_DEFINE_APP_IDS("CCUS", "Carconfig updater service.");
LOG_DECLARE_DEFAULT_CONTEXT(carconfig_updater_log_context, "MAIN", "Main thread");

const uint32_t kCCFrames = 72;    // Total number of carconfig Frames
const uint32_t kCCBytes = 7;      // The number of parameters in each frame
const uint32_t kCCTimeout = 30;  // The maximum time to download all carconfig frames.
// we can handle the slow rate which carconfig frames are relayed by the VIP.
const std::string kCCFileName = "/var/lib/carconfig/carconfig.csv";
const std::string kConfiguredFileName = "/var/lib/carconfig/CARCONFIG_CONFIGURED";
const size_t kDtcMaxParameters = 10;

bool debugMode = false;

void frameReceiver(ccBuffer &buff, uint32_t timeout)
{
    // The VehCfgPrm (carconfig) flexray frame.
    DEReceiver<VehCfgPrm_info> ccReceiver;

    std::set<uint32_t> frameCount;
    steady_clock::time_point timeThen = steady_clock::now();

    log_info() << "Waiting for VehCfgPrm_info carconfig frames...";
    ccReceiver.subscribe([&buff, &frameCount, &ccReceiver]()
                         {
                             auto frame = ccReceiver.get().value();
                             uint32_t zeroBasedBlkID = frame.BlkIDBytePosn1 - 1;
                             log_info() << "Got VehCfgPrm frame: " << frame.BlkIDBytePosn1 << " "
                                        << "Param " << (zeroBasedBlkID * 7) + 1 << "-" << (zeroBasedBlkID * 7) + 7
                                        << "  " << frame.CCPBytePosn2 << "," << frame.CCPBytePosn3 << ","
                                        << frame.CCPBytePosn4 << "," << frame.CCPBytePosn5 << "," << frame.CCPBytePosn6
                                        << "," << frame.CCPBytePosn7 << "," << frame.CCPBytePosn8;
                             if (zeroBasedBlkID < kCCFrames)
                             {
                                 frameCount.insert(zeroBasedBlkID);
                                 uint32_t offset = (kCCBytes * zeroBasedBlkID);
                                 for (uint32_t i = 0; i < kCCBytes; i++)
                                 {
                                     buff[offset + i].received = true;
                                 }
                                 buff[offset + 0].value = frame.CCPBytePosn2;
                                 buff[offset + 1].value = frame.CCPBytePosn3;
                                 buff[offset + 2].value = frame.CCPBytePosn4;
                                 buff[offset + 3].value = frame.CCPBytePosn5;
                                 buff[offset + 4].value = frame.CCPBytePosn6;
                                 buff[offset + 5].value = frame.CCPBytePosn7;
                                 buff[offset + 6].value = frame.CCPBytePosn8;
                             }
                             else
                             {
                                 log_error() << "Got VehCfgPrm_info:BlkIDBytePosn1 higher than 72";
                             }
                         });
    while ((frameCount.size() < kCCFrames) &&
           ((duration_cast<seconds>(steady_clock::now() - timeThen)).count() < timeout))
    {
        usleep(100000);
    }
    if ((duration_cast<seconds>(steady_clock::now() - timeThen)).count() < timeout)
    {
        log_info() << "Received all carconfig frames in "
                   << (duration_cast<seconds>(steady_clock::now() - timeThen)).count() << " seconds";
    }
    else
    {
        log_info() << "Carconfig receive timout occured";
    }
}

bool fileExists(std::string filePath)
{
    struct stat buffer;
    return (stat(filePath.c_str(), &buffer) == 0);
}

void writeEmptyFile(std::string filePath)
{
    std::ofstream fs;
    fs.open(filePath, std::ios::out);
    fs.close();
}

void checkReceivedValues(const std::map<int, std::vector<int>> ccParamList, ccBuffer &buff, bool &allParamsReceived,
                         bool &allParamsOK, std::map<uint32_t, uint8_t> &errorList)
{
    allParamsReceived = true;
    allParamsOK = true;
    uint32_t bufferPos;

    for (auto param : ccParamList)
    {
        bufferPos = param.first - 1;
        buff[bufferPos].subscribed = true;
        if (buff[bufferPos].received != true)
        {
            log_info() << "Did not receive parameter " << param.first;
            allParamsReceived = false;
            allParamsOK = false;
        }
        else
        {
            bool paramOk = false;
            for (auto paramVal : param.second)
            {
                if (buff[bufferPos].value == paramVal)
                {
                    paramOk = true;
                    buff[bufferPos].ok = true;
                }
            }
            if (paramOk == false)
            {
                log_error() << "Parameter " << param.first << "is not within the defined range";
                errorList.insert(std::pair<uint32_t, uint8_t>(bufferPos + 1, buff[bufferPos].value));
                allParamsOK = false;
            }
        }
    }
}

void checkExistingParams(CarconfigFile &ccFile, const std::string ccFileName,
                         const std::map<int, std::vector<int>> ccParamList, bool &allParamsOK,
                         std::map<uint32_t, uint8_t> &errorList)
{
    ccFile.open(ccFileName);
    ccValue ccParam;

    allParamsOK = true;
    for (auto param : ccParamList)
    {
        ccParam = ccFile.getValue(param.first);
        bool paramOk = false;
        for (auto paramVal : param.second)
        {
            if (paramVal == ccParam.raw)
            {
                paramOk = true;
            }
        }
        if (paramOk == false)
        {
            log_warn() << "Previously stored parameter " << param.first << " with value " << ccParam.raw
                       << " is not within the defined range";
            errorList.insert(std::pair<uint32_t, uint8_t>(param.first, ccParam.raw));
            allParamsOK = false;
        }
    }
}

bool storeReceivedParameter(CarconfigFile &ccFile, std::string outputFile, ccBuffer &buffer)
{
    bool paramsChanged = false;
    int bufferPos = 1;  // getFile/setFile uses value 1 to 504.
    ccValue ccFileParam;

    ccFile.open(outputFile);
    for (auto buffVal : buffer)
    {
        // Replace all values in the CSV file with new ones. If bad values have been received
        // replace just the raw value and set status flag so INVALID.
        if (buffVal.subscribed == true)
        {
            ccFileParam = ccFile.getValue(bufferPos);
            if (ccFileParam.raw != buffVal.value)
            {
                ccFileParam.raw = buffVal.value;
                // If we get a good value we also replace the substitute
                // value.
                if (buffVal.ok == true)
                {
                    ccFileParam.status = ccStatus::GOOD;
                    ccFileParam.subs = buffVal.value;
                }
                else
                {
                    ccFileParam.status = ccStatus::INVALID;
                }
                ccFile.setValue(bufferPos, ccFileParam);
                paramsChanged = true;
            }
        }
        // We don't subscribe to these values, but since we might act as a bridge to
        // other ECUs we store these values but only store the raw value. Set the status flag to
        // invalid and the subst value to 255 so that if any component in this IHU tries to
        // access the value they will get an error.
        else
        {
            if (buffVal.received == true)
            {
                ccFileParam.raw = buffVal.value;
            }
            ccFileParam.subs = 255;
            ccFileParam.status = ccStatus::INVALID;
            ccFile.setValue(bufferPos, ccFileParam);
        }
        bufferPos++;
    }
    return paramsChanged;
}

std::vector<uint8_t> carconfigParamFaultsPack(std::map<uint32_t, uint8_t> params)
{
    std::vector<uint8_t> packedParams;
    size_t paramIdx;

    // Ensure that we never put more than 10 parameter into the byte array
    params.size() <= 10 ? paramIdx = params.size() : paramIdx = kDtcMaxParameters;

    // Ensure that length data field matches the number of parameters stored
    packedParams.push_back(paramIdx);

    for (auto param : params)
    {
        packedParams.push_back((param.first >> 8) & 0xFF);
        packedParams.push_back((param.first) & 0xFF);
        packedParams.push_back(param.second);
        if (--paramIdx == 0)
        {
            break;
        }
    }
    // Pad with 0's if there are less than 10 parameters
    while (packedParams.size() < 31)
    {
        packedParams.push_back(0);
    }
    return packedParams;
}

bool setStateAndSendDiagnostics(bool stateConfigured, bool allParamsReceived, bool allParamsOk, bool paramsChanged,
                                bool allStoredParamsOk, std::map<uint32_t, uint8_t> receivedBadParams,
                                std::map<uint32_t, uint8_t> storedBadParams, diagnosticsClient &diagClient,
                                bool &rebootNeeded)
{
    bool NewStateConfigured = false;
    diagClient.registerDID(MK_CD_DID_CAR_CONFIG_PARAM_FAULTS, {0x00});

    // Bulk State
    if (!stateConfigured)
    {
        // All parameters received in time
        if (allParamsReceived && allParamsOk)
        {
            log_info() << "Bulk state: Got ok parameters, switching to Configured.";
            NewStateConfigured = true;
            rebootNeeded = true;
        }
        // All parameters received in time but some are not ok.
        else if (allParamsReceived && !allParamsOk)
        {
            log_warn() << "Bulk state: Got some ok parameters, updating and staying in Bulk state";
            diagClient.updateDID(MK_CD_DID_CAR_CONFIG_PARAM_FAULTS, carconfigParamFaultsPack(receivedBadParams));
            diagClient.sendDiagnosticsMessage(
                MK_CD_DTC_CENTRAL_CONFIGURATION_STATUS,
                {uint8_t(MK_CD_DTC_CENTRAL_CONFIGURATION_STATUS_BYTE_0::invalid_unconfigured_error) |
                 uint8_t(MK_CD_DTC_CENTRAL_CONFIGURATION_STATUS_BYTE_0::not_configured_error)});
            if (paramsChanged)
            {
                rebootNeeded = true;
            }
        }
        // all other cases
        else
        {
            log_warn() << "Bulk state: Did not receive all/any parameters before timeout";
            // If we have bad parameters from a previous session, set the DID for those.
            if (!allStoredParamsOk)
            {
                log_warn() << "Bulk state: Restoring previous DIDs";
                diagClient.updateDID(MK_CD_DID_CAR_CONFIG_PARAM_FAULTS, carconfigParamFaultsPack(storedBadParams));
            }
        }
    }
    else  // Configured state
    {
        // All parameters received in time without updates.
        if (allParamsReceived && allParamsOk && !paramsChanged)
        {
            log_info() << "Configured state: Got ok parameters, but no updated, doing nothing";
        }
        // Parameters received in time with updated, valid data.
        else if (allParamsReceived && allParamsOk && paramsChanged)
        {
            log_info() << "Configured state: Got ok parameters, updated parameters";
            rebootNeeded = true;
        }
        // Parameters received in time, updated but with at least one invalid value
        else if (allParamsReceived && !allParamsOk)
        {
            log_warn() << "Configured state: Got some ok parameters.";
            diagClient.updateDID(MK_CD_DID_CAR_CONFIG_PARAM_FAULTS, carconfigParamFaultsPack(receivedBadParams));
            diagClient.sendDiagnosticsMessage(
                MK_CD_DTC_CENTRAL_CONFIGURATION_STATUS,
                {uint8_t(MK_CD_DTC_CENTRAL_CONFIGURATION_STATUS_BYTE_0::not_configured_error)});
            if (paramsChanged)
            {
                rebootNeeded = true;
            }
        }
        //  Without, or with late, FlexRay signaling (passed the 30 seconds threshold)
        else
        {
            if (!allStoredParamsOk)
            {
                log_warn() << "Bulk state: Restoring previous DIDs";
                diagClient.updateDID(MK_CD_DID_CAR_CONFIG_PARAM_FAULTS, carconfigParamFaultsPack(storedBadParams));
            }
            log_warn() << "Configured state: Did not received parameters before timeout";
        }
    }
    return NewStateConfigured;
}

void updateVipParameters(CarconfigFile &ccFile, CarConfigVipCom &vipcomClient)
{
    // Get parameters of interest: CC175, CC181
    std::vector<uint8_t> parameters;
    ccFile.open(kCCFileName);
    parameters.push_back(ccFile.getValue(175).raw);
    parameters.push_back(ccFile.getValue(181).raw);

    // Send parameters to VIP and wait for response.
    if (vipcomClient.sendConfig(parameters) == -1)
    {
        log_error() << "Failed sending values to VIP";
    }
    else if (vipcomClient.waitForVipAcknowledge() == -1)
    {
        log_error() << "Did not receive ACK from VIP, but continuing with CarConfig updater!";
    }
}

int32_t runUpdater(void)
{
    bool allOldParamsOk;
    bool allParamsOk;
    bool allParamsReceived;
    ccBuffer buffer;
    diagnosticsClient diagClient;
    CarConfigVipCom vipcomClient;

    log_info() << "carconfig-updater started.";

    if (debugMode)
    {
        log_debug() << "Debug mode.";
    }
    else
    {
        // Start the flexray receiver and wait for the result. (30 sec maximum)
        frameReceiver(buffer, kCCTimeout);
    }

    // Iterate thru the generated parameter list and check that each parameter
    // received on flexray is defined within a valied range.
    std::map<uint32_t, uint8_t> receivedBadParams;
    checkReceivedValues(paramList, buffer, allParamsReceived, allParamsOk, receivedBadParams);

    // Check the status of the existing carconfig file.
    std::map<uint32_t, uint8_t> storedBadParams;
    try
    {
        CarconfigFile ccFile;
        checkExistingParams(ccFile, kCCFileName, paramList, allOldParamsOk, storedBadParams);
    }
    catch (std::exception e)
    {
        log_error() << e.what();
    }

    // Only update the stored carconfig if all parameters has been received.
    bool paramsChanged = false;
    if (allParamsReceived)
    {
        log_info() << "Got all parameters, updating carconfig file";
        try
        {
            CarconfigFile ccOutPutFile;
            paramsChanged = storeReceivedParameter(ccOutPutFile, kCCFileName, buffer);
        }
        catch (std::exception e)
        {
            log_error() << e.what();
        }
    }

    // Send DTCs and set DIDs.
    diagClient.connect();
    bool rebootIsRequired = false;
    bool configured =
        setStateAndSendDiagnostics(fileExists(kConfiguredFileName), allParamsReceived, allParamsOk, paramsChanged,
                                   allOldParamsOk, receivedBadParams, storedBadParams, diagClient, rebootIsRequired);
    // Set state to configured
    if (configured)
    {
        writeEmptyFile(kConfiguredFileName);
    }

    // Send interesting parameters to VIP.
    if (paramsChanged)
    {
        // Success of updating VIP values does not matter,
        // we always try it if parameters changed and then ask for reboot.
        try
        {
            CarconfigFile ccFile;
            updateVipParameters(ccFile, vipcomClient);
            rebootIsRequired = true;
        }
        catch (std::exception e)
        {
            log_error() << e.what();
        }
    }

    if (rebootIsRequired)
    {
        log_info() << "Signaling node state manager that a restart is needed.";
        restartClient r;
        r.restart();
    }
    else
    {
        diagClient.runForever();
    }
    return 0;
}
