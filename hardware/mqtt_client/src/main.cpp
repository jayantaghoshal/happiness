/*
 * Copyright 2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include <atomic>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <thread>  // For sleep

#include <IDispatcher.h>
#include <hidl/HidlTransportSupport.h>
#include <sys/signalfd.h>
#include <cctype>
#include <cstdint>
#include <functional>
#include <future>
#include <memory>
#include <utility>

#include "mqtt/async_client.h"

const std::string SERVER_ADDRESS("tcp://localhost:1883");
const std::string CLIENT_ID("mqtt_client");
const std::string TOPIC("ota2");
const int KEEP_ALIVE_INTERVAL = 10;
const int QOS = 2;

#define LOG_TAG "mqttd"

using namespace std;

using namespace tarmac::eventloop;
using namespace android::hardware;

// Setup signal handlers
void SigTermHandler(int fd) {
    struct signalfd_siginfo sigdata;
    read(fd, &sigdata, sizeof(sigdata));

    ALOGD("SIGTERM received...");

    IDispatcher::GetDefaultDispatcher().Stop();  // stop our own IDispatcher mainloop
    IPCThreadState::self()->stopProcess();       // Stop the binder
}

void SigHupHandler(int fd) {
    struct signalfd_siginfo sigdata;
    read(fd, &sigdata, sizeof(sigdata));

    ALOGD("SIGHUP received...");
}

void SigIntHandler(int fd) {
    struct signalfd_siginfo sigdata;
    read(fd, &sigdata, sizeof(sigdata));

    ALOGD("SIGINT received...");

    IDispatcher::GetDefaultDispatcher().Stop();  // stop our own IDispatcher mainloop
    IPCThreadState::self()->stopProcess();       // Stop the binder
}

bool InitSignals() {
    sigset_t signal_set_term;
    sigset_t signal_set_hup;
    sigset_t signal_set_int;

    // Create signals
    if ((sigemptyset(&signal_set_term) < 0) || (sigemptyset(&signal_set_hup) < 0) ||
        (sigemptyset(&signal_set_int) < 0) || (sigaddset(&signal_set_term, SIGTERM) < 0) ||
        (sigaddset(&signal_set_hup, SIGHUP) < 0) || (sigaddset(&signal_set_int, SIGINT) < 0)) {
        ALOGE("Failed to create signals: %s", strerror(errno));
        return false;
    }

    // Block signals until we have added support for them
    if ((sigprocmask(SIG_BLOCK, &signal_set_term, nullptr) < 0) ||
        (sigprocmask(SIG_BLOCK, &signal_set_hup, nullptr) < 0) ||
        (sigprocmask(SIG_BLOCK, &signal_set_int, nullptr) < 0)) {
        ALOGE("Failed to block signals: %s", strerror(errno));
        return false;
    }

    int termfd = signalfd(-1, &signal_set_term, 0);
    int hupfd = signalfd(-1, &signal_set_hup, 0);
    int intfd = signalfd(-1, &signal_set_int, 0);

    if (termfd < 0 || hupfd < 0 || intfd < 0) {
        ALOGE("signalfd failed: %s", strerror(errno));
        return false;
    }

    IDispatcher::GetDefaultDispatcher().AddFd(termfd, [termfd]() { SigTermHandler(termfd); });

    IDispatcher::GetDefaultDispatcher().AddFd(hupfd, [hupfd]() { SigHupHandler(hupfd); });

    IDispatcher::GetDefaultDispatcher().AddFd(intfd, [intfd]() { SigIntHandler(intfd); });

    return true;
}

/////////////////////////////////////////////////////////////////////////////

// Callbacks for the success or failures of requested actions.
// This could be used to initiate further action, but here we just log the
// results to the console.

class action_listener : public virtual mqtt::iaction_listener {
    std::string name_;

    void on_failure(const mqtt::token& tok) override {
        ALOGD("%s failure", name_.c_str());
        if (tok.get_message_id() != 0) ALOGD(" for token: [%d]", tok.get_message_id());
    }

    void on_success(const mqtt::token& tok) override {
        ALOGD("%s success", name_.c_str());
        if (tok.get_message_id() != 0) ALOGD(" for token: [%d]", tok.get_message_id());
        auto top = tok.get_topics();
        if (top && !top->empty()) ALOGD("\ttoken topic: ' %s ', ...", (*top)[0].c_str());
    }

  public:
    action_listener(const std::string& name) : name_(name) {}
};

/////////////////////////////////////////////////////////////////////////////

/**
 * Local callback & listener class for use with the client connection.
 * This is primarily intended to receive messages, but it will also monitor
 * the connection to the broker. If the connection is lost, it will attempt
 * to restore the connection and re-subscribe to the topic.
 */
class callback : public virtual mqtt::callback,
                 public virtual mqtt::iaction_listener

{
    // The MQTT client
    mqtt::async_client& cli_;
    // Options to use if we need to reconnect
    mqtt::connect_options& connOpts_;
    // An action listener to display the result of actions.
    action_listener subListener_;

    // This deomonstrates manually reconnecting to the broker by calling
    // connect() again. This is a possibility for an application that keeps
    // a copy of it's original connect_options, or if the app wants to
    // reconnect with different options.
    // Another way this can be done manually, if using the same options, is
    // to just call the async_client::reconnect() method.
    void reconnect() {
        std::this_thread::sleep_for(std::chrono::milliseconds(2500));
        try {
            cli_.connect(connOpts_, nullptr, *this);
        } catch (const mqtt::exception& exc) {
            ALOGE("Error: %s", exc.what());
            exit(1);
        }
    }

    // Re-connection failure
    void on_failure(const mqtt::token& tok) override {
        ALOGD("Connection failed");
        reconnect();
    }

    // Re-connection success
    void on_success(const mqtt::token& tok) override {
        ALOGD("\nConnection success");
        ALOGD("\nSubscribing to topic '%s'\n\tfor client %s using QoS %d", TOPIC.c_str(), CLIENT_ID.c_str(), QOS);

        cli_.subscribe(TOPIC, QOS, nullptr, subListener_);
    }

    // Callback for when the connection is lost.
    // This will initiate the attempt to manually reconnect.
    void connection_lost(const std::string& cause) override {
        ALOGD("\nConnection lost");
        if (!cause.empty()) ALOGD("\tcause: %s", cause.c_str());

        ALOGD("Reconnecting...");
        reconnect();
    }

    // Callback for when a message arrives.
    void message_arrived(mqtt::const_message_ptr msg) override {
        ALOGD("Message arrived");
        ALOGD("\ttopic: '%s'", msg->get_topic().c_str());
        ALOGD("\tpayload: '%s", msg->to_string().c_str());
    }

    void delivery_complete(mqtt::delivery_token_ptr token) override {}

  public:
    callback(mqtt::async_client& cli, mqtt::connect_options& connOpts)
        : cli_(cli), connOpts_(connOpts), subListener_("Subscription") {}
};

/////////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[]) {
    mqtt::connect_options connOpts;
    // TODO: Remove this todo by verifying the connection options
    connOpts.set_keep_alive_interval(KEEP_ALIVE_INTERVAL);
    connOpts.set_clean_session(true);

    mqtt::async_client client(SERVER_ADDRESS, CLIENT_ID);

    callback cb(client, connOpts);
    client.set_callback(cb);

    // Start the connection.
    // When completed, the callback will subscribe to topic.

    try {
        ALOGD("Connect attempt to the MQTT server ...");
        client.connect(connOpts, nullptr, cb);
    } catch (const mqtt::exception& exc) {
        ALOGE("Error: %s", exc.to_string().c_str());
        ALOGE("Error: %s", exc.what());
        ALOGE("\nERROR: Unable to connect to MQTT server: %s''", SERVER_ADDRESS.c_str());
        exit(1);
    }

    configureRpcThreadpool(1, true /*callerWillJoin*/);

    joinRpcThreadpool();
    return 0;
}
