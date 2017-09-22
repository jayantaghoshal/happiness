/**
 * Unit test of the CarconfigUpdater file.
 *
 * Production code under test:
 *   ihu_cedric/carconfig_updater/src/carconfig_updater.cpp
 *
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <sstream>
#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <unistd.h>
#include <cstdio>
#include "carconfig_updater.h"
#include "carconfig_file.h"
#include "carconfig_parameterlist.h"
#include <mock_diagnostic_client.h>
#include <mock_vipcom_client.h>

using ::testing::_;
using ::testing::Return;
using ::testing::Field;
using ::testing::AtLeast;

extern bool setStateAndSendDiagnostics(bool stateConfigured, bool allParamsReceived, bool allParamsOk,
                                       bool paramsChanged, bool allStoredParamsOk,
                                       std::map<uint32_t, uint8_t> receivedBadParams,
                                       std::map<uint32_t, uint8_t> storedBadParams, diagnosticsClient &diagClient,
                                       bool &rebootNeeded);

typedef std::map<uint32_t, uint8_t> paramMap_t;

class CarConfigUpdaterTestFixture : public ::testing::Test
{
   public:
    virtual void SetUp()
    {
        std::stringstream tmp;
        tmp << _TESTFOLDER;
        testFolderPath = tmp.str();
        mockCarconfigFile = new CarconfigFile();
    }
    std::string testFolderPath;
    std::string tmpFilePath = testFolderPath + "tmpFile";
    CarconfigFile *mockCarconfigFile;
    bool mIsConfigured = false;
    bool mAllParamsReceived = false;
    bool mAllParamsOk = false;
    bool mStoredParamOk = false;
    bool mParamsChanged = false;
    bool mRebootNeeded = false;
    paramMap_t mReceivedParam, mStoredBadParams;

    virtual void TearDown()
    {
        std::remove(testFolderPath.c_str());
        delete mockCarconfigFile;
    }

   protected:
};

class CarConfigUpdateSetStateLogicBulkF : public CarConfigUpdaterTestFixture
{
   public:
    virtual void SetUp()
    {
        CarConfigUpdaterTestFixture::SetUp();
        mIsConfigured = false;
    }

    virtual void TearDown() { CarConfigUpdaterTestFixture::TearDown(); }
};

// Needed for V-table to be generated.
ICarconfigFile::~ICarconfigFile(){};

TEST_F(CarConfigUpdaterTestFixture, framereceiver_emptyBufferAndZeroTimeout_theFunctionShouldReturnImmediately)
{
    ccBuffer buff;
    frameReceiver(buff, 0);
    ASSERT_TRUE(1);
}

TEST_F(CarConfigUpdaterTestFixture, fileExists_givenMissingFile_theFunctionShouldReturnFalse)
{
    ASSERT_FALSE(fileExists(testFolderPath + "/x"));
}

TEST_F(CarConfigUpdaterTestFixture, fileExists_givenExistingFile_theFunctionShouldReturnTrue)
{
    ASSERT_TRUE(fileExists(testFolderPath + "carconfig_updater_test.cpp"));
}

TEST_F(CarConfigUpdaterTestFixture, writeEmptyFile_givenExistingFile_theFunctionShouldWriteAFile)
{
    writeEmptyFile(tmpFilePath);
    ASSERT_TRUE(fileExists(tmpFilePath));
}

TEST_F(CarConfigUpdaterTestFixture,
       checkReceivedValues_givenCompleteBuffer_theFunctionShouldReturnAllParamsReceivedAndallParamsOK)
{
    ccBuffer buff;
    bool allReceived;
    bool allOk;
    for (int i = 0; i < 504; i++)
    {
        buff[i].received = true;
        buff[i].value = 1;
    }
    std::map<uint32_t, uint8_t> receivedBadParams;
    checkReceivedValues(CarConfigParamList::paramList, buff, allReceived, allOk, receivedBadParams);
    ASSERT_TRUE(allReceived);
    ASSERT_TRUE(allOk);

    for (int i = 1; i <= 504; i++)
    {
        if (CarConfigParamList::paramList.count(i) > 0)
        {
            ASSERT_TRUE(buff[i - 1].ok);
            ASSERT_TRUE(buff[i - 1].subscribed);
        }
    }
}

TEST_F(CarConfigUpdaterTestFixture,
       checkReceivedValues_givenCompleteBufferWithSomeBadValues_theFunctionShouldReturnAllParamsReceived)
{
    ccBuffer buff;
    bool allReceived;
    bool allOk;
    for (int i = 0; i < 504; i++)
    {
        buff[i].received = true;
        buff[i].value = 1;
    }
    std::map<uint32_t, uint8_t> receivedBadParams;
    checkReceivedValues(CarConfigParamList::paramListBad, buff, allReceived, allOk, receivedBadParams);
    ASSERT_TRUE(allReceived);
    ASSERT_FALSE(allOk);

    ASSERT_FALSE(buff[17].ok);
    ASSERT_FALSE(buff[198].ok);
}

TEST_F(CarConfigUpdaterTestFixture, checkReceivedValues_givenEmptyBuffer_theFunctionShouldReturnAllFalse)
{
    ccBuffer buff;
    bool allReceived;
    bool allOk;

    std::map<uint32_t, uint8_t> receivedBadParams;
    checkReceivedValues(CarConfigParamList::paramList, buff, allReceived, allOk, receivedBadParams);
    ASSERT_FALSE(allReceived);
    ASSERT_FALSE(allOk);

    for (int i = 1; i <= 504; i++)
    {
        if (CarConfigParamList::paramList.count(i) > 0)
        {
            ASSERT_FALSE(buff[i - 1].ok);
            ASSERT_TRUE(buff[i - 1].subscribed);
        }
    }
}

TEST_F(CarConfigUpdaterTestFixture, checkReceivedValues_givenBadParameters_receivedBadParametersAreConsistent)
{
    ccBuffer buff;
    bool allReceived;
    bool allOk;
    for (int i = 0; i < 504; i++)
    {
        buff[i].received = true;
        buff[i].value = 1;
    }
    std::map<uint32_t, uint8_t> receivedBadParams;
    checkReceivedValues(CarConfigParamList::paramListBad, buff, allReceived, allOk, receivedBadParams);
    ASSERT_TRUE(allReceived);
    ASSERT_FALSE(allOk);

    // The faults are on the indices 17 and 198 of the buff array
    ASSERT_FALSE(buff[17].ok);
    ASSERT_FALSE(buff[198].ok);

    // We expect the errors to be designated as in positions 18 and 199
    std::vector<uint8_t> expectedStoredContent = {2, 0, 18, 1, 0, 199, 1, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};  // expected values

    ASSERT_EQ(expectedStoredContent, carconfigParamFaultsPack(receivedBadParams));
}

TEST_F(CarConfigUpdaterTestFixture, checkExistingParams_givenAllInParamsOk_allParamsOKVariableSholdBeTrue)
{

    EXPECT_CALL(*mockCarconfigFile, open(_)).Times(1);

    ccValue ccVal;
    ccVal.raw = 1;
    ccVal.subs = 1;
    ccVal.status = ccStatus::GOOD;

    EXPECT_CALL(*mockCarconfigFile, getValue(_))
        .Times(CarConfigParamList::paramList.size())
        .WillRepeatedly(Return(ccVal));

    bool allOk;
    std::map<uint32_t, uint8_t> storedBadParams;
    checkExistingParams(*mockCarconfigFile, tmpFilePath, CarConfigParamList::paramList, allOk, storedBadParams);
    ASSERT_TRUE(allOk);
}

TEST_F(CarConfigUpdaterTestFixture, checkExistingParams_givenSomeInParamsBad_allParamsOKVariableShouldBeFalse)
{

    EXPECT_CALL(*mockCarconfigFile, open(_)).Times(1);

    ccValue ccVal;
    ccVal.raw = 2;
    ccVal.subs = 2;
    ccVal.status = ccStatus::GOOD;

    EXPECT_CALL(*mockCarconfigFile, getValue(_))
        .Times(CarConfigParamList::paramList.size())
        .WillRepeatedly(Return(ccVal));

    bool allOk;
    std::map<uint32_t, uint8_t> storedBadParams;
    checkExistingParams(*mockCarconfigFile, tmpFilePath, CarConfigParamList::paramList, allOk, storedBadParams);
    ASSERT_FALSE(allOk);

    // We expect the errors to be designated as in positions 18 and 199
    std::vector<uint8_t> expectedStoredContent = {2, 0, 18, 2, 0, 199, 2, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};  // expected values

    ASSERT_EQ(expectedStoredContent, carconfigParamFaultsPack(storedBadParams));
}

TEST_F(CarConfigUpdaterTestFixture, checkExistinParameters_givenSomeInParamsBad_returnedParamsInPackageAreConsistent)
{

    EXPECT_CALL(*mockCarconfigFile, open(_)).Times(1);

    ccValue ccVal;
    ccVal.raw = 1;
    ccVal.subs = 1;
    ccVal.status = ccStatus::GOOD;

    ccValue badValLow;
    badValLow.raw = 0;
    badValLow.subs = 0;
    badValLow.status = ccStatus::GOOD;

    ccValue badValHigh;
    badValHigh.raw = 0xFF;
    badValHigh.subs = 0xFF;
    badValHigh.status = ccStatus::GOOD;

    EXPECT_CALL(*mockCarconfigFile, getValue(_))
        .Times(CarConfigParamList::paramList.size() - 3)
        .WillRepeatedly(Return(ccVal));

    EXPECT_CALL(*mockCarconfigFile, getValue(1))
        .WillOnce(Return(badValLow));

    EXPECT_CALL(*mockCarconfigFile, getValue(7))
        .WillOnce(Return(badValLow));

    EXPECT_CALL(*mockCarconfigFile, getValue(24))
        .WillOnce(Return(badValHigh));

    bool allOk;
    std::map<uint32_t, uint8_t> storedBadParams;
    checkExistingParams(*mockCarconfigFile, tmpFilePath, CarConfigParamList::paramList, allOk, storedBadParams);
    ASSERT_FALSE(allOk);

    // We expect the errors in positions 1,7 and 24. No error in 4, as that parameter does not exist in the list
    std::vector<uint8_t> expectedStoredContent = {3, 0, 1, 0, 0, 7, 0, 0, 24, 0xFF,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};  // expected values

    ASSERT_EQ(expectedStoredContent, carconfigParamFaultsPack(storedBadParams));
}

TEST_F(CarConfigUpdaterTestFixture, storeReceivedParameter_givenValidUpdatedParameter_outputFileShouldBeReplaced)
{
    ccBuffer buff;
    int i;

    EXPECT_CALL(*mockCarconfigFile, open(_)).Times(1);

    ccValue ccVal;
    ccVal.raw = 1;
    ccVal.subs = 1;
    ccVal.status = ccStatus::GOOD;

    for (i = 0; i < 504; i++)
    {
        buff[i].subscribed = true;
        buff[i].value = 2;
        buff[i].ok = true;
    }

    EXPECT_CALL(*mockCarconfigFile, getValue(_)).Times(504).WillRepeatedly(Return(ccVal));

    EXPECT_CALL(*mockCarconfigFile, setValue(_, (Field(&ccValue::raw, 2), Field(&ccValue::subs, 2),
                                                 Field(&ccValue::status, ccStatus::GOOD)))).Times(504);

    storeReceivedParameter(*mockCarconfigFile, tmpFilePath, buff);
}

TEST_F(CarConfigUpdaterTestFixture,
       storeReceivedParameter_givenBadParameters_onlyRawParameterInOutputFileShouldBeReplaced)
{
    ccBuffer buff;
    int i;

    EXPECT_CALL(*mockCarconfigFile, open(_)).Times(1);

    ccValue ccVal;
    ccVal.raw = 1;
    ccVal.subs = 1;
    ccVal.status = ccStatus::GOOD;

    for (i = 0; i < 504; i++)
    {
        buff[i].subscribed = true;
        buff[i].value = 2;
        buff[i].ok = false;
    }

    EXPECT_CALL(*mockCarconfigFile, getValue(_)).Times(504).WillRepeatedly(Return(ccVal));

    EXPECT_CALL(*mockCarconfigFile, setValue(_, (Field(&ccValue::raw, 2), Field(&ccValue::subs, 1),
                                                 Field(&ccValue::status, ccStatus::INVALID)))).Times(504);

    storeReceivedParameter(*mockCarconfigFile, tmpFilePath, buff);
}

TEST_F(CarConfigUpdaterTestFixture,
       storeReceivedParameter_givenUnsubscribedParameters_onlyRawParameterShouldBeReplacedAndSubTo255)
{
    ccBuffer buff;
    int i;

    EXPECT_CALL(*mockCarconfigFile, open(_)).Times(1);

    for (i = 0; i < 504; i++)
    {
        buff[i].subscribed = false;
        buff[i].received = true;
        buff[i].value = 2;
    }

    EXPECT_CALL(*mockCarconfigFile, setValue(_, (Field(&ccValue::raw, 2), Field(&ccValue::subs, 255),
                                                 Field(&ccValue::status, ccStatus::INVALID)))).Times(504);

    storeReceivedParameter(*mockCarconfigFile, tmpFilePath, buff);
}

TEST_F(CarConfigUpdaterTestFixture, carconfigParamFaultsPack_givenNoParameters_ExpectedSizeShouldBeZero)
{
    std::map<uint32_t, uint8_t> params;
    std::vector<uint8_t> result;
    result = carconfigParamFaultsPack(params);
    ASSERT_EQ(result[0], 0);
    ASSERT_EQ(result.size(), (size_t)31); // Size should be always 31 (10*3 bytes for parameters + 1 for length)
}

TEST_F(CarConfigUpdaterTestFixture, carconfigParamFaultsPack_givenTwoParameters_thePackedListContainsValidFormat)
{
    std::map<uint32_t, uint8_t> params;
    std::vector<uint8_t> result;
    params.insert(std::pair<uint32_t, uint8_t>(0x12C, 0xFF));
    params.insert(std::pair<uint32_t, uint8_t>(0x0E, 0xFE));

    // call UT
    result = carconfigParamFaultsPack(params);

    ASSERT_EQ(result[0], 2);
    ASSERT_EQ(result[1], 0x0);
    ASSERT_EQ(result[2], 0x0E);
    ASSERT_EQ(result[3], 0xFE);
    ASSERT_EQ(result[4], 0x1);
    ASSERT_EQ(result[5], 0x2C);
    ASSERT_EQ(result[6], 0xFF);
    ASSERT_EQ(result.size(), (size_t)31); // Size should be always 31 (10*3 bytes for parameters + 1 for length)
}

TEST_F(CarConfigUpdaterTestFixture, carconfigParamFaultsPack_ReportLessThan10InvalidParameters)
{
    std::map<uint32_t, uint8_t> params;
    std::vector<uint8_t> result;
    for (int idx = 0; idx < 9; idx++)
    {
        params.insert(std::pair<uint32_t, uint8_t>(3 + (idx * 0x2A), 3 + idx));
    }

    // call UT
    result = carconfigParamFaultsPack(params);

    ASSERT_EQ(result[0], 9);  // stores the total size
    ASSERT_EQ(result.size(), (size_t)31); // Size should be always 31 (10*3 bytes for parameters + 1 for length)
}

TEST_F(CarConfigUpdaterTestFixture, carconfigParamFaultsPack_ReportExactly10InvalidParameters)
{
    std::map<uint32_t, uint8_t> params;
    std::vector<uint8_t> result;
    for (int idx = 0; idx < 10; idx++)
    {
        params.insert(std::pair<uint32_t, uint8_t>(3 + (idx * 0x2A), 3 + idx));
    }

    // call UT
    result = carconfigParamFaultsPack(params);

    // we expect 10 items, length == 10, and 31 bytes stored in vector
    ASSERT_EQ(result[0], 10);  // stores the total size
    ASSERT_EQ(result.size(), (size_t)31); // Size should be always 31 (10*3 bytes for parameters + 1 for length)
}

TEST_F(CarConfigUpdaterTestFixture, carconfigParamFaultsPack_DoNotReportMoreThen10InvalidParameters)
{
    std::map<uint32_t, uint8_t> params;
    std::vector<uint8_t> result;
    for (int idx = 0; idx < 11; idx++)
    {
        params.insert(std::pair<uint32_t, uint8_t>(3 + (idx * 0x2A), 3 + idx));
    }

    // call UT
    result = carconfigParamFaultsPack(params);

    // we expect to total amount of invalid frames, but not more than 10*3 bytes attached
    ASSERT_EQ(result[0], 10);  // stores the total size
    ASSERT_EQ(result.size(), (size_t)31); // Size should be always 31 (10*3 bytes for parameters + 1 for length)
}

TEST_F(CarConfigUpdaterTestFixture, updateVipParameters_VipResponseNotOk_FailedSendingValues)
{
    CarConfigVipComMock vipcomClient;

    EXPECT_CALL(*mockCarconfigFile, open(_)).Times(1);
    EXPECT_CALL(*mockCarconfigFile, getValue(_)).Times(2);
    EXPECT_CALL(vipcomClient, sendConfig(_)).Times(1).WillOnce(Return(-1));

    updateVipParameters(*mockCarconfigFile, vipcomClient);
}

TEST_F(CarConfigUpdaterTestFixture, updateVipParameters_VipResponseNotOk_FailedReceivingAck)
{
    CarConfigVipComMock vipcomClient;

    EXPECT_CALL(*mockCarconfigFile, open(_)).Times(1);
    EXPECT_CALL(*mockCarconfigFile, getValue(_)).Times(2);
    EXPECT_CALL(vipcomClient, sendConfig(_)).Times(1).WillOnce(Return(3)); // sent 3 bytes
    EXPECT_CALL(vipcomClient, waitForVipAcknowledge()).Times(1).WillOnce(Return(-1));

    updateVipParameters(*mockCarconfigFile, vipcomClient);
}

TEST_F(CarConfigUpdaterTestFixture, updateVipParameters_VipResponseOk_ReceivedAck)
{
    CarConfigVipComMock vipcomClient;

    EXPECT_CALL(*mockCarconfigFile, open(_)).Times(1);
    EXPECT_CALL(*mockCarconfigFile, getValue(_)).Times(2);
    EXPECT_CALL(vipcomClient, sendConfig(_)).Times(1).WillOnce(Return(3)); // sent 3 bytes
    EXPECT_CALL(vipcomClient, waitForVipAcknowledge()).Times(1).WillOnce(Return(0));

    updateVipParameters(*mockCarconfigFile, vipcomClient);
}

TEST_F(CarConfigUpdateSetStateLogicBulkF, setStateLogic_BulkState_AllReceivedAndOkExpectedReboot)
{
    /*
     * we expect that in BULK mode, all params
     */
    MockDiagnosticClient mockDiagClient;

    mAllParamsOk = true;
    mAllParamsReceived = true;
    bool result;
    std::vector<uint8_t> registerValues = {0x00};

    EXPECT_CALL(mockDiagClient, registerDID(11031, registerValues)).Times(1);

    result = setStateAndSendDiagnostics(mIsConfigured, mAllParamsReceived, mAllParamsOk, mParamsChanged, mStoredParamOk,
                                        mReceivedParam, mStoredBadParams,
                                        *(reinterpret_cast<diagnosticsClient *>(&mockDiagClient)), mRebootNeeded);

    EXPECT_TRUE(result);
    EXPECT_TRUE(mRebootNeeded);
}

TEST_F(CarConfigUpdateSetStateLogicBulkF, setStateLogic_BulkState_AllReceivedAndFailed)
{
    /*
     * in BULK mode, receiving bad parameters should end up in setting a DTC
     * and requesting an REBOOT
     */
    MockDiagnosticClient mockDiagClient;

    mAllParamsReceived = true;
    mAllParamsOk = false;   // invalid parameters received
    mIsConfigured = false;  // BULK mode
    bool result;
    std::vector<uint8_t> registerValues = {0};
    std::vector<uint8_t> updateValues = {1, 0, 32, 12, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    std::vector<uint8_t> expDidStatusCodes = {0b0001 | 0b0100};

    // add some invalid values
    mReceivedParam.insert(std::pair<uint32_t, uint8_t>(32, 12));

    // we expect that this pair send to diagnostic function
    EXPECT_CALL(mockDiagClient, registerDID(11031, registerValues)).Times(1);
    EXPECT_CALL(mockDiagClient, updateDID(11031, updateValues)).Times(1);
    EXPECT_CALL(mockDiagClient, sendDiagnosticsMessage(12040, expDidStatusCodes)).Times(1);

    result = setStateAndSendDiagnostics(mIsConfigured, mAllParamsReceived, mAllParamsOk, mParamsChanged, mStoredParamOk,
                                        mReceivedParam, mStoredBadParams,
                                        *(reinterpret_cast<diagnosticsClient *>(&mockDiagClient)), mRebootNeeded);

    EXPECT_FALSE(result);
    EXPECT_FALSE(mRebootNeeded);
}

TEST_F(CarConfigUpdateSetStateLogicBulkF, setStateLogic_BulkState_NotAllReceivedExpectStoredAreSendToDTC)
{
    /*
     * in BULK mode, receiving bad parameters should end up in setting a DTC
     * and requesting an REBOOT
     */
    MockDiagnosticClient mockDiagClient;

    mAllParamsReceived = false;  // not all received, received values are ignored
    mAllParamsOk = false;        // invalid parameters received
    mIsConfigured = false;       // BULK mode
    mStoredParamOk = false;      // we've stored bad parameters
    bool result;
    std::vector<uint8_t> expectedRegisterContent = {0};
    std::vector<uint8_t> expectedStoredContent = {2, 0, 30, 14, 0, 32, 29, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};  // expected values

    // add some invalid values to the listOfStored parameters
    mStoredBadParams.insert(std::pair<uint32_t, uint8_t>(30, 14));
    mStoredBadParams.insert(std::pair<uint32_t, uint8_t>(32, 29));

    // we expect that this pair send to diagnostic function
    EXPECT_CALL(mockDiagClient, registerDID(11031, expectedRegisterContent)).Times(1);
    EXPECT_CALL(mockDiagClient, updateDID(11031, expectedStoredContent)).Times(1);

    result = setStateAndSendDiagnostics(mIsConfigured, mAllParamsReceived, mAllParamsOk, mParamsChanged, mStoredParamOk,
                                        mReceivedParam, mStoredBadParams,
                                        *(reinterpret_cast<diagnosticsClient *>(&mockDiagClient)), mRebootNeeded);

    EXPECT_FALSE(result);
    EXPECT_FALSE(mRebootNeeded);
}

TEST_F(CarConfigUpdateSetStateLogicBulkF, setStateLogic_BulkState_NotAllReceivedNoBadParamsStoredDoNotUpdateDID)
{
    /*
     * in BULK mode, receiving bad parameters should end up in setting a DTC
     * and requesting an REBOOT
     */
    MockDiagnosticClient mockDiagClient;

    mAllParamsReceived = false;  // not all received, received values are ignored
    mAllParamsOk = false;        // invalid parameters received
    mIsConfigured = false;       // BULK mode
    mStoredParamOk = true;       // no wrong parameters stored
    bool result;
    std::vector<uint8_t> expectedRegisterContent = {0};
    std::vector<uint8_t> expectedStoredContent = {2, 0, 30, 14, 0, 32, 29, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};  // expected values

    // add some invalid values to the listOfStored parameters
    mStoredBadParams.insert(std::pair<uint32_t, uint8_t>(30, 14));
    mStoredBadParams.insert(std::pair<uint32_t, uint8_t>(32, 29));

    // we expect that this pair send to diagnostic function
    EXPECT_CALL(mockDiagClient, registerDID(11031, expectedRegisterContent)).Times(1);
    EXPECT_CALL(mockDiagClient, updateDID(11031, expectedStoredContent)).Times(0);

    result = setStateAndSendDiagnostics(mIsConfigured, mAllParamsReceived, mAllParamsOk, mParamsChanged, mStoredParamOk,
                                        mReceivedParam, mStoredBadParams,
                                        *(reinterpret_cast<diagnosticsClient *>(&mockDiagClient)), mRebootNeeded);

    EXPECT_FALSE(result);
    EXPECT_FALSE(mRebootNeeded);
}

TEST_F(CarConfigUpdateSetStateLogicBulkF, setStateLogic_ConfiguredState_AllReceivedAllOkNothingChanged_ExpectDoNothing)
{
    /*
     * in BULK mode, receiving bad parameters should end up in setting a DTC
     * and requesting an REBOOT
     */
    MockDiagnosticClient mockDiagClient;
    bool result;

    mAllParamsReceived = true;  // not all received, received values are ignored
    mAllParamsOk = true;        // invalid parameters received
    mIsConfigured = true;       // Configured mode
    mParamsChanged = false;     // nothing changed
    std::vector<uint8_t> expectedRegisterContent = {0};
    std::vector<uint8_t> expectedStoredContent = {2, 0, 30, 14, 0, 32, 29, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};  // expected values

    // add some invalid values to the listOfStored parameters
    mStoredBadParams.insert(std::pair<uint32_t, uint8_t>(30, 14));
    mStoredBadParams.insert(std::pair<uint32_t, uint8_t>(32, 29));

    // add some invalid values received from FlexRay
    mReceivedParam.insert(std::pair<uint32_t, uint8_t>(256, 3));

    // we expect that this pair send to diagnostic function
    EXPECT_CALL(mockDiagClient, registerDID(11031, expectedRegisterContent)).Times(1);

    // we DO NOT expected a call to the DID functionality
    EXPECT_CALL(mockDiagClient, updateDID(11031, expectedStoredContent)).Times(0);

    result = setStateAndSendDiagnostics(mIsConfigured, mAllParamsReceived, mAllParamsOk, mParamsChanged, mStoredParamOk,
                                        mReceivedParam, mStoredBadParams,
                                        *(reinterpret_cast<diagnosticsClient *>(&mockDiagClient)), mRebootNeeded);

    EXPECT_FALSE(result);
    EXPECT_FALSE(mRebootNeeded);
}

TEST_F(CarConfigUpdateSetStateLogicBulkF, setStateLogic_ConfiguredState_AllReceivedAllOkValuesChanged_RequestReboot)
{
    /*
     * in Configured mode
     * receiving valid values and received updates should lead to a _reboot_
     */
    MockDiagnosticClient mockDiagClient;
    bool result;

    mIsConfigured = true;       // Configured mode
    mAllParamsReceived = true;  // not all received, received values are ignored
    mAllParamsOk = true;        // invalid parameters received
    mParamsChanged = true;      // nothing changed

    std::vector<uint8_t> expectedRegisterContent = {0};
    std::vector<uint8_t> expectedStoredContent = {2, 0, 30, 14, 0, 32, 29, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};  // expected values

    // add some invalid values to the listOfStored parameters
    mStoredBadParams.insert(std::pair<uint32_t, uint8_t>(30, 14));
    mStoredBadParams.insert(std::pair<uint32_t, uint8_t>(32, 29));

    // add some invalid values received from FlexRay
    mReceivedParam.insert(std::pair<uint32_t, uint8_t>(256, 3));

    // we expect that this pair send to diagnostic function
    EXPECT_CALL(mockDiagClient, registerDID(11031, expectedRegisterContent)).Times(1);

    // we DO NOT expected a call to the DID functionality
    EXPECT_CALL(mockDiagClient, updateDID(11031, expectedStoredContent)).Times(0);

    result = setStateAndSendDiagnostics(mIsConfigured, mAllParamsReceived, mAllParamsOk, mParamsChanged, mStoredParamOk,
                                        mReceivedParam, mStoredBadParams,
                                        *(reinterpret_cast<diagnosticsClient *>(&mockDiagClient)), mRebootNeeded);

    EXPECT_EQ(result, false);
    EXPECT_EQ(mRebootNeeded, true);
}

TEST_F(CarConfigUpdateSetStateLogicBulkF, setStateLogic_ConfiguredState_AllReceivedButNotAllOk_ReportDIDRequestReboot)
{
    /*
     * in Configured mode
     * receiving valid values and received updates should lead to a _reboot_
     */
    MockDiagnosticClient mockDiagClient;
    bool result;

    mIsConfigured = true;       // Configured mode
    mAllParamsReceived = true;  // not all received, received values are ignored
    mAllParamsOk = false;       // __invalid__ parameters received

    std::vector<uint8_t> expectedRegisterContent = {0};
    std::vector<uint8_t> expectedReceivedParams = {1, 0x1, 0x2E, 0x03, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};  // expected values
    std::vector<uint8_t> expDiagnosticCodes = {0b01};

    // add some invalid values received from FlexRay
    mReceivedParam.insert(std::pair<uint32_t, uint8_t>(0x12E, 0x03));

    // we expect that this pair send to diagnostic function
    EXPECT_CALL(mockDiagClient, registerDID(11031, expectedRegisterContent)).Times(1);

    // we DO NOT expected a call to the DID functionality
    EXPECT_CALL(mockDiagClient, updateDID(11031, expectedReceivedParams)).Times(1);

    EXPECT_CALL(mockDiagClient, sendDiagnosticsMessage(12040, expDiagnosticCodes)).Times(1);

    result = setStateAndSendDiagnostics(mIsConfigured, mAllParamsReceived, mAllParamsOk, mParamsChanged, mStoredParamOk,
                                        mReceivedParam, mStoredBadParams,
                                        *(reinterpret_cast<diagnosticsClient *>(&mockDiagClient)), mRebootNeeded);

    EXPECT_EQ(result, false);
    EXPECT_EQ(mRebootNeeded, false);
}

TEST_F(CarConfigUpdateSetStateLogicBulkF, setStateLogic_ConfiguredState_NotAllReceivedStoredOk_ExpectNothing)
{
    /*
     * in Configured mode
     * receiving valid values and received updates should lead to a _reboot_
     */
    MockDiagnosticClient mockDiagClient;
    bool result;

    mIsConfigured = true;        // Configured mode
    mAllParamsReceived = false;  // not all received, received values are ignored
    mAllParamsOk = false;        // __invalid__ parameters received
    mStoredParamOk = true;       // no errors stored

    std::vector<uint8_t> expectedRegisterContent = {0};
    std::vector<uint8_t> expectedReceivedParams = {1, 0x1, 0x2E, 0x03, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};  // expected values
    std::vector<uint8_t> expDiagnosticCodes = {0b01};

    // add some invalid values received from FlexRay
    mReceivedParam.insert(std::pair<uint32_t, uint8_t>(0x12E, 0x03));

    // we expect that this pair send to diagnostic function
    EXPECT_CALL(mockDiagClient, registerDID(11031, expectedRegisterContent)).Times(1);

    // we DO NOT expected a call to the DID functionality
    EXPECT_CALL(mockDiagClient, updateDID(11031, expectedReceivedParams)).Times(0);

    EXPECT_CALL(mockDiagClient, sendDiagnosticsMessage(12040, expDiagnosticCodes)).Times(0);

    result = setStateAndSendDiagnostics(mIsConfigured, mAllParamsReceived, mAllParamsOk, mParamsChanged, mStoredParamOk,
                                        mReceivedParam, mStoredBadParams,
                                        *(reinterpret_cast<diagnosticsClient *>(&mockDiagClient)), mRebootNeeded);

    EXPECT_EQ(result, false);
    EXPECT_EQ(mRebootNeeded, false);
}

TEST_F(CarConfigUpdateSetStateLogicBulkF,
       setStateLogic_ConfiguredState_NotAllReceivedStoredNotOk_ExpectStoredBeingReportedToDID)
{
    /*
     * in Configured mode
     * receiving valid values and received updates should lead to a _reboot_
     */
    MockDiagnosticClient mockDiagClient;
    bool result;

    mIsConfigured = true;        // Configured mode
    mAllParamsReceived = false;  // not all received, received values are ignored
    mAllParamsOk = false;        // __invalid__ parameters received
    mStoredParamOk = false;      // no errors stored

    std::vector<uint8_t> expectedRegisterContent = {0};                 // register is always zero
    std::vector<uint8_t> expectedStoredResults = {1, 0x1, 0x2E, 0x03, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};  // contains expected stored bad parameters
    std::vector<uint8_t> expDiagnosticCodes = {0b01};                   // not configured error

    // add some invalid values received from FlexRay
    mStoredBadParams.insert(std::pair<uint32_t, uint8_t>(0x12E, 0x03));

    // we expect that this pair send to diagnostic function
    EXPECT_CALL(mockDiagClient, registerDID(11031, expectedRegisterContent)).Times(1);

    // we DO expect that bad stored parameters are reported to the DID
    EXPECT_CALL(mockDiagClient, updateDID(11031, expectedStoredResults)).Times(1);

    // in this case we DO expect an diagnostic code to be set
    EXPECT_CALL(mockDiagClient, sendDiagnosticsMessage(12040, expDiagnosticCodes)).Times(0);

    result = setStateAndSendDiagnostics(mIsConfigured, mAllParamsReceived, mAllParamsOk, mParamsChanged, mStoredParamOk,
                                        mReceivedParam, mStoredBadParams,
                                        *(reinterpret_cast<diagnosticsClient *>(&mockDiagClient)), mRebootNeeded);

    EXPECT_EQ(result, false);
    EXPECT_EQ(mRebootNeeded, false);
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
