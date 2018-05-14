/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <vhal_v2_0/VehicleUtils.h>
#include <functional>
#include <map>
#include "IDispatcher.h"
#include "ModuleBase.h"
#include "i_vehicle_hal_impl.h"
#include "notifiable_property.h"
#include "vendor/volvocars/hardware/vehiclehal/1.0/types.h"

namespace vhal20 = ::android::hardware::automotive::vehicle::V2_0;
namespace vccvhal10 = ::vendor::volvocars::hardware::vehiclehal::V1_0;

template <typename T>
class VhalPropertyHandler : public vhal20::impl::ModuleBase {
  public:
    VhalPropertyHandler(vhal20::VehiclePropConfig config,
                        std::shared_ptr<tarmac::eventloop::IDispatcher> dispatcher,
                        vhal20::impl::IVehicleHalImpl* vehicleHal)
        : vhal20::impl::ModuleBase{vehicleHal}, config_{config}, dispatcher_{dispatcher} {
        if (IsGlobal()) {
            vhal20::VehiclePropValue property_value{};
            property_value.areaId = 0;
            property_value.prop = config.prop;
            values_.emplace(property_value.areaId, property_value);
        } else {
            for (const auto& area : config.areaConfigs) {
                vhal20::VehiclePropValue property_value{};
                property_value.areaId = area.areaId;
                property_value.prop = config.prop;
                values_.emplace(area.areaId, property_value);
            }
        }
    }
    std::unique_ptr<vhal20::VehiclePropValue> getProp(const vhal20::VehiclePropValue& requestedPropValue,
                                                      vhal20::impl::Status& status) override {
        if (requestedPropValue.prop != config_.prop) {
            status = vhal20::impl::Status::INVALID_PARAM_ERROR;
            return nullptr;
        }

        const auto requestedAreaId = IsGlobal() ? 0 : requestedPropValue.areaId;
        auto it = values_.find(requestedAreaId);

        if (it == values_.end()) {
            status = vhal20::impl::Status::INVALID_PARAM_ERROR;
            return nullptr;
        }

        status = vhal20::impl::Status::SUCCESS;
        return std::make_unique<vhal20::VehiclePropValue>(it->second);
    }

    std::vector<vhal20::VehiclePropValue> getAllPropValues() override {
        std::vector<vhal20::VehiclePropValue> all_prop_values;
        for (auto& v : values_) {
            all_prop_values.push_back(v.second);
        }
        return all_prop_values;
    }

    std::vector<vhal20::VehiclePropConfig> listProperties() override { return {config_}; };
    int setProp(const vhal20::VehiclePropValue& propValue) override;

    void init() override {
        vhal20::impl::ModuleBase::init();
        registeredWithVhal = true;
    }

    void PushProp(T value, vhal20::VehiclePropertyStatus status, int32_t zone = 0);
    void subscribe_set_prop(std::function<void(T value, int32_t zone)> func) { request_set_prop_ = func; };

  protected:
    const vhal20::VehiclePropConfig config_;
    std::map<int, vhal20::VehiclePropValue> values_;
    std::function<void(T value, int32_t zone)> request_set_prop_;
    std::shared_ptr<tarmac::eventloop::IDispatcher> dispatcher_;
    bool registeredWithVhal = false;

    bool IsGlobal() {
        auto vehicle_area = static_cast<vhal20::VehicleArea>(config_.prop & vhal20::VehicleArea::MASK);
        return (vehicle_area == vhal20::VehicleArea::GLOBAL);
    }
};

vhal20::VehiclePropConfig BoolConfig(vccvhal10::VehicleProperty property);
