
#include <avmp.h>
#include <message_api.h>
#include <vipcomm/VipFramework.h>
#include <vsm_inject.h>
#include <vsm_sink.h>

#undef LOG_TAG
#define LOG_TAG "VSD"
#include <cutils/log.h>

extern bool avmpVersionCheckOk;
extern bool avmpHeartbeatReceived;

static void logMessage(const uint8_t *message, const uint32_t data_size);
static void handleAvmpCtrlMsg(const uint16_t avmpHeader, uint8_t *msg_data, const uint32_t data_size);

void avmpMessageInject(uint8_t *msg_data, const uint32_t data_size)
{
  uint16_t avmpHeader;

  if (nullptr == msg_data)
  {
    ALOGE("avmpMessageInject called with nullptr.");
    return;
  }

  logMessage(msg_data, data_size);

  if (data_size <= avmp::avmpHeaderSize)
  {
    ALOGE("avmpMessageInject called with no header and/or no payload. Data size is %d", data_size);
    return;
  }

  avmpHeader = (msg_data[1] << 8) | msg_data[0];

  if (0 != (avmp::controlMsgMask & avmpHeader))
  {
    ALOGD("handleAvmpCtrlMsg()");
    handleAvmpCtrlMsg(avmpHeader, msg_data, data_size);
  }
  else
  {
    ALOGV("vsm_inject_inject()");
    vsm_inject_inject((avmp::signalIdMask & avmpHeader) | (avmp::signalGroupMask & avmpHeader),
                      static_cast<void *>(&msg_data[avmp::payloadOffset]), 0 != (avmp::errorMask & avmpHeader),
                      data_size - avmp::avmpHeaderSize);
  }
}

static void handleAvmpCtrlMsg(const uint16_t avmpHeader, uint8_t *msg_data, const uint32_t data_size)
{
  uint16_t ctrlMsgId = avmp::controlMsgIdMask & avmpHeader;

  ALOGV("Received AVMP control message with ID %d", ctrlMsgId);

  if (avmp::heartBeatMsgId == ctrlMsgId)
  {
    if ((avmp::avmpHeaderSize + avmp::heartBeatPayloadSize) != data_size)
    {
      ALOGW("Heart beat message has incorrect size, expected %d received %d",
            (avmp::avmpHeaderSize + avmp::heartBeatPayloadSize), data_size);

      return;
    }

    Message_Send_T message;

    // Bounce back same heart beat
    message.data_ptr = msg_data;
    message.data_size = avmp::avmpHeaderSize + avmp::heartBeatPayloadSize;

    ALOGD("Sending AVMP heartbeat %d", msg_data[2]);

    messageSend(&message);

    avmpHeartbeatReceived = true;
  }
  else if (avmp::versionHandshakeMsgId == ctrlMsgId)
  {
    if ((avmp::avmpHeaderSize + avmp::versHandshakePayloadSize) != data_size)
    {
      ALOGW("Version handshake message has incorrect size expected = %d but received %d",
            (avmp::avmpHeaderSize + avmp::versHandshakePayloadSize), data_size);
      return;
    }

    uint8_t avmpVersion = msg_data[2];
    uint16_t comCrc = (msg_data[4] << 8) | msg_data[3];
    uint16_t swcCrc = (msg_data[6] << 8) | msg_data[5];
    uint16_t rteTypeCrc = (msg_data[8] << 8) | msg_data[7];
    uint16_t comCfgCrc = (msg_data[10] << 8) | msg_data[9];

    uint16_t myComCrc, mySwcCrc, myRteTypeCrc, myComCfgCrc;
    vipcomm::getFileCrc(myComCrc, mySwcCrc, myRteTypeCrc, myComCfgCrc);

    if ((avmp::avmpVersion == avmpVersion) && (myComCrc == comCrc) && (mySwcCrc == swcCrc) &&
        (myRteTypeCrc == rteTypeCrc) && (myComCfgCrc == comCfgCrc))
    {
      uint8_t avmpHeader[avmp::avmpHeaderSize];
      Message_Send_T message;

      avmpVersionCheckOk = true;

      // Send a "send all" control message to the VIP to request all
      // Flexray and LIN signals received by the VIP so far
      avmpHeader[0] = avmp::sendAllMsgId;
      avmpHeader[1] = avmp::controlMsgByteMask;
      message.data_ptr = avmpHeader;
      message.data_size = avmp::avmpHeaderSize;

      ALOGI("Sending AVMP 'Send all' command");
      messageSend(&message);

      // Now we can also start sinking/sending signals to the VIP
      vsm_sink_init();
    }
    else
    {
      ALOGE("AVMP version control failed");
      ALOGI("AVMP version, expected %d , received %d", avmp::avmpVersion, avmpVersion);
      ALOGI("COM CRC, expected %d , received %d", myComCrc, comCrc);
      ALOGI("SWC CRC, expected %d, received  %d", mySwcCrc, swcCrc);
      ALOGI("RTE type CRC, expected %d , received = %d", myRteTypeCrc, rteTypeCrc);
      ALOGI("COM CFG CRC, expected %d received %d", myComCfgCrc, comCfgCrc);
    }
  }
  else
  {
    ALOGW("Unknown AVMP control message with ID %d was discarded", ctrlMsgId);
  }
}

static void logMessage(const uint8_t *message, const uint32_t data_size)
{
  if (data_size > 4)
  {
    ALOGV("AVMP message data: 0x%02X%02X%02X%02X%02X", message[0], message[1], message[2], message[3], message[4]);
  }
  else if (data_size > 3)
  {
    ALOGV("AVMP message data: 0x%02X%02X%02X%02X", message[0], message[1], message[2], message[3]);
  }
  else if (data_size > 2)
  {
    ALOGV("AVMP message data: 0x%02X%02X%02X", message[0], message[1], message[2]);
  }
  else if (data_size > 1)
  {
    ALOGV("AVMP message data: 0x%02X%02X", message[0], message[1]);
  }
  else if (data_size > 0)
  {
    ALOGV("AVMP message data: 0x%02X", message[0]);
  }
  else
  {
    ALOGV("No AVMP message data");
  }
}
