#include <cedric_carconfig.h>
#include <gmock/gmock.h>
#include <cstdint>

class CarConfigMock
{
  public:
    CarConfigMock();
    ~CarConfigMock();

    MOCK_METHOD0(init, std::int32_t());
    MOCK_METHOD1(getValue, std::uint8_t(std::uint32_t));
    MOCK_METHOD1(getRawValue, std::uint8_t(std::uint32_t));
    MOCK_METHOD0(isConfigured, bool());
};

