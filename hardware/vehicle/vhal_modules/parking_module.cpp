/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "parking_module.h"
#include <vhal_v2_0/VehicleUtils.h>
#include "carconfig.h"
#include "cc_parameterlist.h"
#include "interface_mapping.h"

#include <cutils/log.h>
#include <functional>
#include <memory>

#undef LOG_TAG
#define LOG_TAG "ParkingModule"

using namespace android::hardware::automotive::vehicle::V2_0;
using namespace std::placeholders;
using namespace android;
using namespace ApplicationDataElement;
using namespace autosar;
using namespace CarConfigParams;

vhal20::VehiclePropConfig propconfig_request_pas_hide() {
    auto prop = vccvhal10::VehicleProperty::PAS_REQUEST_PAS_HIDE;
    return PaPropHandlerHelper::BoolConfig(prop);
}

ParkingModule::ParkingModule(gsl::not_null<VFContext*> ctx)
    : PA_prop_request_pas_hide(propconfig_request_pas_hide(),
                               vccvhal10::VehicleProperty::PAS_REQUEST_PAS_HIDE_STATUS,
                               ctx->dispatcher,
                               &(ctx->vhal)),
      vehmod_flexray_receiver_{ctx->dataelements},
      trlrprsnt_flexray_receiver_{ctx->dataelements},
      prkgassiactvsts_flexray_receiver_{ctx->dataelements},
      nfsdatafront_flexray_receiver_{ctx->dataelements},
      nfsdatarear_flexray_receiver_{ctx->dataelements},
      autnmsprkgactvscenario_flexray_receiver_{ctx->dataelements},
      is_error_(false),
      is_active_(true),
      request_pas_hide_(false),
      is_invalid_pas_signal_(false),
      is_alivetimeout_(false) {
    // Check if enabled
    auto car_config_142 = carconfig::getValue<CC142_ParkingAssistanceType>();

    const bool is_enabled_ = car_config_142 > CC142_ParkingAssistanceType::Without_parking_assistance;

    if (!is_enabled_) {
        ALOGD("Function disabled due to CarConfig.");
        request_pas_hide_ = false;
        return;
    } else {
        ALOGD("Park Assist System enabled in CarConfig.");
    }

    // Start listen to Vhal
    PA_prop_request_pas_hide.registerToVehicleHal();
    PA_prop_request_pas_hide.subscribe_set_prop([&](bool value, int32_t zone) {
        (void)zone;
        ALOGD("PA_prop_request_pas_hide: %d", value);
    });

    // Check if function is active
    vehmod_flexray_receiver_.subscribe([&]() {
        const auto signal = vehmod_flexray_receiver_.get();
        if (signal.isOk()) {
            auto vehmod_value = signal.value();

            is_active_ = (vehmod_value.UsgModSts == UsgModSts1::UsgModActv &&
                          vehmod_value.CarModSts1_ == CarModSts1::CarModNorm);

            ALOGD("is_active_ is changed: %d", is_active_);
            Update();
        } else {
            ALOGD("signal not OK");
        }
    });

    trlrprsnt_flexray_receiver_.subscribe([&]() {
        const auto signal = trlrprsnt_flexray_receiver_.get();
        ALOGD("trlrprsnt signal changed");
    });

    prkgassiactvsts_flexray_receiver_.subscribe([&]() {
        const auto signal = prkgassiactvsts_flexray_receiver_.get();
        ALOGD("prkgassiactvsts signal changed");
    });

    nfsdatafront_flexray_receiver_.subscribe([&]() {
        const auto signal = nfsdatafront_flexray_receiver_.get();
        ALOGD("nfsdatafront signal changed");
    });

    nfsdatarear_flexray_receiver_.subscribe([&]() {
        const auto signal = nfsdatarear_flexray_receiver_.get();
        ALOGD("nfsdatarear signal changed");
    });

    autnmsprkgactvscenario_flexray_receiver_.subscribe([&]() {
        const auto signal = autnmsprkgactvscenario_flexray_receiver_.get();
        ALOGD("autnmsprkgactvscenario signal changed");
    });
}

void ParkingModule::Update() {
    is_error_ = (is_alivetimeout_ || is_invalid_pas_signal_);

    ALOGD("is_error_: %d", is_error_);

    if (is_error_) {
        PA_prop_request_pas_hide.PushProp(false, vccvhal10::PAStatus::SystemError);
    } else {
        if (!is_active_) {
            PA_prop_request_pas_hide.PushProp(false, vccvhal10::PAStatus::Disabled);
        } else {
            PA_prop_request_pas_hide.PushProp(request_pas_hide_, vccvhal10::PAStatus::Active);
        }
    }
}
