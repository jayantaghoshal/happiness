/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "systeminformationmodule.h"
#include <utils/SystemClock.h>
#include <vhal_v2_0/VehicleUtils.h>

#include <cutils/log.h>

#undef LOG_TAG
#define LOG_TAG "SystemInformationModule"

using namespace android::hardware::automotive::vehicle::V2_0;

SystemInformationModule::SystemInformationModule(vhal20::impl::IVehicleHalImpl* vehicle_hal)
    : impl::ModuleBase(vehicle_hal) {
    StartSubscribe();
}

void SystemInformationModule::StartSubscribe() {
    vin_info_receiver_.subscribe([&]() {
        const DataElemValue<autosar::Vin_info>& vin_info = vin_info_receiver_.get();
        onVin1BlockReceived(vin_info);
    });
}

std::string SystemInformationModule::asStdString(const autosar::Vin1& block1,
                                                 const autosar::Vin1& block2,
                                                 const autosar::Vin1& block3) {
    std::stringstream s;
    s << block1.VINSignalPos1 << block1.VINSignalPos2 << block1.VINSignalPos3 << block1.VINSignalPos4
      << block1.VINSignalPos5 << block1.VINSignalPos6 << block1.VINSignalPos7 << block2.VINSignalPos1
      << block2.VINSignalPos2 << block2.VINSignalPos3 << block2.VINSignalPos4 << block2.VINSignalPos5
      << block2.VINSignalPos6 << block2.VINSignalPos7 << block3.VINSignalPos1 << block3.VINSignalPos2
      << block3.VINSignalPos3;

    return s.str();
}

bool SystemInformationModule::hasByteErrors(const autosar::Vin1& block) {
    // Vin1/MAIN;4: A faulty block is indicated using 255
    // In block 3 only three bytes are used.
    bool ret = false;
    if (block.BlockNr != 0x03) {
        ret = block.VINSignalPos1 == 0xFF || block.VINSignalPos2 == 0xFF || block.VINSignalPos3 == 0xFF ||
              block.VINSignalPos4 == 0xFF || block.VINSignalPos5 == 0xFF || block.VINSignalPos6 == 0xFF ||
              block.VINSignalPos7 == 0xFF;
    } else {
        ret = block.VINSignalPos1 == 0xFF || block.VINSignalPos2 == 0xFF || block.VINSignalPos3 == 0xFF;
    }

    return ret;
}

bool SystemInformationModule::hasNonPrintableCharacters(const autosar::Vin1& block) {
    bool ret = false;
    if (block.BlockNr != 0x03) {
        ret = std::iscntrl(block.VINSignalPos1) || std::iscntrl(block.VINSignalPos2) ||
              std::iscntrl(block.VINSignalPos3) || std::iscntrl(block.VINSignalPos4) ||
              std::iscntrl(block.VINSignalPos5) || std::iscntrl(block.VINSignalPos6) ||
              std::iscntrl(block.VINSignalPos7);
    } else {
        ret = std::iscntrl(block.VINSignalPos1) || std::iscntrl(block.VINSignalPos2) ||
              std::iscntrl(block.VINSignalPos3);
    }

    return ret;
}

void SystemInformationModule::onVin1BlockReceived(const DataElemValue<Vin_info>& vin_info) {
    if (m_block1Received && m_block2Received && m_block3Received) {
        return;
    }

    if (vin_info.isError()) {
        ALOGW("Vehicle Identification Number: Received erronous dataelement. Ignoring.");
        return;
    }

    autosar::Vin1 vinBlock = vin_info.value();
    if (hasByteErrors(vinBlock)) {
        ALOGW("Vehicle Identification Number: Received block that contained errors. Ignoring.");
        return;
    }

    if (hasNonPrintableCharacters(vinBlock)) {
        ALOGW("Vehicle Identification Number: Received block contains non-printable characters. Ignoring.");
        return;
    }

    if (vinBlock.BlockNr == 0x01) {
        m_block1 = vinBlock;
        m_block1Received = true;
    } else if (vinBlock.BlockNr == 0x02) {
        m_block2 = vinBlock;
        m_block2Received = true;
    } else if (vinBlock.BlockNr == 0x03) {
        m_block3 = vinBlock;
        m_block3Received = true;
    } else {
        ALOGW("Vehicle Identification Number: Received block with invalid identifier: %i. Block ignored.",
              vinBlock.BlockNr);
        return;
    }

    if (m_block1Received && m_block2Received && m_block3Received) {
        m_vin = asStdString(m_block1, m_block2, m_block3);
        VehiclePropValue prop_value;

        prop_value.timestamp = android::elapsedRealtimeNano();
        prop_value.areaId = 0;

        prop_value.prop = toInt(VehicleProperty::INFO_VIN);
        prop_value.value.stringValue = m_vin;

        pushProp(prop_value);
        ALOGI("Vehicle Identification Number: All blocks has been successfully received. Vin = %s", m_vin.c_str());
    }
}

std::vector<VehiclePropConfig> SystemInformationModule::listProperties() {
    std::vector<VehiclePropConfig> prop_configs;
    {
        VehiclePropConfig prop_config;

        prop_config.prop = toInt(VehicleProperty::INFO_VIN);
        prop_config.access = VehiclePropertyAccess::READ;
        prop_config.changeMode = vhal20::VehiclePropertyChangeMode::STATIC;
        prop_configs.push_back(prop_config);
    }

    return prop_configs;
}
