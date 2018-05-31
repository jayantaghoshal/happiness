/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include "ModuleBase.h"

#include "HisipClient.h"

#include <condition_variable>
#include <mutex>
#include <queue>
#include <string>
#include <thread>

#include "Application_dataelement.h"
#include "homebuttonmodule.h"
#include "i_vehicle_hal_impl.h"
#include "vendor/volvocars/hardware/vehiclehal/1.0/types.h"

namespace vhal20 = ::android::hardware::automotive::vehicle::V2_0;
namespace vccvhal10 = ::vendor::volvocars::hardware::vehiclehal::V1_0;

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
 *  @brief Key Manager Desip Client class
 */
class KeyManagerModule : public HisipClient,
                         public vhal20::impl::ModuleBase,
                         public vendor::volvocars::hardware::HomeButtonCallback {
  public:
    KeyManagerModule(vhal20::impl::IVehicleHalImpl* vehicleHal);

    ~KeyManagerModule();

    /**
     * Initiates reader and worker threads
     */
    void init();

    std::vector<vhal20::VehiclePropConfig> listProperties() override;
    std::unique_ptr<vhal20::VehiclePropValue> getProp(const vhal20::VehiclePropValue& requestedPropValue,
                                                      vhal20::impl::Status& /*status*/) override;

  private:
    vendor::volvocars::hardware::HomeButtonModule home_button_handler_;
    const std::vector<vhal20::VehiclePropConfig> keyboard_prop_config_;
    int homekeyjobid_ = 0;
    HomeButtonState homekeystate_ = HomeButtonState::kHomeButtonLongInactive;
    bool homelongpress_ = false;

    std::thread reader_thread_;

    std::set<uint16_t> altPressKeys;

    /**
     * The thread that spawns the Desip Client listener. Will populate a message
     * queue shared with the worker thread which will process it.
     */
    void VIPReader();

    /**
     * Processes the message incoming from VIP
     */
    void ProcessMessage(vip_msg& msg);

    /**
     * Send version request to VIP
     */
    void SendVersionRequestMsg();
    /**
     * Send version response to VIP
     */
    void SendVersionResponseMsg();
    /**
     * Send a message to VIP
     * @param[in] fid  Function ID
     * @param[in] data Pointer towards the data
     * @param[in] size Data Size
     */
    void SendToVIP(uint8_t fid, int8_t data[], uint32_t size);

    /**
     * Set the received message's application ID
     * @param[in] msg The received message
     */
    virtual std::vector<uint8_t> getApplicationId() override;

    void processKey(int8_t* data);
    void processKnob(int8_t* data);

    /**
     * Function for handling button presses and push values to HW_KEY_INPUT.
     */
    uint8_t HandleButtonStateRequest(int key_code, ButtonStateType req);

    /**
     * Function for getting HomeButtonState as VehiclePropValue
     * with all the appropriate fields.
     */
    vhal20::VehiclePropValue convertToLongpressPropValue(HomeButtonState homekeystate);

    void HomeButtonPressed(bool pressed) override;

    /**
     *  @brief Power Modding Desip Client Listener class
     */
    class VIPListener final : public HisipClient::HisipClientListener {
      public:
        /**
         * Default constructor for VIP Listener
         * @param desip_client Instance of the Desip Client class to communicate with
         */
        VIPListener(void* hisip_client);

        /**
         * Default destructor of the VIP Listener class
         */
        virtual ~VIPListener() = default;

        /**
         * Callback to handle incoming messages from the VIP
         * @param  msg          Message coming from the vip
         * @return              status
         */
        virtual bool onMessageFromVip(const HisipMessage& msg) override;

        /**
         * Get the Application Name
         * @return application name
         */
        virtual std::string getUserId() override;

      private:
        // The instance of the Desip Client to communicate with
        KeyManagerModule* key_manager_module_;
    };

    /**
     * @brief Key event handler for autonomous drive
     */
    class AutonomousDriveHandler {
      public:
        AutonomousDriveHandler();
        bool shouldIgnoreKey(const vip_msg& msg);

        enum class AutonomousDriveStatus { kManual, kAuto };
        enum class VehicleOperatorMode { kNoVO, kVO };

      private:
        // TODO change to correct signal AutmnsDrvModeSts1
        // Also change the check in constructor
        ApplicationDataElement::DEReceiver<autosar::DoorDrvrSts_info> aut_drive_status_receiver_;
        // TODO change to correct signal VehOperModSts1
        ApplicationDataElement::DEReceiver<autosar::DoorPassSts_info> veh_oper_mod_receiver_;

        AutonomousDriveStatus autonomous_drive_status_ = AutonomousDriveStatus::kManual;
        VehicleOperatorMode vehicle_operator_mode_ = VehicleOperatorMode::kNoVO;

        bool autonomous_drive_ = false;
    } autonomous_drive_handler_;
};
