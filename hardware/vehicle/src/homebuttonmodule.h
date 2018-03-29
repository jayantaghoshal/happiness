/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <condition_variable>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include "DesipClient.hpp"

namespace vendor {
namespace volvocars {
namespace hardware {

class HomeButtonCallback {
  public:
    virtual ~HomeButtonCallback() = default;
    virtual void HomeButtonPressed(bool pressed) = 0;
};

/**
 *  @brief Key Manager Desip Client class
 */
class HomeButtonModule : public DesipClient {
  public:
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
     * Initiates reader and worker threads
     */
    void init(HomeButtonCallback* listener);

  private:
    HomeButtonCallback* home_button_listener_;

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

}  // namespace hardware
}  // namespace volvocars
}  // namespace vendor
