/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <utils/SystemClock.h>
#include <vhal_v2_0/VehicleUtils.h>
#include <functional>
#include <memory>

#include <cutils/log.h>
#include "Log.h"
#include "cartimemodule.h"

#undef LOG_TAG
#define LOG_TAG "CarTimeModule"

using namespace android::hardware::automotive::vehicle::V2_0;

using namespace ApplicationDataElement;
using namespace autosar;
using namespace android;
using namespace tarmac::eventloop;

CarTimeHal::CarTimeHal(vhal20::impl::IVehicleHalImpl* vehicleHal)
    : vhal20::impl::ModuleBase(vehicleHal), settime_job_(0) {
    StartFlexraySubscribers();
}

int CarTimeHal::setProp(const VehiclePropValue& propValue) {
    ALOGD("setProp: 0x%0x", propValue.prop);
    switch (propValue.prop) {
        case toInt(VehicleProperty::UNIX_TIME):
            StartSetTimeSequenceOnFlexray(propValue.value.int64Values[0]);
            return vhal20::impl::Status::SUCCESS;
            break;

        default:
            ALOGW("Unknown setProp: 0x%0x", propValue.prop);
            return vhal20::impl::Status::INVALID_PARAM_ERROR;
            break;
    }
}

std::unique_ptr<VehiclePropValue> CarTimeHal::getProp(const VehiclePropValue& requestedPropValue) {
    ALOGV("getProp: 0x%0x", requestedPropValue.prop);
    VehiclePropValue prop_value;
    switch (requestedPropValue.prop) {
        case toInt(VehicleProperty::UNIX_TIME):
            GetTimeFromFlexray(prop_value);
            break;

        default:
            ALOGW("Unknown getProp: 0x%0x", requestedPropValue.prop);
            return nullptr;
            break;
    }
    return std::make_unique<VehiclePropValue>(prop_value);
}

std::vector<VehiclePropValue> CarTimeHal::getAllPropValues() {
    VehiclePropValue cem_time;
    GetTimeFromFlexray(cem_time);
    return {cem_time};
}

std::vector<VehiclePropConfig> CarTimeHal::listProperties() {
    VehiclePropConfig timeconfig;

    timeconfig.prop = toInt(VehicleProperty::UNIX_TIME);
    timeconfig.access = VehiclePropertyAccess::READ_WRITE;
    timeconfig.changeMode = VehiclePropertyChangeMode::ON_SET;
    timeconfig.supportedAreas = toInt(VehicleAreaZone::WHOLE_CABIN);
    timeconfig.areaConfigs.resize(1);
    timeconfig.areaConfigs[0].areaId = toInt(VehicleAreaZone::WHOLE_CABIN);
    timeconfig.areaConfigs[0].minInt64Value = 0x0000000000000000;
    timeconfig.areaConfigs[0].maxInt64Value = 0x7FFFFFFFFFFFFFFF;

    return {timeconfig};
}

void CarTimeHal::StartFlexraySubscribers() {
    time_receiver_.subscribe([this]() {
        VehiclePropValue prop_value;
        // Only push a new value "upwards" when the time signal value is valid and we are currently not setting the CEM
        // time
        if (GetTimeFromFlexray(prop_value)) {
            if (settime_job_ == 0) {
                ALOGV("Time changed to: %ld", prop_value.value.int64Values[0]);
                pushProp(prop_value);
            } else {
                // We are currently trying to set the time in the CEM.
                // Lets compare the received time with what we have attempted to set.
                // The diff shall be 1 or 0, then we are done :)
                const time_t diff = prop_value.value.int64Values[0] - timetoset_;
                if (diff == 0 || diff == 1) {
                    // we are done !! :)
                    IDispatcher::GetDefaultDispatcher().Cancel(settime_job_);
                    StopSetTimeSequenceOnFlexray();
                    pushProp(prop_value);
                    ALOGI("CEM time set sequence ended: %ld", prop_value.value.int64Values[0]);
                }
            }
        }
    });
}

bool CarTimeHal::GetTimeFromFlexray(VehiclePropValue& prop_value) {
    prop_value.timestamp = elapsedRealtimeNano();
    prop_value.areaId = 0;
    prop_value.prop = toInt(VehicleProperty::UNIX_TIME);
    prop_value.value.int64Values.resize(1);
    prop_value.value.int64Values[0] = 0;  // A default indicating that something is wrong

    const auto& signal_state_value = time_receiver_.get();
    // only handle time-signal if state==Ok and that the CEM indicates the value is valid
    if (signal_state_value.isOk() && (signal_state_value.value().DataValid == NoYes1::Yes)) {
        const DateTi30 datetime = signal_state_value.value();
        struct tm tmtime;
        tmtime.tm_year = 100 + datetime.Yr1;  // (2000 + datetime.Yr1) - 1900;
        tmtime.tm_mon = datetime.Mth1 - 1;
        tmtime.tm_mday = datetime.Day;
        tmtime.tm_hour = datetime.Hr1;
        tmtime.tm_min = datetime.Mins1;
        tmtime.tm_sec = datetime.Sec1;
        tmtime.tm_isdst = -1;

        const time_t epoch1970 = mktime(&tmtime);
        if (epoch1970 > 0) {
            prop_value.value.int64Values[0] = epoch1970;
            return true;
        } else {
            ALOGW("Failed to convert CEM time to epoch (%d-%d-%d %d:%d:%d)", datetime.Yr1, datetime.Mth1, datetime.Day,
                  datetime.Hr1, datetime.Mins1, datetime.Sec1);
            return false;
        }
    } else {
        ALOGI("%s signal TiAndDateIndcn is in error or invalid.", __FUNCTION__);
        return false;
    }
}

void CarTimeHal::StartSetTimeSequenceOnFlexray(time_t timetoset) {
    activatevfc_.send({Vfc::UserInputSettings, 3});  // Make sure the CEM & FR is alive
    IDispatcher::EnqueueTask([this, timetoset]() {
        if (settime_job_ > 0) {  // stop any potential already running set sequence
            IDispatcher::GetDefaultDispatcher().Cancel(settime_job_);
            settime_job_ = 0;
        }
        timetoset_ = timetoset;
        struct tm datetime;
        localtime_r(&timetoset_, &datetime);
        DateTi1ForSet datetime_signal_value;
        datetime_signal_value.Year = datetime.tm_year - 100;
        datetime_signal_value.Month = datetime.tm_mon + 1;
        datetime_signal_value.Day = datetime.tm_mday;
        datetime_signal_value.Hour = datetime.tm_hour;
        datetime_signal_value.Minute = datetime.tm_min;
        datetime_signal_value.Second = datetime.tm_sec;
        datetime_signal_value.DateOrTi_ = DateOrTi::DateTi;

        time_sender_.send(datetime_signal_value);  // set time in CEM

        settime_job_ = IDispatcher::EnqueueTaskWithDelay(std::chrono::milliseconds(1000), [this]() {
            // it seems we didn't receive anything within 1 sec -> lets end this
            StopSetTimeSequenceOnFlexray();
            ALOGW("Failed to receive response from CEM withing 1sec");
        });
    });
}

void CarTimeHal::StopSetTimeSequenceOnFlexray() {
    settime_job_ = 0;
    activatevfc_.send({Vfc::UserInputSettings, 1});
    DateTi1ForSet datetime_signal_value;  // tell the CEM we are no more setting time
    datetime_signal_value.DateOrTi_ = DateOrTi::None;
    time_sender_.send(datetime_signal_value);
}
