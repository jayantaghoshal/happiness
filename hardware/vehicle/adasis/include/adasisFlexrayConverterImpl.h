/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once
#include <deque>
#include "../adasisFlexrayConverter.h"

#include <vhal_v2_0/VmsUtils.h>
#include "VmsAdasisConverter.h"
#include "adasis.pb.h"
#include "vms.pb.h"

class adasisFlexrayConverterImpl : public adasisFlexrayConverter {
    std::unique_ptr<::geo::vms::vms_adasis_converter::VmsAdasisConverter> converter;
    std::deque<::android::hardware::automotive::vehicle::V2_0::VehiclePropValue> values_queue_;
    bool open_result;
    bool layersSubscribed;
    bool layerAvailaible;
    bool messageSubscribe;

  public:
    adasisFlexrayConverterImpl();
    bool startConverterInstance();
    bool sendShortHorizonMessage() override;
    bool sendLongHorizonMessage() override;
    bool sendHorizonMetaData() override;
    bool sendStubMessage() override;
    bool sendSegmentMessage() override;
    bool sendPositionMessage() override;
    void stopConverterInstance();
    void subscribeToHAL();
    bool getConverterStarted();
    void vmsMessageController(const ::android::hardware::automotive::vehicle::V2_0::VehiclePropValue& value);

    void HandleVmsVehicleProperty(const ::android::hardware::automotive::vehicle::V2_0::VehiclePropValue& value);
    std::vector<std::unique_ptr<::android::hardware::automotive::vehicle::V2_0::VehiclePropValue>>
    StartVmsSubscription();
    ~adasisFlexrayConverterImpl();

#ifdef UNIT_TEST
    void initUnitTest(std::unique_ptr<::geo::vms::vms_adasis_converter::VmsAdasisConverter> mock_converter);
#endif
};
