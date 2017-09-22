#ifndef VIPCOM_CLIENT_H
#define VIPCOM_CLIENT_H

#include <vipcom/vipcom_api.h>

#define TIMEOUT_USEC 1000 // one timeout every 1 ms
#define TIMEOUT_COUNT 1000 // total waiting time with 1000 timeouts is 1 s
#define INVALID_VIP_REPLY 0xff

class CarConfigVipCom : public IVipComClient
{
public:
    explicit CarConfigVipCom();

    // Returns -1 on failure
    int sendConfig(std::vector<uint8_t> &values);
    // Blocking function, returns -1 if VIP provided no response or NACK/invalid value, 0 otherwise
    int waitForVipAcknowledge();

private:
    VipCom vipCom;
    bool vipResponded;
    uint8_t vipAcknowledge;
    void onMessage(const uint8_t &_fid, const std::vector<uint8_t> &_payload);
    void onConnectionError(ConnectionError e);

    typedef enum
    {
        sysRepCarConfigFid = 0x25,
        sysRepCarConfigAck = 0x01,
        sysRepCarConfigNack = 0x02,
        sysSetCarConfigFid = 0x26,
        sysSetCarConfigControlByte = 0x00
    } hisipBytes;
};

#endif // VIPCOM_CLIENT_H
