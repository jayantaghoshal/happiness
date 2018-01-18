/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "vhal_util.h"

class VhalListener : public vhal20::IVehicleCallback {
  public:
    explicit VhalListener(::android::sp<vhal20::IVehicle> service) : service_{std::move(service)} {}

    ::android::hardware::Return<void> onPropertyEvent(
            const ::android::hardware::hidl_vec<vhal20::VehiclePropValue>& values) override {
        for (auto const& prop_value : values) {
            VhalUtil::DumpPropertyValue(service_, prop_value);
        }
        return ::android::hardware::Return<void>();
    }

    ::android::hardware::Return<void> onPropertySet(const vhal20::VehiclePropValue& /*value*/) override {
        // Ignore the direct set calls (we don't expect to make any anyway)
        return ::android::hardware::Return<void>();
    }

    ::android::hardware::Return<void> onPropertySetError(vhal20::StatusCode /* errorCode */, int32_t /* propId */,
                                                         int32_t /* areaId */) override {
        // We don't set values, so we don't listen for set errors
        return ::android::hardware::Return<void>();
    }

  private:
    ::android::sp<vhal20::IVehicle> service_;
};
