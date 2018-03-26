/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <IDispatcher.h>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include "DesipClient.hpp"
#include "ModuleBase.h"
#include "i_vehicle_hal_impl.h"
#include "vendor/volvocars/hardware/vehiclehal/1.0/types.h"

namespace vendor {
namespace volvocars {
namespace hardware {
namespace vehiclehal {
namespace V1_0 {

namespace impl {
namespace vhal20 = ::android::hardware::automotive::vehicle::V2_0;
namespace vccvhal10 = ::vendor::volvocars::hardware::vehiclehal::V1_0;

/**
 *  @brief Key Manager Desip Client class
 */
class HomeButtonModule : public DesipClient, public vhal20::impl::ModuleBase {
  public:
    HomeButtonModule(vhal20::impl::IVehicleHalImpl* vehicleHal);

    ~HomeButtonModule();
    /**
    * Simple structure to contain messages coming from VIP
    */
    typedef struct {
        uint8_t fid;
        uint8_t data_length;
        int8_t data[20];
    } vip_msg;
    /**
    * Home button state types commanded by VIP
    */
    enum class ButtonStateType { kButtonPressed = 0, kButtonReleased };
    /**
     * Home button longpress state
     */
    enum class HomeButtonState { kHomeButtonLongInactive = 0, kHomeButtonLongActive };

    /**
     * Initiates reader and worker threads
     */
    void init();
    /**
    * Android Vehicle hal functions to follow
    */
    std::vector<vhal20::VehiclePropConfig> listProperties() override;
    std::unique_ptr<vhal20::VehiclePropValue> getProp(const vhal20::VehiclePropValue& requestedPropValue,
                                                      vhal20::impl::Status& /*status*/) override;

  private:
    const vhal20::VehiclePropConfig keyboard_prop_config_;
    const vhal20::VehiclePropConfig homekeylongpress_prop_config_;
    int homekeyjobid_ = 0;
    HomeButtonState homekeystate_ = HomeButtonState::kHomeButtonLongInactive;
    bool homelongpress_ = false;

    std::thread reader_thread_;
    /**
     * The thread that spawns the Desip Client listener. Will populate a message
     * queue shared with the worker thread which will process it.
     */
    void VIPReader();

    /**
     * Processes the message incoming from VIP
     */
    void processMessage(vip_msg& msg);

    /**
     * Set the received message's application ID
     * @param[in] msg The received message
     */
    virtual void setRxMsgID(ParcelableDesipMessage* msg) override;

    /**
     * Function for handling button presses and push values to HW_KEY_INPUT.
     */
    uint8_t handleButtonStateRequest(int key_code, ButtonStateType req);
    /**
    * Function for getting HomeButtonState as VehiclePropValue
    * with all the appropriate fields.
    */
    vhal20::VehiclePropValue convertToPropValue(HomeButtonState homekeystate);
    /**
     *  @brief Power Modding Desip Client Listener class
     */
    class VIPListener final : public DesipClient::DesipClientListener {
      public:
        /**
         * Default constructor for VIP Listener
         * @param homebuttonModule Instance of the Desip Client class to communicate with.
         */
        VIPListener(HomeButtonModule* homebuttonModule);

        /**
         * Default destructor of the VIP Listener class
         */
        virtual ~VIPListener() = default;

        /**
         * Callback to handle incoming messages from the VIP
         * @param  msg          Message coming from the vip
         * @param  _aidl_return error code
         * @return              status
         */
        virtual android::binder::Status deliverMessage(const ParcelableDesipMessage& msg, bool* _aidl_return) override;

        /**
         * Get the Application Name
         * @return application name
         */
        virtual String16 getId() override;

      private:
        // The instance of the Desip Client to communicate with
        HomeButtonModule* home_button_module_;
    };
};

}  // namespace impl

}  // namespace V1_0
}  // namespace vehiclehal
}  // namespace hardware
}  // namespace volvocars
}  // namespace vendor
