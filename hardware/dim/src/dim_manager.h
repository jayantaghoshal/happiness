/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#ifndef DIMSERVICE_H
#define DIMSERVICE_H

#include <vendor/volvocars/hardware/vehiclecom/1.0/IVehicleCom.h>
#include "dim_tbt_area.h"
using ::vendor::volvocars::hardware::vehiclecom::V1_0::IVehicleCom;
using ::android::sp;

namespace vcc {
namespace dim {

class DimManager final : public ::android::hidl::manager::V1_0::IServiceNotification,
                         public virtual ::android::RefBase,
                         private ::android::hardware::hidl_death_recipient {
  public:
    void Start();
    bool SetDimVideoStreamMode(DimTbtArea::TypeActivationIdentifier id);

    // hidl_death_recipient
    void serviceDied(uint64_t cookie, const ::android::wp<IBase>& who) override;

  private:
    bool SetVehicleCom();

    void ExitDimManager();

    // Methods from IServiceNotification follow
    ::android::hardware::Return<void> onRegistration(const ::android::hardware::hidl_string& fqName,
                                                     const ::android::hardware::hidl_string& name,
                                                     bool preexisting) override;

    sp<IVehicleCom> vehicle_com_server_;
    std::uint8_t sequence_id_ = 0;

    const std::string service_name_ = "dim";
};

}  // namespace dim
}  // namespace vcc

#endif
