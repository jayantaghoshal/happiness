/*
 * Copyright 2017-2018 Volvo Car Corporation
 * This file is covered by LICENSE file in the root of this project
 */

#pragma once

#include <ipcommandbus/Message.h>
#include <ipcommandbus/MessageDispatcher.h>
#include <ipcommandbus/TransportServices.h>
#include <ipcommandbus/socket.h>
#include <memory>
#include <vector>

#include "diagnostics_client.h"

namespace vcc {
namespace ipcb {
namespace testing {

using EcuIpMap = std::vector<std::pair<Connectivity::Message::Ecu, Connectivity::ISocket::EcuAddress>>;

class IpcbSimulator {
  public:
    explicit IpcbSimulator(const EcuIpMap& ecu_map);
    ~IpcbSimulator() = default;

    uint64_t RegisterMessageCallback(const Connectivity::IpCmdTypes::ServiceId& serviceId,
                                     const Connectivity::IpCmdTypes::OperationId& operationId,
                                     const Connectivity::IpCmdTypes::OperationType& operationType,
                                     Connectivity::MessageDispatcher::MessageCallback messageCb);

    bool UnregisterCallback(const uint64_t& registeredReceiverId);
    void SendMessage(Connectivity::Message&& msg,
                     std::shared_ptr<Connectivity::MessageDispatcher::CallerData> pCallerData = nullptr);

    void Initialize(const Connectivity::Message::Ecu& ecu, const std::string& protocol);

  private:
    void SetupSocket(const Connectivity::Message::Ecu& ecu);

    std::unique_ptr<Connectivity::TransportServices> transport_services_;
    std::unique_ptr<Connectivity::MessageDispatcher> message_dispatcher_;
    std::unique_ptr<DiagnosticsClient> diagnostics_client_;

    std::shared_ptr<Connectivity::Socket> sock_;

    EcuIpMap ecu_map_;
};
}  // namespace testing
}  // namespace ipcb
}  // namespace vcc