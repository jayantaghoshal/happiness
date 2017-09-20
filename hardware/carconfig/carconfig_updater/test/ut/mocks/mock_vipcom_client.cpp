#include <mock_vipcom_client.h>

CarConfigVipCom::CarConfigVipCom() : vipCom(VipCom::Application::systemIpcIndex, this) {}

int CarConfigVipCom::sendConfig(std::vector<uint8_t> &values)
{
    return static_cast<CarConfigVipComMock*>(this)->sendConfig(values);
}

int CarConfigVipCom::waitForVipAcknowledge()
{
    return static_cast<CarConfigVipComMock*>(this)->waitForVipAcknowledge();
}

void CarConfigVipCom::onMessage(const uint8_t &_fid, const std::vector<uint8_t> &_payload)
{
    static_cast<CarConfigVipComMock*>(this)->onMessage(_fid, _payload);
}

void CarConfigVipCom::onConnectionError(ConnectionError e)
{
    static_cast<CarConfigVipComMock*>(this)->onConnectionError(e);
}