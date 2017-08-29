
/**
 * Unit test of the VSM.
 *
 * Production code under test:
 *   vipcommunication/vehiclesignalsmanager/src/vsm.cpp
 *
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vsm_inject_mock.h>
#include <misc_mock.h>
#include <vsm.h>

extern "C" {
int openReturnValue;
ssize_t readReturnValue;
int createReturnValue;
bool openCalledFromTest;
bool readCalledFromTest;
bool createCalledFromTest;
int readCalled;
int createCalled;
}

/* Create a mock of the messageSend-function */
class MessageSend
{
public:
   virtual void messageSend(Message_Send_T *msg_data) = 0;
};

class MessageSendMock: public MessageSend
{
public:
   MOCK_METHOD1(messageSend, void(Message_Send_T*));
};

static MessageSendMock *messageSendMockPtr;

class VsmTestFixture: public ::testing::Test
{
public:
   virtual void SetUp()
   {
      vsm_inject_mock_init(&vsmInjectMock);
      misc_mock_init(&miscMock);
      messageSendMockPtr = &messageSendMock;
      miscMock.selectReturnValue = 0;
      miscMock.unlinkReturnValue = 0;
      miscMock.mkfifoReturnValue = 0;
      readReturnValue = 0;
      openReturnValue = 0;
      createReturnValue = 0;
      errno = 0;
      readCalled = 0;
      createCalled = 0;
   }

   virtual void TearDown()
   {
   }

protected:
   VsmInjectMock vsmInjectMock;
   MessageSendMock messageSendMock;
   MiscMock miscMock;
};

void messageSend(Message_Send_T *msg_data)
{
   messageSendMockPtr->messageSend(msg_data);
}

using ::testing::_;

TEST_F (VsmTestFixture, vsmInit_noPresumptions_shallCallVsmInjectInit)
{
   EXPECT_CALL(vsmInjectMock, vsm_inject_init()).Times(1);
   vsm_init();
}

TEST_F (VsmTestFixture, vsmDeInit_noPresumptions_shallDoNothing)
{
   vsm_deInit();
}

int main(int argc, char *argv[])
{
   ::testing::InitGoogleTest(&argc, argv);
   return RUN_ALL_TESTS();
}
