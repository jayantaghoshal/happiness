/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#include "ipcb_simulator.h"

#include <ipcommandbus/TcpSocket.h>
#include <ipcommandbus/UdpSocket.h>

#include <chrono>
#include <thread>

#undef LOG_TAG
#define LOG_TAG "IpcbSim"
#include <cutils/log.h>

namespace vcc {
namespace ipcb {
namespace testing {

IpcbSimulator::IpcbSimulator(const EcuIpMap& ecu_map) : ecu_map_(std::move(ecu_map)) {}

void IpcbSimulator::Initialize(const Connectivity::Message::Ecu& ecu, const std::string& protocol) {
    tarmac::eventloop::IDispatcher& dispatcher = tarmac::eventloop::IDispatcher::GetDefaultDispatcher();
    transport_services_ = std::make_unique<Connectivity::TransportServices>(dispatcher, dispatcher, ecu);

    try {
        if (protocol == "UDP") {
            ALOGV("Setting up a UDP socket");
            sock_ = std::make_shared<Connectivity::UdpSocket>(dispatcher, ecu_map_);
            transport_services_->setSocket(sock_.get());
            SetupSocket(ecu);
        } else if (protocol == "UDPB") {
            ALOGV("Setting up a UDPB socket");
            sock_ = std::make_shared<Connectivity::UdpSocket>(dispatcher, ecu_map_);
            transport_services_->setBroadcastSocket(sock_.get());
            SetupSocket(Connectivity::Message::ALL);
        } else if (protocol == "TCP") {
            ALOGV("Setting up a TCP socket");
            ALOGW("TCP NOT SUPPORTED...");
            assert(false);
        } else {
            ALOGE("Unknown protocol specified (%s)", protocol.c_str());
            assert(false);
        }

        message_dispatcher_ = std::make_unique<Connectivity::MessageDispatcher>(transport_services_.get(), dispatcher);
        diagnostics_client_ = std::make_unique<DiagnosticsClient>();

        message_dispatcher_->setDiagnostics(diagnostics_client_.get());
    } catch (const Connectivity::SocketException& e) {
        ALOGE("%s . Code (%s : %i)", e.what(), e.code().category().name(), e.code().value());
    }
}

uint64_t IpcbSimulator::RegisterMessageCallback(const Connectivity::IpCmdTypes::ServiceId& serviceId,
                                                const Connectivity::IpCmdTypes::OperationId& operationId,
                                                const Connectivity::IpCmdTypes::OperationType& operationType,
                                                Connectivity::MessageDispatcher::MessageCallback messageCb) {
    return message_dispatcher_->registerMessageCallback(serviceId, operationId, operationType, std::move(messageCb));
}

bool IpcbSimulator::UnregisterCallback(const uint64_t& registeredReceiverId) {
    return message_dispatcher_->unregisterCallback(registeredReceiverId);
}
void IpcbSimulator::SendMessage(Connectivity::Message&& msg,
                                std::shared_ptr<Connectivity::MessageDispatcher::CallerData> pCallerData) {
    message_dispatcher_->sendMessage(std::move(msg), std::move(pCallerData));
}

void IpcbSimulator::SetupSocket(const Connectivity::Message::Ecu& ecu) {
    // Wait for-ever, no need to stop since service is dependent on this to work
    std::error_code previous_error;
    do {
        ALOGI("Setup socket for ecu %u", ecu);
        try {
            sock_->setup(ecu);
            ALOGD("Setup socket for Ecu %u successfully", ecu);
            return;
        } catch (const Connectivity::SocketException& e) {
            if (e.code() != previous_error) {
                previous_error = e.code();
                ALOGE("Can not setup socket for Ecu %u, error %s, continue trying...", ecu, e.what());
            }
            using namespace std::chrono_literals;
            std::this_thread::sleep_for(100ms);
        }
    } while (true);
}

}  // namespace testing
}  // namespace ipcb
}  // namespace vcc