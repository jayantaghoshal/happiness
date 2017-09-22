#ifndef MOCK_VIPCOM_CLIENT_H
#define MOCK_VIPCOM_CLIENT_H

#include <gmock/gmock.h>
#include <vipcom_client.h>

class CarConfigVipComMock : public CarConfigVipCom
{
public:
    MOCK_METHOD1(sendConfig, int(std::vector<uint8_t> &values));
    MOCK_METHOD0(waitForVipAcknowledge, int());
    MOCK_METHOD2(onMessage, void(const uint8_t &_fid, const std::vector<uint8_t> &_payload));
    MOCK_METHOD1(onConnectionError, void(ConnectionError e));
};

#endif // MOCK_VIPCOM_CLIENT_H