#pragma once

#include "Application_dataelement.h"
#include "ModuleBase.h"
#include "android/hardware/automotive/vehicle/2.0/types.h"

namespace vhal20 = ::android::hardware::automotive::vehicle::V2_0;

using namespace ApplicationDataElement;
using namespace autosar;

class KeyManagerModule : public vhal20::impl::ModuleBase {
  public:
    KeyManagerModule(vhal20::impl::IVehicleHalImpl* vehicle_hal);

    void StartSubscribe();

    uint8_t HandleButtonStateRequest(int key_code, int req);

    std::vector<vhal20::VehiclePropConfig> listProperties() override;

  private:
    DEReceiver<BtnAudFbSts_info> key_ptt_receiver_;
    DEReceiver<BtnAudVolSts_info> key_vol_up_down_receiver_;
    DEReceiver<RotyDirReq1_info> roty_vol_dir_receiver_;
    DEReceiver<RotyPosReq1_info> roty_vol_pos_receiver_;

    BtnAudVolSts last_vol_btn_state_ = BtnAudVolSts::NoBtnPsd;
    RotyDirUI1 volume_roty_dir_ = RotyDirUI1::Idle;
};