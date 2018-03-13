/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once
#include <iostream>
#include "vhal_util.h"

class VhalListener : public vhal20::IVehicleCallback, public ::android::hardware::hidl_death_recipient {
  public:
    explicit VhalListener(::android::sp<vhal20::IVehicle> service) : service_{std::move(service)} {}

    void startLinkToDeath() {
        // Causes segfault when placed in constructor, because "this" is not fully constructed ???
        auto result = service_->linkToDeath(this, 0xdead);
        if (!result.isOk()) {
            std::cout << "ERROR: Failed to linkToDeath: " << result.description() << std::endl;
            std::cout << "       VHAL restarts will not be notified!" << std::endl;
        }
    }

    void serviceDied(uint64_t cookie, const android::wp<::android::hidl::base::V1_0::IBase>& who) override {
        (void)who;
        (void)cookie;
        std::cout << "ERROR: VHAL service died" << std::endl;
        // TODO: Automatic reconnect
        exit(1);
    }

    ::android::hardware::Return<void> onPropertyEvent(
            const ::android::hardware::hidl_vec<vhal20::VehiclePropValue>& values) override {
        for (auto const& prop_value : values) {
            VhalUtil::DumpPropertyValue(prop_value);
            std::cout << std::endl;
        }
        return ::android::hardware::Return<void>();
    }

    ::android::hardware::Return<void> onPropertySet(const vhal20::VehiclePropValue& /*value*/) override {
        // Ignore the direct set calls (we don't expect to make any anyway)
        return ::android::hardware::Return<void>();
    }

    ::android::hardware::Return<void> onPropertySetError(vhal20::StatusCode /* errorCode */,
                                                         int32_t /* propId */,
                                                         int32_t /* areaId */) override {
        // We don't set values, so we don't listen for set errors
        return ::android::hardware::Return<void>();
    }

  private:
    ::android::sp<vhal20::IVehicle> service_;
};
