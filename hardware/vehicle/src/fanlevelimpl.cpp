#include "fanlevelimpl.h"
#include "Application_dataelement.h"

#include <cutils/log.h>

#undef LOG_TAG
#define LOG_TAG "FanLevelImpl"

using namespace ApplicationDataElement;
using namespace autosar;

constexpr uint8_t FAN_OFF = 0;
constexpr uint8_t FAN_1 = 1;
constexpr uint8_t FAN_2 = 2;
constexpr uint8_t FAN_3 = 3;
constexpr uint8_t FAN_4 = 4;
constexpr uint8_t FAN_5 = 5;
constexpr uint8_t FAN_MAX = 6;

FanLevelImpl::FanLevelImpl(NotifiableProperty<int32_t>& fanLevel) : m_fanLevel(fanLevel) {
    setFanLevel(FAN_3);  // TODO Hard coded init value due to no persistant settings are available
}

void FanLevelImpl::setFanLevel(int32_t fanLevel) {
    ALOGI("Send HmiHvacFanLvlFrnt_info for setting fan speed to: %d", fanLevel);
    m_fanLevel.set(fanLevel);

    HmiHvacFanLvl autosarFanSpeed;
    switch (fanLevel) {
        case FAN_OFF:
            autosarFanSpeed = HmiHvacFanLvl::Off;
            break;

        case FAN_1:
            autosarFanSpeed = HmiHvacFanLvl::LvlAutMinusMinus;
            break;

        case FAN_2:
            autosarFanSpeed = HmiHvacFanLvl::LvlAutMinus;
            break;

        case FAN_3:
            autosarFanSpeed = HmiHvacFanLvl::LvlAutoNorm;
            break;

        case FAN_4:
            autosarFanSpeed = HmiHvacFanLvl::LvlAutPlus;
            break;

        case FAN_5:
            autosarFanSpeed = HmiHvacFanLvl::LvlAutPlusPlus;
            break;

        case FAN_MAX:
            autosarFanSpeed = HmiHvacFanLvl::Max;
            break;

        default:
            ALOGW("Non-supported fan_speed: %d\n", fanLevel);
            return;
    }

    DESender<HmiHvacFanLvlFrnt_info> fanLvlFrntSender;
    fanLvlFrntSender.send(autosarFanSpeed);
}
