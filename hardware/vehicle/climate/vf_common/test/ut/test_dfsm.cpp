/*===========================================================================*\
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
\*===========================================================================*/

#include "dfsm.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace {
const int NO_STATE = -1;
const int FAKE_STATE = 5000;
}

class DFSMMainTest : public ::testing::Test {
public:
  DFSMMainTest() : callbackValue_{0} {}

protected:
  enum TestState {
    INIT = 0,
    STATE_1,
    STATE_2,
    STATE_2_1,
    STATE_2_1_1,
    STATE_2_1_2,
    STATE_2_2,
    STATE_2_2_1,
    STATE_2_2_2,
    STATE_3,
    STATE_3_1,
    STATE_3_2,
    STATE_4
  };

  int callbackValue_;

  std::unique_ptr<DFsm_Main> createSut() {
    return std::make_unique<DFsm_Main>();
  }
};

/*==============================================*/
/*               Tests begins here              */
/*==============================================*/

TEST_F(DFSMMainTest, Constructor_WhenNotCreateState_WillNotBeInInitState) {

  auto sut = createSut();

  EXPECT_TRUE(sut->inState(NO_STATE));
}

TEST_F(DFSMMainTest, Constructor_WhenNotRegisterState_WillNotBeInInitState) {

  auto sut = createSut();

  sut->createState(INIT);

  EXPECT_TRUE(sut->inState(NO_STATE));
}

TEST_F(DFSMMainTest, Constructor_WhenRegisterInitState_WillBeInInitState) {

  auto sut = createSut();

  sut->createState(INIT);
  sut->registerMainInitState(INIT);

  EXPECT_TRUE(sut->inState(INIT));
}

TEST_F(
    DFSMMainTest,
    TransitionToState_WhenNotRegisterInitStateAndChangeState_WillNotBeInNewState) {

  auto sut = createSut();

  sut->createState(INIT);
  sut->createState(STATE_1);

  sut->createTransition(INIT, STATE_1);

  sut->transitionToState(STATE_1);

  EXPECT_TRUE(sut->inState(NO_STATE));
}

TEST_F(
    DFSMMainTest,
    TransitionToState_WhenChangeStateAndNoTransitionCreated_WillNotBeInNewState) {

  auto sut = createSut();
  sut->createState(INIT);
  sut->registerMainInitState(INIT);

  sut->createState(STATE_1);

  sut->transitionToState(STATE_1);

  EXPECT_FALSE(sut->inState(STATE_1));
}

TEST_F(DFSMMainTest,
       TransitionToState_WhenRegisterInitStateAndChangeState_WillBeInNewState) {

  auto sut = createSut();
  sut->createState(INIT);
  sut->registerMainInitState(INIT);

  sut->createState(STATE_1);
  sut->createTransition(INIT, STATE_1);

  sut->transitionToState(STATE_1);

  EXPECT_TRUE(sut->inState(STATE_1));
}

TEST_F(
    DFSMMainTest,
    TransitionToState_WhenHavingRegisteredSubState_WillBeInBothParentAndChildStates) {

  auto sut = createSut();
  sut->createState(INIT);
  sut->registerMainInitState(INIT);

  sut->createState(STATE_1);
  sut->createState(STATE_2);
  sut->createState(STATE_2_1, STATE_2);

  sut->createTransition(INIT, STATE_1);
  sut->createTransition(STATE_1, STATE_2);

  sut->addInitState(STATE_2, STATE_2_1);

  sut->transitionToState(STATE_1);
  EXPECT_TRUE(sut->inState(STATE_1));

  sut->transitionToState(STATE_2);
  EXPECT_TRUE(sut->inState(STATE_2));
  EXPECT_TRUE(sut->inState(STATE_2_1));
}

TEST_F(DFSMMainTest,
       TransitionToState_WhenStateNotExist_WillStayInPreviousState) {

  auto sut = createSut();
  sut->createState(INIT);
  sut->registerMainInitState(INIT);

  sut->createState(STATE_1);

  sut->createTransition(INIT, STATE_1);
  sut->createTransition(STATE_1, FAKE_STATE);

  sut->transitionToState(STATE_1);
  EXPECT_TRUE(sut->inState(STATE_1));

  sut->transitionToState(FAKE_STATE);
  EXPECT_TRUE(sut->inState(STATE_1));
}

//*******************************************************************************************//
/*
// Testing setup:
// --------------
//                             |-------------------------------------------------------------|
//                             |STATE_2                                                      |
//                             |                                                        (H)  |
//   |--------------|          |   |-------------------------------------------------|       |
//   |INIT          |          |   |STATE_2_1                                   (H)  |       |
//   |--------------|          |   |                                                 |       |
//         |                   |   |  |----------------| ------> |----------------|  |       |
//         |                   |   |  |STATE_2_1_1     |         |STATE_2_1_2     |  |       |
//         |                   |   |  |----------------| <------ |----------------|  |       |
//         V                   |   |                                                 |       |
//   |--------------| -------> |   |-------------------------------------------------|       |
//   |STATE_1       |          |               ^                       |                     |
//   |--------------|          |               |                       V                     |
//         ^                   |   |-------------------------------------------------|       |
//         |                   |   |STATE_2_2                                   (H)  |       |
//         |                   |   |                                                 |       |
//         |                   |   |  |----------------| ------> |----------------|  |       |
//         |                   |   |  |STATE_2_2-1     |         |STATE_2_2_2     |  |       |
//         |                   |   |  |----------------| <------ |----------------|  |       |
//         |                   |   |                                                 |       |
//         |                   |   |-------------------------------------------------|       |
//         |                   |                                                             |
//         |                   |-------------------------------------------------------------|
//         |                                  ^                   |
//         |                                  |                   V
//         |                    |-------------------------------------------------|
//         |                    |STATE_3                                      (H) |
//         |                    |                                                 |
//   |--------------| ------->  |  |----------------| ------> |----------------|  |
//   |STATE_4       |           |  |STATE_3_1       |         |STATE_3_2       |  |
//   |--------------| <-------  |  |----------------| <------ |----------------|  |
//                              |                                                 |
//                              |-------------------------------------------------|
//
*/
//*******************************************************************************************//

TEST_F(DFSMMainTest,
       Overall_WhenGoingBetweenStates_WillBeInRightParentAndChildStates) {

  auto sut = createSut();
  sut->createState(INIT);
  sut->registerMainInitState(INIT);

  sut->createState(STATE_1);
  sut->createState(STATE_2);
  sut->createState(STATE_2_1, STATE_2);
  sut->createState(STATE_2_1_1, STATE_2_1);
  sut->createState(STATE_2_1_2, STATE_2_1);
  sut->createState(STATE_2_2, STATE_2);
  sut->createState(STATE_2_2_1, STATE_2_2);
  sut->createState(STATE_2_2_2, STATE_2_2);
  sut->createState(STATE_3);
  sut->createState(STATE_3_1, STATE_3);
  sut->createState(STATE_3_2, STATE_3);
  sut->createState(STATE_4);

  sut->createTransition(INIT, STATE_1);
  sut->createTransition(STATE_1, STATE_2);
  sut->createTransition(STATE_2_1, STATE_2_2);
  sut->createTransition(STATE_2_2, STATE_2_1);
  sut->createTransition(STATE_2_1_1, STATE_2_1_2);
  sut->createTransition(STATE_2_1_2, STATE_2_1_1);
  sut->createTransition(STATE_2_2_1, STATE_2_2_2);
  sut->createTransition(STATE_2_2_2, STATE_2_2_1);
  sut->createTransition(STATE_2, STATE_3);
  sut->createTransition(STATE_2, STATE_4);
  sut->createTransition(STATE_3, STATE_2);
  sut->createTransition(STATE_3, STATE_4);
  sut->createTransition(STATE_4, STATE_1);

  sut->addInitState(STATE_2, STATE_2_1);
  sut->addInitState(STATE_2_1, STATE_2_1_1);
  sut->addInitState(STATE_2_2, STATE_2_2_1);
  sut->addInitState(STATE_3, STATE_3_1);

  sut->transitionToState(STATE_1);
  EXPECT_TRUE(sut->inState(STATE_1));

  // Test not possible to go to STATE_3
  sut->transitionToState(STATE_3);
  EXPECT_FALSE(sut->inState(STATE_3));

  sut->transitionToState(STATE_2);
  EXPECT_TRUE(sut->inState(STATE_2));
  EXPECT_TRUE(sut->inState(STATE_2_1));
  EXPECT_TRUE(sut->inState(STATE_2_1_1));

  sut->transitionToState(STATE_2_1_2);
  EXPECT_TRUE(sut->inState(STATE_2_1_2));

  sut->transitionToState(STATE_2_1_1);
  EXPECT_TRUE(sut->inState(STATE_2_1_1));

  sut->transitionToState(STATE_2_1_2);
  EXPECT_TRUE(sut->inState(STATE_2_1_2));

  // Test not possible to go between two states substate
  sut->transitionToState(STATE_2_2_2);
  EXPECT_FALSE(sut->inState(STATE_2_2_2));

  sut->transitionToState(STATE_2_2);
  EXPECT_TRUE(sut->inState(STATE_2));
  EXPECT_TRUE(sut->inState(STATE_2_2));
  EXPECT_TRUE(sut->inState(STATE_2_2_1));

  sut->transitionToState(STATE_2_2_2);
  EXPECT_TRUE(sut->inState(STATE_2_2_2));

  sut->transitionToState(STATE_2_2_1);
  EXPECT_TRUE(sut->inState(STATE_2_2_1));

  sut->transitionToState(STATE_2_2_2);
  EXPECT_TRUE(sut->inState(STATE_2_2_2));

  // Test not possible to go to STATE_1
  sut->transitionToState(STATE_1);
  EXPECT_FALSE(sut->inState(STATE_1));

  sut->transitionToState(STATE_3);
  EXPECT_TRUE(sut->inState(STATE_3));
  EXPECT_TRUE(sut->inState(STATE_3_1));

  // Test history point
  sut->transitionToState(STATE_2);
  EXPECT_TRUE(sut->inState(STATE_2));
  EXPECT_TRUE(sut->inState(STATE_2_2));
  EXPECT_TRUE(sut->inState(STATE_2_2_2));

  // Test history point
  sut->transitionToState(STATE_2_1);
  EXPECT_TRUE(sut->inState(STATE_2_1));
  EXPECT_TRUE(sut->inState(STATE_2_1_2));

  sut->transitionToState(STATE_4);
  EXPECT_TRUE(sut->inState(STATE_4));

  // Test not possible to go to STATE_3
  sut->transitionToState(STATE_3);
  EXPECT_FALSE(sut->inState(STATE_3));

  sut->transitionToState(STATE_1);
  EXPECT_TRUE(sut->inState(STATE_1));
}

TEST_F(DFSMMainTest, CallbackEntry_WhenStateEntrySet_WillTriggerLambdaCall) {

  auto sut = createSut();

  sut->createState(INIT);
  sut->registerMainInitState(INIT);

  sut->createState(STATE_1);
  sut->createState(STATE_2);

  // Test State Entry
  sut->addStateEntry(STATE_2, [this]() { this->callbackValue_++; });

  sut->createTransition(INIT, STATE_1);
  sut->createTransition(STATE_1, STATE_2);

  sut->transitionToState(STATE_1);
  EXPECT_TRUE(sut->inState(STATE_1));

  sut->transitionToState(STATE_2);
  EXPECT_TRUE(sut->inState(STATE_2));
  EXPECT_EQ(1, callbackValue_);
}

TEST_F(DFSMMainTest, CallbackExit_WhenStateExitSet_WillTriggerLambdaCall) {

  auto sut = createSut();

  sut->createState(INIT);
  sut->registerMainInitState(INIT);

  sut->createState(STATE_1);
  sut->createState(STATE_2);

  // Test State Exit
  sut->addStateExit(STATE_1, [this]() { this->callbackValue_++; });

  sut->createTransition(INIT, STATE_1);
  sut->createTransition(STATE_1, STATE_2);

  sut->transitionToState(STATE_1);
  EXPECT_TRUE(sut->inState(STATE_1));

  sut->transitionToState(STATE_2);
  EXPECT_TRUE(sut->inState(STATE_2));
  EXPECT_EQ(1, callbackValue_);
}

TEST_F(DFSMMainTest, CallbackBody_WhenStateBodySet_WillTriggerLambdaCall) {

  auto sut = createSut();

  sut->createState(INIT);
  sut->registerMainInitState(INIT);

  sut->createState(STATE_1);
  sut->createState(STATE_2);

  // Test State Body
  sut->addStateBody(STATE_2, [this]() { this->callbackValue_++; });

  sut->createTransition(INIT, STATE_1);
  sut->createTransition(STATE_1, STATE_2);

  sut->transitionToState(STATE_1);
  EXPECT_TRUE(sut->inState(STATE_1));

  sut->transitionToState(STATE_2);
  EXPECT_TRUE(sut->inState(STATE_2));
  EXPECT_EQ(0, callbackValue_);

  sut->executeBody();
  EXPECT_EQ(1, callbackValue_);
}

TEST_F(DFSMMainTest,
       CallbackBody_WhenStateEntry_Exit_and_BodySet_WillTriggerLambdaCall) {

  auto sut = createSut();

  sut->createState(INIT);
  sut->registerMainInitState(INIT);

  sut->createState(STATE_1);

  sut->createState(STATE_2, 0, [this]() { callbackValue_++; },
                   [this]() { callbackValue_++; },
                   [this]() { callbackValue_++; });

  sut->createTransition(INIT, STATE_1);
  sut->createTransition(STATE_1, STATE_2);
  sut->createTransition(STATE_2, STATE_1);

  sut->executeBody();
  EXPECT_EQ(0, callbackValue_);

  sut->transitionToState(STATE_1);
  EXPECT_TRUE(sut->inState(STATE_1));

  sut->transitionToState(STATE_2);
  EXPECT_TRUE(sut->inState(STATE_2));
  EXPECT_EQ(1, callbackValue_);

  sut->executeBody();
  EXPECT_EQ(2, callbackValue_);

  sut->executeBody(STATE_1);
  EXPECT_EQ(2, callbackValue_);

  sut->executeBody(STATE_2);
  EXPECT_EQ(3, callbackValue_);

  sut->transitionToState(STATE_1);
  EXPECT_TRUE(sut->inState(STATE_1));
  EXPECT_EQ(4, callbackValue_);
}

TEST_F(DFSMMainTest,
       ExecuteTransition_WhenTransitionExistWithPositiveGuard_WillEnterNewState) {

  auto sut = createSut();

  sut->createState(INIT);
  sut->registerMainInitState(INIT);

  sut->createState(STATE_1);

  sut->createState(STATE_2);

  sut->createTransition(INIT, STATE_1);
  sut->createTransition(STATE_1, STATE_2, [this](){ return true; });
  sut->createTransition(STATE_2, STATE_1, [this]() { return false; });

  EXPECT_TRUE(sut->inState(INIT));

  sut->transitionToState(STATE_1);
  EXPECT_TRUE(sut->inState(STATE_1));

  sut->executeTransition(STATE_1, STATE_2);
  EXPECT_TRUE(sut->inState(STATE_2));

  sut->executeTransition(STATE_2, STATE_1);
  EXPECT_FALSE(sut->inState(STATE_1));

}

TEST_F(DFSMMainTest,
       addStateEntryBodyAndExit_WhenAlreadyExist_WillNotRegisterNew) {

  auto sut = createSut();

  sut->createState(INIT);
  sut->registerMainInitState(INIT);

  sut->createState(STATE_1);

  sut->createState(STATE_2);

  sut->createTransition(INIT, STATE_1);
  sut->createTransition(STATE_1, STATE_2);
  sut->createTransition(STATE_2, STATE_1);

  sut->addStateEntry(STATE_1, [this]() { this->callbackValue_++; });
  sut->addStateEntry(STATE_1, [this]() { this->callbackValue_++; });

  sut->addStateBody(STATE_1, [this]() { this->callbackValue_++; });
  sut->addStateBody(STATE_1, [this]() { this->callbackValue_++; });

  sut->addStateExit(STATE_1, [this]() { this->callbackValue_++; });
  sut->addStateExit(STATE_1, [this]() { this->callbackValue_++; });

  EXPECT_TRUE(sut->inState(INIT));

  sut->transitionToState(STATE_1);
  EXPECT_TRUE(sut->inState(STATE_1));
  EXPECT_EQ(1, callbackValue_);

  sut->executeBody(STATE_1);
  EXPECT_EQ(2, callbackValue_);

  sut->executeTransition(STATE_1, STATE_2);
  EXPECT_TRUE(sut->inState(STATE_2));
  EXPECT_EQ(3, callbackValue_);

  sut->executeBody(STATE_2);
  EXPECT_EQ(3, callbackValue_);

}

TEST_F(DFSMMainTest,
       CreateTransition_WhenStateNotExist_WillNotChangeToNonExistingState) {

  auto sut = createSut();

  sut->createState(INIT);
  sut->registerMainInitState(INIT);

  sut->createState(STATE_1);

  sut->createTransition(INIT, STATE_1);
  sut->createTransition(STATE_1, STATE_2);
  sut->createTransition(STATE_2, STATE_1);
  sut->createTransition(STATE_2, STATE_3);

  EXPECT_TRUE(sut->inState(INIT));

  sut->transitionToState(STATE_1);
  EXPECT_TRUE(sut->inState(STATE_1));

  sut->executeTransition(STATE_1, STATE_2);
  EXPECT_FALSE(sut->inState(STATE_2));

  sut->executeTransition(STATE_2, STATE_3);
  EXPECT_FALSE(sut->inState(STATE_3));
}

TEST_F(DFSMMainTest,
       CreateStateEntryBodyAndExit_WhenStateNotExist_WillNotRegisterLamdaFunction) {

    auto sut = createSut();

    sut->createState(INIT);
    sut->registerMainInitState(INIT);

    sut->createState(STATE_2);

    sut->createTransition(INIT, STATE_1);
    sut->createTransition(STATE_1, STATE_2);
    sut->createTransition(STATE_2, STATE_1);

    sut->addStateEntry(STATE_1, [this]() { this->callbackValue_++; });

    sut->addStateBody(STATE_1, [this]() { this->callbackValue_++; });

    sut->addStateExit(STATE_1, [this]() { this->callbackValue_++; });

    EXPECT_TRUE(sut->inState(INIT));

    sut->transitionToState(STATE_1);
    EXPECT_FALSE(sut->inState(STATE_1));
    EXPECT_EQ(0, callbackValue_);

    sut->executeBody();
    EXPECT_EQ(0, callbackValue_);

    sut->executeTransition(STATE_1, STATE_2);
    EXPECT_FALSE(sut->inState(STATE_2));
    EXPECT_EQ(0, callbackValue_);

    sut->executeBody(STATE_1);
    EXPECT_EQ(0, callbackValue_);
}

TEST_F(DFSMMainTest,
       AddInitState_WhenStateNotExist_WillNotChangeState) {

    auto sut = createSut();

    sut->createState(INIT);
    sut->registerMainInitState(INIT);

    sut->createState(STATE_1);

    sut->addInitState(STATE_1, STATE_2);

    sut->addInitState(STATE_2, STATE_3);

    sut->createTransition(INIT, STATE_1);
    sut->createTransition(STATE_1, STATE_2);
    sut->createTransition(STATE_2, STATE_1);
    sut->createTransition(STATE_1, STATE_3);

    EXPECT_TRUE(sut->inState(INIT));

    sut->transitionToState(STATE_1);
    EXPECT_TRUE(sut->inState(STATE_1));
    EXPECT_FALSE(sut->inState(STATE_2));

    sut->transitionToState(STATE_3);
    EXPECT_FALSE(sut->inState(STATE_3));

}

class DFSMInheranceTest : private DFsm_Main, public ::testing::Test
{
public:
    DFSMInheranceTest() {};
protected:
    enum TestState {
      INIT = 0,
      STATE_1,
      STATE_2,
      STATE_3,
      STATE_4
    };
    void registerFSM()
    {
        createState(INIT);
        registerMainInitState(INIT);

        createState(STATE_1);
        createState(STATE_2, 0, [this](){ transitionToState(STATE_3); });
        createState(STATE_3);

        createTransition(INIT, STATE_1);
        createTransition(STATE_1, STATE_2);
        createTransition(STATE_2, STATE_1);
        createTransition(STATE_2, STATE_3);

    }
    bool isInState(int state)
    {
        return inState(state);
    }
    void gotoToState(int toState)
    {
        transitionToState(toState);
    }
};

TEST_F(DFSMInheranceTest,
       executeTransition_WhenChangeStateWithinATransition_WillCallQueueTransition) {

    registerFSM();

    EXPECT_TRUE(isInState(INIT));

    gotoToState(STATE_1);
    EXPECT_TRUE(isInState(STATE_1));

    gotoToState(STATE_2);
    EXPECT_TRUE(isInState(STATE_3));
}
