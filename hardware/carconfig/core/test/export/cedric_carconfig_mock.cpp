#include <assert.h>
#include <cedric_carconfig_mock.h>

static CarConfigMock* carConfigMock;

CarConfigMock::CarConfigMock()
{
    assert(carConfigMock == nullptr);
    carConfigMock = this;
}

CarConfigMock::~CarConfigMock()
{
    carConfigMock = nullptr;
}

namespace cedric
{
namespace core
{
namespace carconfig
{
std::int32_t init(void)
{
    assert(carConfigMock != nullptr);
    return carConfigMock->init();
}

std::uint8_t getValue(std::uint32_t parameterNumber)
{
    assert(carConfigMock != nullptr);
    return carConfigMock->getValue(parameterNumber);
}

std::uint8_t getRawValue(std::uint32_t parameterNumber)
{
    assert(carConfigMock != nullptr);
    return carConfigMock->getRawValue(parameterNumber);
}

bool isConfigured()
{
    assert(carConfigMock != nullptr);
    return carConfigMock->isConfigured();
}
}
}
}
