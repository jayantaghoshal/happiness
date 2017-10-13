#include <set>
#include <sys/stat.h>
#include <fstream>
#include <chrono>
#include <utils/Log.h>
#include <array>
#include "Application_dataelement.h"
#include "cc_parameterlist_map.h"
//#include "diagnostics_client.h"
#include "carconfig_updater.h"
//#include "restart_client.h"
#include "carconfig_reader.h"
#include "carconfig_file_writer.h"

#undef LOG_TAG
#define LOG_TAG "CarConfigUpdater"

using namespace ApplicationDataElement;
using namespace autosar;
using namespace CarConfigParamList;
using namespace std::chrono;
using namespace Carconfig_base;

const uint32_t kCCFrames = 72;    // Total number of carconfig Frames
const uint32_t kCCBytes = 7;      // The number of parameters in each frame
const uint32_t kCCTimeout = 30;  // The maximum time to download all carconfig frames.
// we can handle the slow rate which carconfig frames are relayed by the VIP.

const size_t kDtcMaxParameters = 10;

bool debugMode = false;

void CarConfigUpdater::frameReceiver(CarConfigList &buff, uint32_t timeout)
{
    // The VehCfgPrm (carconfig) flexray frame.
    DEReceiver<VehCfgPrm_info> ccReceiver;

    std::set<uint32_t> frameCount;
    steady_clock::time_point timeThen = steady_clock::now();

    ALOGI("Waiting for VehCfgPrm_info carconfig frames...");
    ccReceiver.subscribe([&buff, &frameCount, &ccReceiver]()
                         {
                             auto frame = ccReceiver.get().value();
                             uint32_t zeroBasedBlkID = frame.BlkIDBytePosn1 - 1;
                             ALOGV("Got VehCfgPrm frame: %d Param %d - %d:   %d, %d, %d, %d, %d, %d, %d", \
                                frame.BlkIDBytePosn1, (zeroBasedBlkID * 7) + 1, (zeroBasedBlkID * 7) + 7 , \
                                frame.CCPBytePosn2, frame.CCPBytePosn3, frame.CCPBytePosn4, frame.CCPBytePosn5,\
                                frame.CCPBytePosn6, frame.CCPBytePosn7, frame.CCPBytePosn8);

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
                                 ALOGE("Got VehCfgPrm_info:BlkIDBytePosn1 higher than 72");
                             }
                         });
    while ((frameCount.size() < kCCFrames) &&
           ((duration_cast<seconds>(steady_clock::now() - timeThen)).count() < timeout))
    {
        usleep(100000);
    }
    if ((duration_cast<seconds>(steady_clock::now() - timeThen)).count() < timeout)
    {
        ALOGD("Received all carconfig frames in %lld seconds", (duration_cast<seconds>(steady_clock::now() - timeThen)).count());
    }
    else
    {
        ALOGW("Carconfig receive timout occured");
    }
}

void CarConfigUpdater::writeEmptyFile(std::string filePath)
{
    std::ofstream fs;
    fs.open(filePath, std::ios::out);
    fs.close();
}

void CarConfigUpdater::checkReceivedValues(const std::map<int, std::vector<int>> ccParamList, CarConfigList &buff, bool &allParamsReceived,
                                           bool &allParamsOK, std::map<uint32_t, uint8_t> &errorList)
{
    allParamsReceived = true;
    allParamsOK = true;
    uint32_t bufferPos;

    for (const auto& param : ccParamList)
    {
        bufferPos = param.first - 1;
        buff[bufferPos].subscribed = true;
        if (buff[bufferPos].received != true)
        {
            ALOGD("Did not receive parameter %d", param.first);
            allParamsReceived = false;
            allParamsOK = false;
        }
        else
        {
            bool paramOk = false;
            for (const auto& paramVal : param.second)
            {
                if (buff[bufferPos].value == paramVal)
                {
                    paramOk = true;
                    buff[bufferPos].ok = true;
                    break;
                }
            }
            if (paramOk == false)
            {
                ALOGI("Parameter %d is not within the defined range", param.first);
                errorList.insert(std::pair<uint32_t, uint8_t>(bufferPos + 1, buff[bufferPos].value));
                allParamsOK = false;
            }
        }
    }
}

void CarConfigUpdater::checkExistingParams(const std::map<int, std::vector<int>> ccParamList, bool &allParamsOK,
                         std::map<uint32_t, uint8_t> &errorList)
{
    CarConfigReader ccReader;

    allParamsOK = true;
    for (const auto& param : ccParamList)
    {
        auto ccValue = ccReader.getValue(param.first);
        bool paramOk = false;
        for (const auto& paramVal : param.second)
        {
            if (paramVal == ccValue)
            {
                paramOk = true;
            }
        }
        if (paramOk == false)
        {
            ALOGW("Previously stored parameter %d with value %d is not within the defined range", param.first, ccValue);
            errorList.insert(std::pair<uint32_t, uint8_t>(param.first, ccValue));
            allParamsOK = false;
        }
    }
}

bool CarConfigUpdater::storeReceivedParameter(CarConfigList &buffer)
{
    bool paramsChanged = false;
    int bufferPos = 1;  // getFile uses value 1 to cc_no_of_parameters.
    ccValue ccFileParam;
    CarConfigReader ccReader;
    std::array<ccValue, cc_no_of_parameters> carConfigs;

    for (const auto& buffVal : buffer)
    {
        // Replace all values in the CSV file with new ones. If bad values have been received
        // replace just the raw value and set status flag so INVALID.

        ccFileParam.subs = ccReader.getValue(bufferPos);
        ccFileParam.raw = ccReader.getRawValue(bufferPos);
        ccFileParam.status =ccReader.getStatus(bufferPos);

        if (buffVal.subscribed == true)
        {
            if (ccFileParam.raw != buffVal.value)
            {
                ALOGD("Subscribed CC%d changed raw value from %d to %d", bufferPos, ccFileParam.raw, buffVal.value);
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
        }
        carConfigs[bufferPos-1] = ccFileParam;
        bufferPos++;
    }

    if(paramsChanged || !fileExists(carconfig_file_name))
    {
        ALOGI("Writing received values to: %s", carconfig_file_name.c_str());
        CarConfigFileWriter::writeCarConfig(carConfigs);
    }
    return paramsChanged;
}

std::vector<uint8_t> CarConfigUpdater::carconfigParamFaultsPack(std::map<uint32_t, uint8_t> params)
{
    std::vector<uint8_t> packedParams;
    size_t paramIdx;

    // Ensure that we never put more than 10 parameter into the byte array
    params.size() <= 10 ? paramIdx = params.size() : paramIdx = kDtcMaxParameters;

    // Ensure that length data field matches the number of parameters stored
    packedParams.push_back(paramIdx);

    for (const auto& param : params)
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

bool CarConfigUpdater::setStateAndSendDiagnostics(bool stateConfigured, bool allParamsReceived, bool allParamsOk, bool paramsChanged,
                                bool allStoredParamsOk, std::map<uint32_t, uint8_t> receivedBadParams,
                                std::map<uint32_t, uint8_t> storedBadParams, //diagnosticsClient &diagClient, //TODO add diagnostics
                                bool &rebootNeeded)
{
    bool NewStateConfigured = false;
    // diagClient.registerDID(MK_CD_DID_CAR_CONFIG_PARAM_FAULTS, {0x00});

    // Bulk State
    if (!stateConfigured)
    {
        // All parameters received in time
        if (allParamsReceived && allParamsOk)
        {
            ALOGI("Got all valid parameters, leaving Bulk state and switching to Configured state.");
            NewStateConfigured = true;
            if (paramsChanged)
            {
                rebootNeeded = true;
            }
            // TODO Here we should probably report "pass" on MK_CD_DTC_CENTRAL_CONFIGURATION_STATUS to diagnostics
        }
        // All parameters received in time but some are not ok.
        else if (allParamsReceived && !allParamsOk)
        {
            ALOGW("Bulk state: All parameters are not in range, staying in Bulk state");
            // diagClient.updateDID(MK_CD_DID_CAR_CONFIG_PARAM_FAULTS, carconfigParamFaultsPack(receivedBadParams));
            // diagClient.sendDiagnosticsMessage(
                // MK_CD_DTC_CENTRAL_CONFIGURATION_STATUS,
                // {uint8_t(MK_CD_DTC_CENTRAL_CONFIGURATION_STATUS_BYTE_0::invalid_unconfigured_error) |
                //  uint8_t(MK_CD_DTC_CENTRAL_CONFIGURATION_STATUS_BYTE_0::not_configured_error)});
            if (paramsChanged)
            {
                rebootNeeded = true;
            }
        }
        // all other cases
        else
        {
            ALOGW("Bulk state: Did not receive all/any parameters before timeout");
            // If we have bad parameters from a previous session, set the DID for those.
            if (!allStoredParamsOk)
            {
                ALOGW("Bulk state: Restoring previous DIDs");
                // diagClient.updateDID(MK_CD_DID_CAR_CONFIG_PARAM_FAULTS, carconfigParamFaultsPack(storedBadParams));
            }
        }
    }
    else  // Configured state
    {
        // All parameters received in time without updates.
        if (allParamsReceived && allParamsOk && !paramsChanged)
        {
            ALOGI("Configured state: Got all valid parameters, but no updated, doing nothing");
        }
        // Parameters received in time with updated, valid data.
        else if (allParamsReceived && allParamsOk && paramsChanged)
        {
            ALOGI("Configured state: Got updated and valid parameters");
            rebootNeeded = true;
        }
        // Parameters received in time, updated but with at least one invalid value
        else if (allParamsReceived && !allParamsOk)
        {
            ALOGW("Configured state: Got some invalid parameters");
            // diagClient.updateDID(MK_CD_DID_CAR_CONFIG_PARAM_FAULTS, carconfigParamFaultsPack(receivedBadParams));
            // diagClient.sendDiagnosticsMessage(
                // MK_CD_DTC_CENTRAL_CONFIGURATION_STATUS,
                // {uint8_t(MK_CD_DTC_CENTRAL_CONFIGURATION_STATUS_BYTE_0::not_configured_error)});
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
                ALOGW("Bulk state: Restoring previous DIDs");
                // diagClient.updateDID(MK_CD_DID_CAR_CONFIG_PARAM_FAULTS, carconfigParamFaultsPack(storedBadParams));
            }
            ALOGW("Configured state: Did not received parameters before timeout");
        }
    }
    return NewStateConfigured;
}

// void updateVipParameters(CarConfigVipCom &vipcomClient)
// {
//     CarconfigFileReader ccFileReader;
//     // Get parameters of interest: CC175, CC181
//     std::vector<uint8_t> parameters;
//     ccFileReader.open(carconfig_file_name);
//     parameters.push_back(ccFileReader.getValue(175).raw);
//     parameters.push_back(ccFileReader.getValue(181).raw);

//     // Send parameters to VIP and wait for response.
//     if (vipcomClient.sendConfig(parameters) == -1)
//     {
//         ALOGE("Failed sending values to VIP");
//     }
//     else if (vipcomClient.waitForVipAcknowledge() == -1)
//     {
//         ALOGE("Did not receive ACK from VIP, but continuing with CarConfig updater!");
//     }
// }

int32_t CarConfigUpdater::runUpdater()
{
    bool allOldParamsOk;
    bool allParamsOk;
    bool allParamsReceived;
    CarConfigList buffer;
    // diagnosticsClient diagClient;
    // CarConfigVipCom vipcomClient; //TODO add vipCom

    ALOGI("carconfig-updater started");

    // Start the flexray receiver and wait for the result. (30 sec maximum)
    frameReceiver(buffer, kCCTimeout);

    // Iterate thru the generated parameter list and check that each parameter
    // received on flexray is defined within a valied range.
    std::map<uint32_t, uint8_t> receivedBadParams;
    checkReceivedValues(paramList, buffer, allParamsReceived, allParamsOk, receivedBadParams);

    // Check the status of the existing carconfig file.
    std::map<uint32_t, uint8_t> storedBadParams;
    try
    {
        checkExistingParams(paramList, allOldParamsOk, storedBadParams);
    }
    catch (std::exception& e)
    {
        ALOGE("%s", e.what());
    }

    // Only update the stored carconfig if all parameters has been received.
    bool paramsChanged = false;
    if (allParamsReceived)
    {
        ALOGI("Got all carconfig parameters");
        try
        {
            paramsChanged = storeReceivedParameter(buffer);
        }
        catch (std::exception& e)
        {
            ALOGE("%s", e.what());
        }
    }

    // Send DTCs and set DIDs.
    // diagClient.connect();
    bool rebootIsRequired = false;
    bool configured =
        setStateAndSendDiagnostics(emptyFileExists(carconfig_configured_filename), allParamsReceived, allParamsOk, paramsChanged,
                                   allOldParamsOk, receivedBadParams, storedBadParams, /*diagClient,*/ rebootIsRequired);
    // Set state to configured
    if (configured)
    {
        writeEmptyFile(carconfig_configured_filename);
    }

    // Send interesting parameters to VIP.
    if (paramsChanged)
    {
        // Success of updating VIP values does not matter,
        // we always try it if parameters changed and then ask for reboot.
        try
        {
            //updateVipParameters(vipcomClient); //TODO Send params to VIP when vipCom is availabele
            ALOGW("Send parameters to vip is not yet implemented");
            rebootIsRequired = true;
        }
        catch (std::exception& e)
        {
            ALOGE("%s", e.what());
        }
    }

    if (rebootIsRequired)
    {
        ALOGI("Signaling node state manager that a restart is needed");
        ALOGW("Proper rebooot request is not yet implemented. Rebooting with system call directly to MP");
        system("/system/bin/reboot"); //TODO change this to a call to "power manager"

        //restartClient r;
        //r.restart();
    }
    else
    {
        // diagClient.runForever();
    }
    return 0;
}
