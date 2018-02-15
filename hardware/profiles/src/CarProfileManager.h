/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <mutex>

#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>
#include <vendor/volvocars/hardware/profiles/1.0/ICarProfileManager.h>
#include <map>
#include <queue>
#include <string>
#include "libsettings/setting.h"

#include "Application_dataelement.h"
#include "timer_manager_interface.h"

namespace vendor {
namespace volvocars {
namespace hardware {
namespace profiles {
namespace V1_0 {
namespace implementation {

using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::sp;
using namespace tarmac::timeprovider;
using namespace SettingsFramework;

class CarProfileManager : public ICarProfileManager, public ::android::hardware::hidl_death_recipient {
  public:
    CarProfileManager(std::shared_ptr<TimerManagerInterface> time_provider,
                      const android::sp<SettingsManager>& manager);
    Return<void> subscribeUserChange(const sp<IProfileChangedHandler>& cb) override;
    Return<void> requestSwitchUser(const hidl_string& androidUserId) override;
    Return<KeyId> getUserProfileInformation(const hidl_string& androidUserId) override;
    Return<int16_t> getNrOfUnusedProfiles() override;
    Return<bool> pairAndroidUserToUnusedVehicleProfile(const hidl_string& androidUserId) override;
    Return<void> deleteUser(const hidl_string& androidUserId) override;
    Return<void> pairCurrentProfileToKey(KeyLocation searchLocation,
                                         const sp<IKeyPairResponseHandler>& onKeySearchCompleted) override;
    Return<bool> disconnectCurrentProfileFromKey() override;

    void serviceDied(uint64_t cookie, const android::wp<::android::hidl::base::V1_0::IBase>& who) override;

  private:
    void disconnectProfileFromKey(ProfileIdentifier profile);

    const std::string UNUSED_ANDROID_USER;

    ApplicationDataElement::DESender<autosar::ProfChg_info> prof_chg_sender_;
    ApplicationDataElement::DEReceiver<autosar::ProfPenSts1_info> prof_pen_sts1_receiver_;
    ApplicationDataElement::DESender<autosar::KeyReadReqFromSetgMgr_info> key_read_req_sender_;
    ApplicationDataElement::DEReceiver<autosar::KeyReadStsToProfCtrl_info> key_read_sts_receiver_;
    ApplicationDataElement::DEReceiver<autosar::NrOfKeyAvl_info> nr_of_key_receiver_;
    ApplicationDataElement::DESender<autosar::KeyProfMpgUpd_info> key_prof_mgr_upd_sender_;

    std::shared_ptr<TimerManagerInterface> time_provider_;
    std::unique_ptr<TimerSubscriptionHandle> prof_chg_timer_handle_;
    std::unique_ptr<TimerSubscriptionHandle> pair_key_timer_handle_;
    std::unique_ptr<TimerSubscriptionHandle> pair_key_timeout_handle_;
    std::unique_ptr<TimerSubscriptionHandle> unpair_key_timer_handle_;

    Setting<std::string, UserScope::USER> s_android_user_;
    Setting<autosar::IdPen, UserScope::NOT_USER_RELATED> s_current_profile_;
    Setting<KeyId, UserScope::USER> s_profile_info_;

    std::map<std::string, autosar::IdPen> android_user_to_profile_;
    std::queue<autosar::IdPen> unused_profiles_;

    ProfileIdentifier current_profile_;
    std::list<sp<IProfileChangedHandler>> profile_changed_listeners_;
    std::mutex callback_lock_;
    std::mutex profile_storage_lock_;

    enum class State { IDLE, KEY_SEARCH_ACTIVE };
    State state_;
};

}  // namespace implementation
}  // namespace V1_0
}  // namespace profiles
}  // namespace hardware
}  // namespace volvocars
}  // namespace vendor
