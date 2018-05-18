/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "mqtt/async_client.h"

// TODO: delete constants when actual values can be retrieved
const std::string CLIENT_ID("ota_mq");
const std::string TOPIC("ota_shoulder");
const int KEEP_ALIVE_INTERVAL = 10;
const int QOS = 2;

class action_listener : public virtual mqtt::iaction_listener {
  public:
    action_listener(const std::string& name);

  private:
    std::string name_;
    void on_failure(const mqtt::token& tok) override;
    void on_success(const mqtt::token& tok) override;
};

/**
 * Local callback & listener class for use with the client connection.
 * This is primarily intended to receive messages, but it will also monitor
 * the connection to the broker. If the connection is lost, it will attempt
 * to restore the connection and re-subscribe to the topic.
 */
class MqttCallback : public virtual mqtt::callback, public virtual mqtt::iaction_listener {
  public:
    // callback(mqtt::async_client_ptr cli, mqtt::connect_options_ptr connOpts);
    MqttCallback();
    void SetMessageHandler(std::function<void(mqtt::const_message_ptr msg)> messageHandler);
    void SetConnectionHandler(std::function<void()> connectionHandler);
    // An action listener to display the result of actions.
    action_listener subListener_;

  private:
    // callbacks and virtual methods

    void on_failure(const mqtt::token& tok) override;
    void on_success(const mqtt::token& tok) override;

    void connection_lost(const std::string& cause) override;
    void message_arrived(mqtt::const_message_ptr) override;

    std::function<void()> connectionHandler_;
    std::function<void(mqtt::const_message_ptr msg)> messageHandler_;
};
