/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include "ModuleBase.h"

#include "DesipClient.hpp"

#include <condition_variable>
#include <mutex>
#include <queue>
#include <string>
#include <thread>

namespace vhal20 = ::android::hardware::automotive::vehicle::V2_0;

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
 *  @brief Key Manager Desip Client class
 */
class KeyManagerModule : public DesipClient, public vhal20::impl::ModuleBase {
  public:
    KeyManagerModule(vhal20::impl::IVehicleHalImpl* vehicleHal);

    ~KeyManagerModule();

    /**
     * Initiates reader and worker threads
     */
    void init();

    std::vector<vhal20::VehiclePropConfig> listProperties() override;

  private:
    const std::vector<vhal20::VehiclePropConfig> keyboard_prop_config_;

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
    virtual void setRxMsgID(ParcelableDesipMessage* msg) override;

    void processKey(int8_t* data);
    void processKnob(int8_t* data);

    uint8_t HandleButtonStateRequest(int key_code, ButtonStateType req);

    /**
     *  @brief Power Modding Desip Client Listener class
     */
    class VIPListener final : public DesipClient::DesipClientListener {
      public:
        /**
         * Default constructor for VIP Listener
         * @param desip_client Instance of the Desip Client class to communicate with
         */
        VIPListener(void* desip_client);

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
        KeyManagerModule* key_manager_module_;
    };
};