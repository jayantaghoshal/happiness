/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <vhal_v2_0/VehicleUtils.h>
#include <vhal_v2_0/VmsUtils.h>
#include <functional>
#include <memory>

#include <cutils/log.h>

#include "vmsmodule.h"

#undef LOG_TAG
#define LOG_TAG "vmsModule"

using namespace android::hardware::automotive::vehicle::V2_0;
using ::android::hardware::automotive::vehicle::V2_0::vms::createSubscribeMessage;
using ::android::hardware::automotive::vehicle::V2_0::vms::VmsLayer;
using ::android::hardware::automotive::vehicle::V2_0::vms::isValidVmsMessage;
using ::android::hardware::automotive::vehicle::V2_0::vms::parseMessageType;
using ::android::hardware::automotive::vehicle::V2_0::vms::parseData;
using ::geo::vms::v1::VmsPacket;
// using ::geo::vms::v1::VmsLayerTypeAndVersion;

// using namespace autosar;
using namespace android;

vmsHal::vmsHal(vhal20::impl::IVehicleHalImpl* vehicleHal)
    : vhal20::impl::ModuleBase(vehicleHal),
      vehicleMessageService{{
              .prop = toInt(vhal20::VehicleProperty::VEHICLE_MAP_SERVICE),
              .access = vhal20::VehiclePropertyAccess::READ_WRITE,
              .changeMode = vhal20::VehiclePropertyChangeMode::ON_CHANGE,

      }} {
    // StartFlexraySubscribers();
    ALOGI("Adasis message: in main");
    googleAdasisConverter = new adasisFlexrayConverterImpl();

    _flagLayerNotSet = true;
    timerStarted = false;
}

int vmsHal::setProp(const VehiclePropValue& propValue) {
    ALOGD("setProp: 0x%0x", propValue.prop);
    ALOGI("Vms Message type: %d", propValue.value.int32Values[0]);

    if (_flagLayerNotSet) {
        int status;
        /* std::unique_ptr<VehiclePropValue> subscribe_message;
         subscribe_message=createSubscribeMessage(VmsLayer(3, 0, 1));

         status= pushProp(*(subscribe_message));
         ALOGI("VMS Subscribed Property 0x%0x, to layer %d, With status %d",subscribe_message->prop,3,status);

         subscribe_message=createSubscribeMessage(VmsLayer(10, 0, 1));

         status=pushProp(*(subscribe_message));
         ALOGI("VMS Subscribed Property 0x%0x, to layer %d, With status %d",subscribe_message->prop,10,status);
         subscribe_message=createSubscribeMessage(VmsLayer(4, 0, 1));

         status=pushProp(*(subscribe_message));
         ALOGI("VMS Subscribed Property 0x%0x, to layer %d, With status %d",subscribe_message->prop,4,status);
         subscribe_message=createSubscribeMessage(VmsLayer(5, 0, 1));

         status=pushProp(*(subscribe_message));
         ALOGI("VMS Subscribed Property 0x%0x, to layer %d, With status %d",subscribe_message->prop,5,status);
         subscribe_message=createSubscribeMessage(VmsLayer(6, 0, 1));

         status=pushProp(*(subscribe_message));
         ALOGI("VMS Subscribed Property 0x%0x, to layer %d, With status %d",subscribe_message->prop,6,status);
         subscribe_message=createSubscribeMessage(VmsLayer(8, 0, 1));

         status=pushProp(*(subscribe_message));
         ALOGI("VMS Subscribed Property 0x%0x, to layer %d, With status %d",subscribe_message->prop,8,status);
         subscribe_message=createSubscribeMessage(VmsLayer(2, 0, 1));

         status=pushProp(*(subscribe_message));
         ALOGI("VMS Subscribed Property 0x%0x, to layer %d, With status %d",subscribe_message->prop,2,status);*/

        for (const auto& vmsProperty : googleAdasisConverter->StartVmsSubscription()) {
            status = pushProp(*vmsProperty);
        }
        _flagLayerNotSet = false;
    } else {
        VmsPacket vms_packet;
        vms_packet.ParseFromString(parseData(propValue));
        ALOGI("Pranay vms packet: %s", vms_packet.ShortDebugString().c_str());
        if (isValidVmsMessage(propValue) && parseMessageType(propValue) == VmsMessageType::DATA) {
            googleAdasisConverter->HandleVmsVehicleProperty(propValue);
            if (timerStarted == false) {
                googleMessageTimer = std::make_unique<timerGoogleImpl>(googleAdasisConverter);
                timerStarted = true;
            }
        }
    }
    return 0;
}

std::unique_ptr<VehiclePropValue> vmsHal::getProp(const VehiclePropValue& requestedPropValue, vhal20::impl::Status&) {
    ALOGV("getProp: 0x%0x", requestedPropValue.prop);

    return std::make_unique<VehiclePropValue>(requestedPropValue);
}

std::vector<VehiclePropConfig> vmsHal::listProperties() {
    VehiclePropConfig vms;
    vms.prop = toInt(vhal20::VehicleProperty::VEHICLE_MAP_SERVICE);
    vms.access = vhal20::VehiclePropertyAccess::READ_WRITE;
    vms.changeMode = vhal20::VehiclePropertyChangeMode::ON_CHANGE;
    vms.areaConfigs.resize(1);
    vms.areaConfigs[0].areaId = 0;
    vms.areaConfigs[0].minInt32Value = 0;    // 0%
    vms.areaConfigs[0].maxInt32Value = 100;  // 100%
    vms.configFlags = 0;
    ALOGI("In List Properties");
    return {vms};
}

void vmsHal::sendPositionMessage() { googleAdasisConverter->sendPositionMessage(); }

void vmsHal::sendShortHorizonMessage() { googleAdasisConverter->sendShortHorizonMessage(); }

void vmsHal::sendLongHorizonMessage() { googleAdasisConverter->sendLongHorizonMessage(); }

void vmsHal::sendHorizonMetaData() { googleAdasisConverter->sendHorizonMetaData(); }

void vmsHal::sendStubMessage() { googleAdasisConverter->sendStubMessage(); }

void vmsHal::sendSegmentMessage() { googleAdasisConverter->sendSegmentMessage(); }
