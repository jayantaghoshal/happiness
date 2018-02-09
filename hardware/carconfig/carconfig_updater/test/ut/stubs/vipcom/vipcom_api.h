/*
 * Copyright 2017 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <cstdint>
#include <thread>
#include <vector>

/**
 * This API provides a class that when instantiated creates a communication
 * channel to the Vehicle Data Driver.
 * A client application can register a callback method when constructing an object
 * so that incoming Hisip messages can be received.
 * There is also a method for sending Hisip messages.
 */

/* START EXAMPLE

#include <vipcom_api.h>

// An application should implement the IVipComClient interface
//
class MyApplication : public IVipComClient
{
    virtual void onMessage(const uint8_t &_fid, const std::vector<uint8_t> &_payload)
    {
        log_debug() << "I got a Hisip Message with fid:" << _fid;
    }
    virtual void onConnectionError(ConnectionError e)
    {
        if (IVipComClient::ConnectionError::CONNECT_FAILED == e)
        {
            log_debug() << "I got an critical error when creating a VipCom object!";
            log_debug() << "therefor I should delete the object and try creating one again, or exit my application!";
            log_debug() << "but I shouldnt delete the object from this thread; because it is owned by the object...";
        }
    }
}

int main(int argc, char *argv[])
{
    auto myApp = new MyApplication();
    auto myVipCom = new VipCom(VipCom::Application::powermodeIpcIndex, myApp);

    const uint8_t GetHisipVersion_fid = 0x00; // First message needs to be the version request message
    const std::vector<uint8_t> msg{};         // Version request message has no payload
    auto ret myVipCom->sendMessage(GetHisipVersion_fid, msg);
    if (-1 == ret)
    {
        log_error() << "Sending version request message didn't go well";
        return EXIT_FAILURE;
    }

    // Wait here until your application receives a version response message from the VIP
    // It will arrive in the onMessage function
    waitForVersionResponseMessage();

    // Validate the version in the version response message
    if(-1 == validateHisipVersion())
    {
        log_error() << "HISIP version check didn't go well";
        return EXIT_FAILURE;
    }
    else
    {
        // Wait here until your application receives a version request message from the VIP
        // It will arrive in the onMessage callback function
        waitForVersionRequestMessage();

        // Send version response message to the VIP
        const uint8_t RespHisipVersion_fid = 0x01;  // Second message sent needs to be the version response message
        const std::vector<uint8_t> msg2{0x01, 0x00}; // Version response message has the version as payload (e.g. 1.00)
        auto ret myVipCom->sendMessage(RespHisipVersion_fid, msg2);
        if (-1 == ret)
        {
            log_error() << "Sending version response message didn't go well";
            return EXIT_FAILURE;
        }
    }

    // Sending other messages to the VIP goes here

    return 0;
}


END EXAMPLE */

/**
 * @brief Interface a client should implement in order to get asynchronous requests/events from the vehicle data driver
 * Note: Callbacks will execute in a different thread than the clients regular thread.
 */
class IVipComClient {
  public:
    enum class ConnectionError {
        OK,
        CONNECT_FAILED,  // Failed making initial connection. Not possible to recover.
        CONNECTION_LOST  // Problems with existing connection. Not possible to recover.
    };

    virtual ~IVipComClient() {}
    virtual void onMessage(const uint8_t& _fid, const std::vector<uint8_t>& _payload) = 0;
    virtual void onConnectionError(ConnectionError e) = 0;
};

class VipCom {
  public:
    // @brief return values for getStatus()
    enum class Status { INITIALIZING, OK, ERROR };

    // @brief Values for specifying type of VDD communication when creating a VipCom object.
    enum class Application {
        amfmIpcIndex = 0,
        diagnosticsIpcIndex,
        earlyAudioIpcIndex,
        earlyGearIpcIndex,
        keypanelIpcIndex,
        powermodeIpcIndex,
        vsmIpcIndex,
        positionIpcIndex,
        systemIpcIndex,
        swdlIpcIndex
    };

    VipCom() = delete;
    /**
     * @brief Create a VipCom object that communicates over the channel identified by app id.
     * @param appIndex selects which connection type that shall be established
     * @param Use this to hook up callbacks to your app's IVipComClient implementation. Or nullptr.
     */
    VipCom(Application appIndex, IVipComClient* vipcom_client);
    ~VipCom();

    /**
     * @brief Send a message to the VIP
     * @param _fid
     * @param _payload
     * @return -1 if not successful (this could mean that the connection to VDD is down.)
     */
    int sendMessage(const uint8_t& _fid, const std::vector<uint8_t>& _payload);

    /**
     * @brief Get status of VipCom object. If there is a connection problem the status will indicate NOK
     * @return  0 if OK, -1 if object is dead and passive (NOK)
     */
    Status getStatus() { return status; };

  private:
    std::thread thread_;
    int efd_;  // file descriptor for event notification in order to exit main_loop()
    int8_t appIndex_;
    int vdd_socket;
    Status status;

    void main_loop();

    // Pointers to client application's handler
    IVipComClient* vipcom_client_ = nullptr;
};
