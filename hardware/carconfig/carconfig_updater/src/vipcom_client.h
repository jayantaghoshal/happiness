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
#include "carconfig_base.h"
#include "carconfig_reader.h"

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

    void versionReport(void);
    void versionRequest(void);
    void setTransfer(void);
    void checksumCmd(const int8_t payload[35]);
    void dataRequest(const int8_t payload[35]);
    bool sendDESIPMsg(ParcelableDesipMessage msg);
    int32_t calculateChecksum(char* data, int32_t calcLength);
    int waitForVipAcknowledge();
    void onMessage(const uint8_t& _fid, const int8_t _payload[35]);
    void VipReader();

  private:
    void setRxMsgID(ParcelableDesipMessage* msg);

    std::thread vipReader;
    CarConfigReader rd;
    uint8_t vipAcknowledge = INVALID_VIP_REPLY;
    int32_t ccChecksum;
    std::array<char, Carconfig_base::cc_no_of_parameters> ccList;

    typedef enum {
        carConfigVersionRequest = 0x00,
        carConfigVersionReport = 0x01,
        carConfigChecksumReport = 0x05,
        carConfigChecksumReportOk = 0x00,
        carConfigChecksumReportNok = 0x01,
        carConfigChecksumCmd = 0x06,
        carConfigDataRequest = 0x08,
        carConfigDataReport = 0x09,
        carConfigTransferReport = 0x0D,
        carConfigTransferReportOk = 0x00,
        carConfigTransferReportNok = 0x01,
        carConfigTransferCmd = 0x0E,
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