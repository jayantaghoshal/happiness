/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

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
    VhalPropertyHandler(vhal20::VehiclePropConfig config, T default_value,
                        std::shared_ptr<tarmac::eventloop::IDispatcher> dispatcher,
                        vhal20::impl::IVehicleHalImpl* vehicleHal)
        : vhal20::impl::ModuleBase{vehicleHal}, config_{config}, dispatcher_{dispatcher} {
        value_.prop = config.prop;
        value_.areaId = config.areaConfigs[0].areaId;
        (void)default_value;  // TODO: Put default_value into value_
    }

    int setProp(const vhal20::VehiclePropValue& propValue) override;
    std::unique_ptr<vhal20::VehiclePropValue> getProp(const vhal20::VehiclePropValue& requestedPropValue,
                                                      vhal20::impl::Status& status) {
        if (requestedPropValue.prop != config_.prop) {
            status = vhal20::impl::Status::INVALID_PARAM_ERROR;
            return nullptr;
        }
        status = vhal20::impl::Status::SUCCESS;
        return std::make_unique<vhal20::VehiclePropValue>(value_);
    }
    std::vector<vhal20::VehiclePropValue> getAllPropValues() override { return {value_}; }
    std::vector<vhal20::VehiclePropConfig> listProperties() override { return {config_}; };

    void subscribe_set_prop(std::function<void(T)> func) { request_set_prop_ = func; };
    void myPushProp(T value);

  protected:
    const vhal20::VehiclePropConfig config_;
    vhal20::VehiclePropValue value_;
    std::function<void(T)> request_set_prop_;
    std::shared_ptr<tarmac::eventloop::IDispatcher> dispatcher_;
};

template <typename T>
class MultiZonePropertyHandler : public vhal20::impl::ModuleBase {
  public:
    MultiZonePropertyHandler(vhal20::VehiclePropConfig config,
                             std::shared_ptr<tarmac::eventloop::IDispatcher> dispatcher,
                             vhal20::impl::IVehicleHalImpl* vehicleHal)
        : vhal20::impl::ModuleBase{vehicleHal}, config_{config}, dispatcher_{dispatcher} {
        for (int32_t i = 0; i < 32; i++) {
            int32_t mask = (1 << i);
            if ((mask & config.supportedAreas) != 0) {
                vhal20::VehiclePropValue v{};
                v.areaId = mask;
                v.prop = config.prop;
                values_.emplace(mask, v);
            }
        }
    }

    int setProp(const vhal20::VehiclePropValue& propValue) override;
    std::unique_ptr<vhal20::VehiclePropValue> getProp(const vhal20::VehiclePropValue& requestedPropValue,
                                                      vhal20::impl::Status& status) {
        if (requestedPropValue.prop != config_.prop) {
            status = vhal20::impl::Status::INVALID_PARAM_ERROR;
            return nullptr;
        }

        auto it = values_.find(requestedPropValue.areaId);
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

    void subscribe_set_prop(std::function<void(T value, int32_t zone)> func) { request_set_prop_ = func; };
    void myPushProp(T value, int32_t zone);

  protected:
    const vhal20::VehiclePropConfig config_;
    std::map<int, vhal20::VehiclePropValue> values_;
    std::function<void(T value, int32_t zone)> request_set_prop_;
    std::shared_ptr<tarmac::eventloop::IDispatcher> dispatcher_;
};
