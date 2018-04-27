/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "mqtt_callbacks.h"
#include <cutils/log.h>

#define LOG_TAG "CloudD"
action_listener::action_listener(const std::string& name) {
    name_ = name;
}
// Callbacks for the success or failures of requested actions.
// This could be used to initiate further action, but here we just log the
// results to the console.

void action_listener::on_failure(const mqtt::token& tok) {
    ALOGD("%s failure", name_.c_str());
    if (tok.get_message_id() != 0) ALOGD(" for token: [%d] ", tok.get_message_id());
    auto top = tok.get_topics();
    if (top && !top->empty()) ALOGD("\ttoken topic: ' %s ', ...", (*top)[0].c_str());
}

void action_listener::on_success(const mqtt::token& tok) {
    ALOGD("%s success", name_.c_str());
    if (tok.get_message_id() != 0) ALOGD(" for token: [%d]", tok.get_message_id());
    auto top = tok.get_topics();
    if (top && !top->empty()) ALOGD("\ttoken topic: ' %s ', ...", (*top)[0].c_str());
}

/////////////////////////////////////////////////////////////////////////////

MqttCallback::MqttCallback() : subListener_("Subscription") {}

void MqttCallback::SetConnectionHandler(std::function<void()> connectionHandler) {
    connectionHandler_ = connectionHandler;
}
void MqttCallback::SetMessageHandler(std::function<void(mqtt::const_message_ptr msg)> messageHandler) {
    messageHandler_ = messageHandler;
}

// Re-connection failure
void MqttCallback::on_failure(const mqtt::token& tok) {
    ALOGD("Connection failed");
}

// Re-connection success
void MqttCallback::on_success(const mqtt::token& tok) {
    ALOGD("\nConnection success");

    if (connectionHandler_) connectionHandler_();
}

// MqttCallback for when the connection is lost.
// This will initiate the attempt to manually reconnect.
void MqttCallback::connection_lost(const std::string& cause) {
    ALOGD("\nConnection lost");
    if (!cause.empty()) ALOGD("\tcause: %s", cause.c_str());

    ALOGD("Reconnecting...");
}

// MqttCallback for when a message arrives.
void MqttCallback::message_arrived(mqtt::const_message_ptr msg) {
    ALOGD("Message arrived");

    if (messageHandler_) messageHandler_(msg);
}
