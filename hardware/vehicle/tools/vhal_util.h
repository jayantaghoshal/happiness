/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once
#include <cstdio>
#include <cstdlib>
#include <map>
#include <string>
#include <vector>

#include <android/hardware/automotive/vehicle/2.0/IVehicle.h>
#include "vendor/volvocars/hardware/vehiclehal/1.0/types.h"

namespace vhal20 = ::android::hardware::automotive::vehicle::V2_0;
namespace vccvhal10 = ::vendor::volvocars::hardware::vehiclehal::V1_0;

namespace VhalUtil {

std::string GetPropertyName(int32_t prop);

std::string GetVehicleZoneNameByVehicleAreaAndAreaId(vhal20::VehicleArea vehicle_area, int32_t area_id);

void GetAndDumpPropertyValue(const ::android::sp<vhal20::IVehicle>& service,
                             const vhal20::VehiclePropValue& property_value, const std::string indentation);
void DumpPropertyValue(const vhal20::VehiclePropValue& property_value, std::string indentation = "");

void DumpProperties(const ::android::sp<vhal20::IVehicle>& service,
                    const ::android::hardware::hidl_vec<vhal20::VehiclePropConfig>& property_config_list);
}  // namespace VhalUtil