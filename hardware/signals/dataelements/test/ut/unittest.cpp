
/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/

#include <gtest/gtest.h>

#include <iostream>

#include <condition_variable>
#include <thread>
#include <atomic>
#include <chrono>
#include <random>

// This shall only be included by the applications/other code in the MP
#include <Application_dataelement.h>


// This shall be included by the "extended" ECD (that is written in c++)
#include <ECD_dataelement.h>

#include <vipcomm/VipFramework.h>
#include <vipcomm/VipFrameworkPrivate.h>

#include "vsm/com_crc.h"
#include "vsm/swc_crc.h"
#include "Mp_Router_crc.h"

// Types needed by MpRouter_Signals.h below
using int8 = int8_t;
using int16 = int16_t;
using int32 = int32_t;
using uint8 = uint8_t;
using uint16 = uint16_t;
using uint32 = uint32_t;
using boolean = bool;
#include "gen/MpRouter_Signals.h"

#undef LOG_TAG
#define LOG_TAG "Dataelements UnitTest"
#include <cutils/log.h>

// This is the namespace for ARSender and ARReceiver (found in Application_dataelement.h>
using namespace ApplicationDataElement;

// This is the namespace for ARInjector and ARSink (found in ECD_dataelement.h)
// This shall be used by the ECD
using namespace ECDDataElement;


static vipcomm::SignalID sink_SignalId;
static std::vector<uint8_t> sink_signalData;

static std::condition_variable cvsink;
static std::atomic_bool cvsink_cont;

static auto waitTimeout = std::chrono::seconds(2);
static const int SignalGroup = 0x2000;


class DETest : public ::testing::Test{
  void SetUp(){
    DataElementFramework::instance().reset();

    vipcomm::initializeInject();
    vipcomm::initializeSink([](vipcomm::SignalID signalId, void* signalData, size_t length) {
              sink_SignalId = signalId;
              sink_signalData = std::vector<uint8_t>((uint8_t*)signalData,((uint8_t*)signalData)+length);
              cvsink_cont=true;
              cvsink.notify_all();
    });

    // clear sink data
    sink_SignalId = -1;
    sink_signalData.clear();
  }
};

std::condition_variable cv;
std::mutex cv_m;
std::atomic_bool cv_cont;

struct CompileTest {
    void test(){}
};


// Send structs, enums and arrays from Application to ECD and verify the received values in the "ECD" (DESink)
TEST_F (DETest, sendStruct_verifyReceivedInCallbackAndParsedOk) {
    DESender<autosar::LaneKeepAidActv_info> sender;
    DESink<autosar::LaneKeepAidActv_info> receiver;

    receiver.subscribeVal([](auto signalValue) {
        // std::cout << receiver.get().serialize().toStdString() << std::endl;
        EXPECT_EQ (true, signalValue.isOk());
        EXPECT_EQ (autosar::IdPen::Prof2, signalValue.value().Pen);
        EXPECT_EQ (autosar::OnOff1::On, signalValue.value().Sts);
        EXPECT_EQ (autosar::Dir::OUT, signalValue.direction());
        cv_cont=true;
        cv.notify_all();
    });

    std::unique_lock<std::mutex> lk(cv_m);
    cv_cont=false;
    sender.send(autosar::OnOffPen{autosar::OnOff1::On,autosar::IdPen::Prof2});

    bool signalReceivedWithinExpectedTime = cv.wait_for(lk, waitTimeout, [](){return cv_cont.load();});
    EXPECT_EQ(true, signalReceivedWithinExpectedTime);
}

TEST_F (DETest, sendEnum_verifyReceivedInCallbackAndParsedOk) {
    DESender<autosar::OffsForDrvrSpprtFctActvSts_info> sender;
    DESink<autosar::OffsForDrvrSpprtFctActvSts_info> receiver;

    receiver.subscribe([&receiver]() {
        // std::cout << receiver.get().serialize().toStdString() << std::endl;
        EXPECT_EQ (true, receiver.get().isOk());
        EXPECT_EQ (autosar::OnOff1::Off, receiver.get().value());
        EXPECT_EQ (autosar::Dir::OUT, receiver.get().direction());

        cv_cont=true;
        cv.notify_all();
    });

    std::unique_lock<std::mutex> lk(cv_m);
    cv_cont=false;
    sender.send(autosar::OnOff1::Off);

    bool signalReceivedWithinExpectedTime = cv.wait_for(lk, waitTimeout, [](){return cv_cont.load();});
    EXPECT_EQ(true, signalReceivedWithinExpectedTime);
}

TEST_F (DETest, sendArray_verifyReceivedInCallbackAndParsedOk) {
    DESender<autosar::DrvrHmiTmrForHmiCen_info> sender;
    DESink<autosar::DrvrHmiTmrForHmiCen_info> receiver;

    receiver.subscribe([&receiver]() {
        // std::cout << receiver.get().serialize().toStdString() << std::endl;
        EXPECT_EQ (true, receiver.get().isOk());
        autosar::Array8ByteU8 result = receiver.get().value();
        EXPECT_EQ (7, result[0]);
        EXPECT_EQ (6, result[1]);
        EXPECT_EQ (5, result[2]);
        EXPECT_EQ (4, result[3]);
        EXPECT_EQ (3, result[4]);
        EXPECT_EQ (2, result[5]);
        EXPECT_EQ (255, result[6]);
        EXPECT_EQ (0, result[7]);
        EXPECT_EQ (autosar::Dir::OUT, receiver.get().direction());

        cv_cont=true;
        cv.notify_all();
    });

    std::unique_lock<std::mutex> lk(cv_m);
    cv_cont=false;
    sender.send({7,6,5,4,3,2,255,0});

    bool signalReceivedWithinExpectedTime = cv.wait_for(lk, waitTimeout, [](){return cv_cont.load();});
    EXPECT_EQ(true, signalReceivedWithinExpectedTime);
}


// Inject structs, enums, arrays and errors from the ECD to Application and verify the received values in the "Application" (DEReceiver)
TEST_F (DETest, injectStruct_verifyReceivedInCallbackAndParsedOk) {
    DEInjector<autosar::ConSftyWarn_info> sender;
    DEReceiver<autosar::ConSftyWarn_info> receiver;

    receiver.subscribeVal([](auto signalValue) {
        // std::cout << receiver.get().serialize().toStdString() << std::endl;
        EXPECT_EQ (true, signalValue.isOk());
        EXPECT_EQ (77, signalValue.value().ConSftyWarnId);
        EXPECT_EQ (autosar::ConSftyWarnLvl1::MedLvl, signalValue.value().ConSftyWarnLvl);
        EXPECT_EQ (autosar::OnOff1::On, signalValue.value().ConSftyWarnSnd);
        EXPECT_EQ (254, signalValue.value().ConSftyWarnTyp);
        EXPECT_EQ (4321, signalValue.value().DistanceToWarning);
        EXPECT_EQ (autosar::Dir::IN, signalValue.direction());
        EXPECT_EQ (998877, signalValue.timestamp());

        cv_cont=true;
        cv.notify_all();
    });

    std::unique_lock<std::mutex> lk(cv_m);
    cv_cont=false;
    sender.inject(autosar::ConSftyWarn1{77,autosar::OnOff1::On,254,autosar::ConSftyWarnLvl1::MedLvl,4321}, 998877);

    bool signalReceivedWithinExpectedTime = cv.wait_for(lk, waitTimeout, [](){return cv_cont.load();});
    EXPECT_EQ(true, signalReceivedWithinExpectedTime);
}

TEST_F (DETest, injectEnum_verifyReceivedInCallbackAndParsedOk) {
    DEInjector<autosar::PrpsnModElecDrvBlkd_info> sender;
    DEReceiver<autosar::PrpsnModElecDrvBlkd_info> receiver;

    receiver.subscribe([&receiver]() {
        // std::cout << receiver.get().serialize().toStdString() << std::endl;
        EXPECT_EQ (true, receiver.get().isOk());
        EXPECT_EQ (autosar::Typ1::Typ4, receiver.get().value());
        EXPECT_EQ (autosar::Dir::IN, receiver.get().direction());
        EXPECT_EQ (8877665544, receiver.get().timestamp());

        cv_cont=true;
        cv.notify_all();
    });

    std::unique_lock<std::mutex> lk(cv_m);
    cv_cont=false;
    sender.inject(autosar::Typ1::Typ4, 8877665544);

    bool signalReceivedWithinExpectedTime = cv.wait_for(lk, waitTimeout, [](){return cv_cont.load();});
    EXPECT_EQ(true, signalReceivedWithinExpectedTime);
}

TEST_F (DETest, injectArray_verifyReceivedInCallbackAndParsedOk) {
    DEInjector<autosar::HmiCenForDrvrHmi_info> sender;
    DEReceiver<autosar::HmiCenForDrvrHmi_info> receiver;

    receiver.subscribe([&receiver]() {
        // std::cout << receiver.get().serialize().toStdString() << std::endl;
        EXPECT_EQ (true, receiver.get().isOk());
        autosar::HmiCenForDrvrHmi result = receiver.get().value();
        EXPECT_EQ (77, result[0]);
        EXPECT_EQ (66, result[1]);
        EXPECT_EQ (0, result[2]);
        EXPECT_EQ (151, result[251]);
        EXPECT_EQ (152, result[252]);
        EXPECT_EQ (153, result[253]);
        EXPECT_EQ (autosar::Dir::IN, receiver.get().direction());
        EXPECT_EQ (1, receiver.get().timestamp());

        cv_cont=true;
        cv.notify_all();
    });

    autosar::HmiCenForDrvrHmi sendValue;
    sendValue.fill(0);
    sendValue[0] = 77;
    sendValue[1] = 66;
    sendValue[2] = 0;
    sendValue[251] = 151;
    sendValue[252] = 152;
    sendValue[253] = 153;
    std::unique_lock<std::mutex> lk(cv_m);
    cv_cont=false;
    sender.inject(sendValue,1);

    bool signalReceivedWithinExpectedTime = cv.wait_for(lk, waitTimeout, [](){return cv_cont.load();});
    EXPECT_EQ(true, signalReceivedWithinExpectedTime);
}

TEST_F (DETest, injectError_verifyReceivedInCallbackAndParsedOk) {
    DEInjector<autosar::PrpsnModElecDrvBlkd_info> sender;
    DEReceiver<autosar::PrpsnModElecDrvBlkd_info> receiver;


    receiver.subscribe([&receiver]() {
        // std::cout << receiver.get().serialize().toStdString() << std::endl;
        EXPECT_EQ (true, receiver.get().isError());
        EXPECT_EQ (88, receiver.get().errorCode());
        EXPECT_EQ (autosar::Dir::IN, receiver.get().direction());
        EXPECT_EQ (1234567890, receiver.get().timestamp());

        cv_cont=true;
        cv.notify_all();
    });

    std::unique_lock<std::mutex> lk(cv_m);
    cv_cont=false;
    sender.error(88,1234567890);

    bool signalReceivedWithinExpectedTime = cv.wait_for(lk, waitTimeout, [](){return cv_cont.load();});
    EXPECT_EQ(true, signalReceivedWithinExpectedTime);
}

TEST_F (DETest, internalSignal_verifyReceivedInCallbackAndParsedOk) {
    DESender<autosar::ActvNoiseCtrlReq_info> sender;
    DEReceiver<autosar::ActvNoiseCtrlReq_info> receiver;

    receiver.subscribe([&receiver]() {
        EXPECT_EQ (true, receiver.get().isOk());
        EXPECT_EQ (autosar::IdPen::Prof2, receiver.get().value().Pen);
        EXPECT_EQ (autosar::NormSptType::Spt, receiver.get().value().NormSpt);
        EXPECT_EQ (autosar::Dir::INTERNAL, receiver.get().direction());
        cv_cont=true;
        cv.notify_all();
    });

    std::unique_lock<std::mutex> lk(cv_m);
    cv_cont=false;
    sender.send(autosar::NormSptPen{autosar::NormSptType::Spt,autosar::IdPen::Prof2});

    bool signalReceivedWithinExpectedTime = cv.wait_for(lk, waitTimeout, [](){return cv_cont.load();});
    EXPECT_EQ(true, signalReceivedWithinExpectedTime);
}

TEST_F (DETest, stressTest) {
    // Verify that all signals injected are received in sequence and in same order that they are sent,
    // or at least that "latest is greatest"


    DEInjector<autosar::UsrSwtDispClimaReqForRowSec_info> sender;
    DEReceiver<autosar::UsrSwtDispClimaReqForRowSec_info> receiver;
    std::vector<autosar::UsrSwtDispClimaReqForRowSec> expectedDataSequence;
    std::default_random_engine generator;
    //std::uniform_real_distribution<double> doubleDist(0, 1000000000000);
    std::uniform_real_distribution<double> doubleDist(0, 1000000000);

    for (int i=0;i<150000;i++)
    {
        autosar::UsrSwtDispClimaReqForRowSec d{
            doubleDist(generator),
            true,
            1,
            true,
            autosar::SeatClimaLvl::Lvl1,
            true,
            false,
            autosar::SeatClimaLvl::Lvl2,
            autosar::HmiHvacFanLvl::LvlAutMinusMinus,
            true,
            autosar::HmiCmptmtTSpSpcl::Hi,
            autosar::HmiCmptmtTSpSpcl::Lo};
        expectedDataSequence.push_back(d);
    }

    auto lastReceivedIndex = std::begin(expectedDataSequence);
    int64_t lastDistance = 0;
    size_t nrOfReceived = 0;
    std::vector<size_t> receivedIndices;
    std::mutex receiverMutex;
    bool failed = false;    //Used to only show the first error instead of spamming too much


    receiver.subscribeVal([
                       &expectedDataSequence,
                       &lastReceivedIndex,
                       &nrOfReceived,
                       &lastDistance,
                       &receivedIndices,
                       &receiverMutex,
                       &failed](auto signalValue)
    {
        std::unique_lock<std::mutex> receiverLock(receiverMutex);
        if (failed)
            return;

        autosar::UsrSwtDispClimaReqForRowSec value = signalValue.value();

        auto comparator = [&value] (autosar::UsrSwtDispClimaReqForRowSec const& x) {
            // TODO check this, issue written: https://github.com/nlohmann/json/issues/360
            //const ::testing::internal::FloatingPoint<double> lhs(x.UsrSwtDispClimaReqForTSpForRowSecLe), rhs(value.UsrSwtDispClimaReqForTSpForRowSecLe);
            //const bool equal = lhs.AlmostEquals(rhs);
            //return equal;
            //return (x.UsrSwtDispClimaReqForTSpForRowSecLe == value.UsrSwtDispClimaReqForTSpForRowSecLe);
            return std::fabs(x.UsrSwtDispClimaReqForTSpForRowSecLe - value.UsrSwtDispClimaReqForTSpForRowSecLe) <= 0.00001 ;
        };


        auto it = std::find_if(
                    lastReceivedIndex,
                    std::end(expectedDataSequence),
                    comparator);



        if (it == std::end(expectedDataSequence))
        {
            autosar::UsrSwtDispClimaReqForRowSec expectedValue = expectedDataSequence[nrOfReceived];
            std::cout << "ERROR: did not find reveived signal in list of expected values (after previously received): " << std::endl;
            std::cout << "Nr of received: " << std::to_string(nrOfReceived) << std::endl;
            std::cout << "lastdistance  : " << std::to_string(lastDistance) << std::endl;
            std::cout << "Got value: " << std::to_string(value.UsrSwtDispClimaReqForTSpForRowSecLe) << std::endl;
            std::cout << "Expected value: " << std::to_string(expectedValue.UsrSwtDispClimaReqForTSpForRowSecLe) << std::endl;

            std::cout << "Previously received: " << std::endl;
            for (auto &x : receivedIndices)
                std::cout << "xxx:: " << std::to_string(x) << std::endl;


            auto it2 = std::find_if(
                        std::begin(expectedDataSequence),
                        std::end(expectedDataSequence),
                        comparator);

            if (it2 != std::end(expectedDataSequence))
            {
                auto dist2 = std::distance(std::begin(expectedDataSequence), it2);
                std::cout << "found in sequence at index: " << std::to_string(dist2) << std::endl;
            }
            else
            {
                std::cout << "not found in sequence at all " << std::endl;
            }

            failed = true;
            EXPECT_EQ(true, false);
        }
        else
        {
            long dist = std::distance(std::begin(expectedDataSequence), it);
            if (dist < lastDistance)
            {
                std::cout << "Got an older message than before " << std::endl;
                std::cout << "lastdistance: " << std::to_string(lastDistance) <<  std::endl;
                std::cout << "dist        : " << std::to_string(dist) <<  std::endl;
            }
            lastDistance = std::max(dist, lastDistance);

            receivedIndices.push_back(dist);
            lastReceivedIndex = it;
        }

        nrOfReceived++;
        if (failed ||
            (static_cast<size_t>(lastDistance+1) == expectedDataSequence.size()) ||
            (nrOfReceived == expectedDataSequence.size()))
        {
            cv_cont = true;
            cv.notify_all();
        }
    });

    std::unique_lock<std::mutex> lk(cv_m);
    cv_cont=false;
    std::chrono::steady_clock::time_point before = std::chrono::steady_clock::time_point();
    for (auto d : expectedDataSequence)
    {
        sender.inject(d);
    }
    std::chrono::steady_clock::time_point after = std::chrono::steady_clock::time_point();

    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(after -before).count();
    std::cout << "Time to send all: " << std::to_string(ms) << "ms" << std::endl;


    bool signalReceivedWithinExpectedTime = cv.wait_for(lk, std::chrono::seconds(5), [](){return cv_cont.load();});
    EXPECT_EQ(true, signalReceivedWithinExpectedTime);

    if(!signalReceivedWithinExpectedTime)
    {
        std::cout << "Last received distance: " << lastDistance << std::endl;
    }

    std::chrono::steady_clock::time_point after2 = std::chrono::steady_clock::time_point();
    auto ms2 = std::chrono::duration_cast<std::chrono::milliseconds>(after2 -before).count();
    std::cout << "Time to receive all: " << std::to_string(ms2) << "ms" << std::endl;
    std::cout << "Number of sent    : " << std::to_string(expectedDataSequence.size()) << std::endl;
    std::cout << "Number of received: " << std::to_string(nrOfReceived) << std::endl;
}

TEST_F (DETest, RTE_test_inject_ok_signed1) {
    DEReceiver<autosar::ADataRawSafe_info> receiver;

    receiver.subscribe([&receiver]() {
        EXPECT_TRUE (receiver.get().isOk());
        EXPECT_EQ (-2.0 * 0.0085, receiver.get().value().ALat);
        EXPECT_EQ (2.0 * 0.0085, receiver.get().value().ALgt);
        EXPECT_EQ (autosar::Qf1::DataCalcdWithDevDefd, receiver.get().value().ALat1Qf);
        EXPECT_EQ (autosar::Qf1::DataTmpUndefdAndEvlnInProgs, receiver.get().value().ALgt1Qf);
        EXPECT_EQ (5.0 * 0.0085, receiver.get().value().AVert);
        EXPECT_EQ (autosar::Qf1::DevOfDataUndefd, receiver.get().value().AVertQf);
        cv_cont=true;
        cv.notify_all();
    });
    std::unique_lock<std::mutex> lk(cv_m);
    cv_cont=false;

    const int signalId = SignalGroup | ComConf_ComSignalGroup_igADataRawSafe_mrx;
    ADataRawSafe rteType;

    rteType.ALat1 = 0x7FFE; // ALat is 15 bit signed int so this represents -2
    rteType.ALgt1 = 2;
    rteType.ALat1Qf1 = (int)autosar::Qf1::DataCalcdWithDevDefd;
    rteType.ALgt1Qf1 = (int)autosar::Qf1::DataTmpUndefdAndEvlnInProgs;
    rteType.AVert2 = 5;
    rteType.AVert2Qf1 = (int)autosar::Qf1::DevOfDataUndefd;

    vipcomm::injectIncomingOkSignal(signalId, &rteType, sizeof(rteType));

    bool signalReceivedWithinExpectedTime = cv.wait_for(lk, waitTimeout, [](){return cv_cont.load();});
    EXPECT_TRUE(signalReceivedWithinExpectedTime);
}

TEST_F (DETest, RTE_test_inject_ok_signed2) {
    DEReceiver<autosar::PinionSteerAg1_info> receiver;

    receiver.subscribe([&receiver]() {
        EXPECT_TRUE (receiver.get().isOk());
        EXPECT_EQ (-16384.0 * 9.765625E-4, receiver.get().value().PinionSteerAg1);
        cv_cont=true;
        cv.notify_all();
    });
    std::unique_lock<std::mutex> lk(cv_m);
    cv_cont=false;

    const int signalId = SignalGroup | ComConf_ComSignalGroup_igPinionSteerAg1Group_mrx;
    PinionSteerAg1Group rteType;

    rteType.PinionSteerAg1 = 0x4000; // PinionSteerAg1 is 15 bit signed int so this represents -16384

    vipcomm::injectIncomingOkSignal(signalId, &rteType, sizeof(rteType));

    bool signalReceivedWithinExpectedTime = cv.wait_for(lk, waitTimeout, [](){return cv_cont.load();});
    EXPECT_TRUE(signalReceivedWithinExpectedTime);
}

TEST_F (DETest, RTE_test_inject_ok_signed3) {
    DEReceiver<autosar::PinionSteerAg1_info> receiver;

    receiver.subscribe([&receiver]() {
        EXPECT_TRUE (receiver.get().isOk());
        EXPECT_EQ (1000.0 * 9.765625E-4, receiver.get().value().PinionSteerAg1);
        cv_cont=true;
        cv.notify_all();
    });
    std::unique_lock<std::mutex> lk(cv_m);
    cv_cont=false;

    const int signalId = SignalGroup | ComConf_ComSignalGroup_igPinionSteerAg1Group_mrx;
    PinionSteerAg1Group rteType;

    rteType.PinionSteerAg1 = 1000; // PinionSteerAg1 is 15 bit signed int so this represents 1000

    vipcomm::injectIncomingOkSignal(signalId, &rteType, sizeof(rteType));

    bool signalReceivedWithinExpectedTime = cv.wait_for(lk, waitTimeout, [](){return cv_cont.load();});
    EXPECT_TRUE(signalReceivedWithinExpectedTime);
}

TEST_F (DETest, RTE_test_inject_ok_unsigned1) {
    DEReceiver<autosar::EngOilLvl_info> receiver;

    receiver.subscribe([&receiver]() {
        EXPECT_TRUE (receiver.get().isOk());
        EXPECT_EQ (5.0 * 0.5, receiver.get().value());
        cv_cont=true;
        cv.notify_all();
    });
    std::unique_lock<std::mutex> lk(cv_m);
    cv_cont=false;

    const int signalId = ComConf_ComSignal_isEngOilLvl_mrx;
    EngOilLvl rteType;

    rteType = 5;

    vipcomm::injectIncomingOkSignal(signalId, &rteType, sizeof(rteType));

    bool signalReceivedWithinExpectedTime = cv.wait_for(lk, waitTimeout, [](){return cv_cont.load();});
    EXPECT_TRUE(signalReceivedWithinExpectedTime);
}

TEST_F (DETest, RTE_test_inject_ok_unsigned2) {
    DEReceiver<autosar::EngOilLvl_info> receiver;

    receiver.subscribe([&receiver]() {
        EXPECT_TRUE (receiver.get().isOk());
        EXPECT_EQ (254.0 * 0.5, receiver.get().value());
        cv_cont=true;
        cv.notify_all();
    });
    std::unique_lock<std::mutex> lk(cv_m);
    cv_cont=false;

    const int signalId = ComConf_ComSignal_isEngOilLvl_mrx;
    EngOilLvl rteType;

    rteType = 254;

    vipcomm::injectIncomingOkSignal(signalId, &rteType, sizeof(rteType));

    bool signalReceivedWithinExpectedTime = cv.wait_for(lk, waitTimeout, [](){return cv_cont.load();});
    EXPECT_TRUE(signalReceivedWithinExpectedTime);
}

TEST_F (DETest, RTE_test_inject_error) {
    DEReceiver<autosar::AccSts_info> receiver;

    receiver.subscribe([&receiver]() {
        EXPECT_TRUE (receiver.get().isError());
        EXPECT_EQ (123, receiver.get().errorCode());
        cv_cont=true;
        cv.notify_all();
    });
    std::unique_lock<std::mutex> lk(cv_m);
    cv_cont=false;

    const int signalId = ComConf_ComSignal_isAccSts_mrx;
    const int errorCode=123;

    vipcomm::injectIncomingErrorSignal(signalId, errorCode);

    bool signalReceivedWithinExpectedTime = cv.wait_for(lk, waitTimeout, [](){return cv_cont.load();});
    EXPECT_TRUE(signalReceivedWithinExpectedTime);
}

TEST_F (DETest, RTE_test_sink_unitless_enum) {
    DESender<autosar::HdHwAprvdWirelsAdr_info> sender;

    std::unique_lock<std::mutex> lk(cv_m);
    cvsink_cont=false;

    autosar::HdHwAprvdWirelsAdr_info::data_elem_type senddata;
    senddata.HwOffsAdr1 = 7;
    senddata.HwOffsAdr6 = 250;
    senddata.AprvdSts_ = autosar::AprvdSts::Trusted;
    sender.send(senddata);

    bool signalReceivedWithinExpectedTime = cvsink.wait_for(lk, waitTimeout, [](){return cvsink_cont.load();});
    EXPECT_TRUE(signalReceivedWithinExpectedTime);
    EXPECT_EQ(ComConf_ComSignalGroup_igHdHwAprvdWirelsAdr_mtx|SignalGroup, sink_SignalId);
    EXPECT_EQ(sizeof(HdHwAprvdWirelsAdr), sink_signalData.size());
    HdHwAprvdWirelsAdr& rteType = *(HdHwAprvdWirelsAdr*)&sink_signalData[0];
    EXPECT_EQ(7, rteType.HdHwAprvdWirelsAdrHwOffsAdr1);
    EXPECT_EQ(250, rteType.HdHwAprvdWirelsAdrHwOffsAdr6);
    EXPECT_EQ(3, rteType.HdHwAprvdWirelsAdrAprvdSts);
}

TEST_F (DETest, RTE_test_sink_unsigned) {
    DESender<autosar::HmiCmptmtTSp_info> sender;

    std::unique_lock<std::mutex> lk(cv_m);
    cvsink_cont=false;

    autosar::HmiCmptmtTSp_info::data_elem_type senddata;

    senddata.HmiCmptmtTSpForRowFirstLe = 15.0;
    senddata.HmiCmptmtTSpForRowFirstRi = 30.5;
    senddata.HmiCmptmtTSpForRowSecLe = 142.5; // this is out-of-range BUT it should encode properly
    sender.send(senddata);

    bool signalReceivedWithinExpectedTime = cvsink.wait_for(lk, waitTimeout, [](){return cvsink_cont.load();});

    EXPECT_TRUE(signalReceivedWithinExpectedTime);
    EXPECT_EQ(ComConf_ComSignalGroup_igHmiCmptmtTSp_mtx|SignalGroup, sink_SignalId);
    EXPECT_EQ(sizeof(HmiCmptmtTSp), sink_signalData.size());
    HmiCmptmtTSp& rteType = *(HmiCmptmtTSp*)&sink_signalData[0];
    EXPECT_EQ(0, rteType.HmiCmptmtTSpForRowFirstLe);
    EXPECT_EQ((30.5-15.0)/0.5, rteType.HmiCmptmtTSpForRowFirstRi);
    EXPECT_EQ(255, rteType.HmiCmptmtTSpForRowSecLe);
}

TEST_F (DETest, RTE_checksum) {
    uint16_t com_arxml,swc_arxml,rte_type,com_cfg;
    vipcomm::getFileCrc(com_arxml, swc_arxml, rte_type, com_cfg);

    EXPECT_EQ(0,com_cfg);
    EXPECT_EQ(MP_ROUTER_CRC,rte_type);
    EXPECT_EQ(COM_CRC,com_arxml);
    EXPECT_EQ(SWC_CRC,swc_arxml);
}

TEST_F (DETest, Negative8BitValue_toSignedFromRaw_success) {
    uint8_t v = 31; // -1 for a 5-bit signed
    int8_t r = toSignedFromRaw<5>(v);
    EXPECT_EQ(-1,r);

    v = 254; // -2 for an 8-bit signed
    r = toSignedFromRaw<8>(v);
    EXPECT_EQ(-2,r);
}

TEST_F (DETest, Positive8BitValue_toSignedFromRaw_success) {
    uint8_t v = 3; // this corresponds to 3 in a 5 bit int
    int8_t r = toSignedFromRaw<5>(v);
    EXPECT_EQ(3,r);

    v = 127;
    r = toSignedFromRaw<8>(v);
    EXPECT_EQ(127,r);
}

TEST_F (DETest, Negative32BitValue_toSignedFromRaw_success) {
    uint32_t v = 0x7FFFE; // -2 for a 19-bit signed
    int32_t r = toSignedFromRaw<19>(v);
    EXPECT_EQ(-2,r);

    v = 0xFFFFFFFF; // -1 for an 32-bit signed
    r = toSignedFromRaw<32>(v);
    EXPECT_EQ(-1,r);
}

TEST_F (DETest, Positive32BitValue_toSignedFromRaw_success) {
    uint32_t v = 65536; // 65536 for a 19-bit signed
    int32_t r = toSignedFromRaw<19>(v);
    EXPECT_EQ(65536,r);

    v = 2000000000; // 4000000000 for an 32-bit signed
    r = toSignedFromRaw<32>(v);
    EXPECT_EQ(2000000000,r);
}

TEST_F (DETest, Positive16BitValue_toUnsignedFromRaw_success) {
    uint16_t v = 65535; // 65535 for a 16-bit unsigned
    int32_t r = toUnsignedFromRaw<16>(v);
    EXPECT_EQ(65535,r);

    v = 1; // 1 for an 4-bit signed
    r = toUnsignedFromRaw<4>(v);
    EXPECT_EQ(1,r);
}

TEST_F (DETest, ReadDataelementsVector_verify_instances) {

    const std::vector<autosar::DataElemInfo*>& v = vipcomm::getAllDataelements();

    EXPECT_GT((int)v.size(), 500);

    EXPECT_STREQ("ADataRawSafe", v[0]->name());
    EXPECT_EQ(autosar::Dir::IN, v[0]->direction());
}

TEST_F (DETest, CompileTest) {
    CompileTest ct;

    DEReceiver<autosar::ConSftyWarn_info> receiver;
    receiver.subscribe(std::bind(&CompileTest::test,&ct));
    receiver.subscribeVal(std::bind(&CompileTest::test,&ct));

    DESink<autosar::LaneKeepAidActv_info> sink;
    sink.subscribe(std::bind(&CompileTest::test,&ct));
    sink.subscribeVal(std::bind(&CompileTest::test,&ct));
}
