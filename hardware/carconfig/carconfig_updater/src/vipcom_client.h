/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <DesipClient.hpp>
#include <mutex>
#include <queue>
#include <string>
#include <thread>

#define TIMEOUT_USEC 1000   // one timeout every 1 ms
#define TIMEOUT_COUNT 1000  // total waiting time with 1000 timeouts is 1 s
#define INVALID_VIP_REPLY 0xff

/**
 * Simple structure to contain messages coming from VIP
 */
typedef struct {
    uint8_t fid;
    uint8_t data_length;
    int8_t data[35];
} vip_msg;

class CarConfigVipCom : public DesipClient {
  public:
    explicit CarConfigVipCom();

    ~CarConfigVipCom();

    int32_t versionReport(void);
    int32_t versionRequest(void);

    int sendConfig(std::vector<int8_t>& values);
    int waitForVipAcknowledge();
    void onMessage(const uint8_t& _fid, const int8_t _payload[20]);
    void VipReader();

  private:
    void setRxMsgID(ParcelableDesipMessage* msg);

    std::thread vipReader;
    uint8_t vipAcknowledge = INVALID_VIP_REPLY;

    typedef enum {
        sysVersionRequest = 0x00,
        sysVersionReport = 0x01,
        sysRepCarConfigAck = 0x01,
        sysRepCarConfigNack = 0x02,
        sysGetCarConfigFid = 0x24,
        sysRepCarConfigFid = 0x25,
        sysSetCarConfigFid = 0x26,
        sysSetCarConfigControlByte = 0x00
    } hisipBytes;
};

class CC_Desip_Listener : public DesipClient::DesipClientListener {
  public:
    CC_Desip_Listener(CarConfigVipCom* carConfigVipCom);
    Status deliverMessage(const ParcelableDesipMessage& msg, bool* _aidl_return);

    String16 getId();

  private:
    CarConfigVipCom* carConfigVipCom;
};