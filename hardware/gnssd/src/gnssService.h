/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <IDispatcher.h>

extern "C" {
#include <pl/asn_base/asn_base.h>
}

#include <vendor/volvocars/hardware/vehiclecom/1.0/IMessageCallback.h>
#include <vendor/volvocars/hardware/vehiclecom/1.0/IVehicleCom.h>
#include "binderimpl/gnss/Gnss.h"

using ::android::hidl::base::V1_0::DebugInfo;
using ::android::hidl::base::V1_0::IBase;
using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::RefBase;
using ::android::sp;
using ::android::wp;
using ::tarmac::eventloop::IDispatcher;

using ::vendor::volvocars::hardware::vehiclecom::V1_0::IVehicleCom;
using ::vendor::volvocars::hardware::vehiclecom::V1_0::IMessageCallback;
using ::vendor::volvocars::hardware::vehiclecom::V1_0::Msg;

using ::android::hardware::hidl_death_recipient;

using namespace android::hardware::gnss::V1_0::implementation;
using namespace android::hardware::gnss::V1_0;

// Only refcounting and public methods should be visible externally
class GnssService : public virtual RefBase, private IMessageCallback, private hidl_death_recipient {
  public:
    GnssService();
    ~GnssService() = default;

    void StartSubscribe();

    /**
       * Initialize IpLmService and the internal Link Manager functional block.
       */
    bool Initialize();

    void GNSSPositionDataNotificationHandler(const Msg &msg);
    void GNSSPositionDataAccuracyNotificationHandler(const Msg &msg);

  private:
    // Methods from ::vendor::volvocars::hardware::vehiclecom::V1_0::IMessageCallback follow.
    Return<void> onMessageRcvd(const Msg &msg) override;

    // Methods from hidl_death_recipient follow.
    void serviceDied(uint64_t cookie, const wp<IBase> &who) override;
    void unsubscribeAll();
    sp<IVehicleCom> ipcbServer_;

    tarmac::eventloop::IDispatcher &timeProvider_;

    Gnss gnss_;
    GnssLocation location_;
    bool expect_location_accuracy_;

    // Subscription Id's
    uint64_t pos_notification_id;
    uint64_t pos_cyclic_id;
    uint64_t acc_notification_id;
    uint64_t acc_cyclic_id;

    ASN_BYTE m_session_buffer_msgd[ASN_SESSION_SIZE + 2048];
    ASN_Session m_session_msgd;
    bool connectionError = false;
};
