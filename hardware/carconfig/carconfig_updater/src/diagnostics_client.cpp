#include "diagnostics_client.h"
#include <ivi-logging.h>
#include <unistd.h>
#include <functional>
#include <iomanip>
#include <iterator>
#include <sstream>

typedef logging::DefaultLogContext LogContext;
LOG_DECLARE_CLASS_CONTEXT("DIAG", "Diagnostics client");

diagnosticsClient::diagnosticsClient() {
    runtime = CommonAPI::Runtime::get();
    diagnosticsProxy =
            runtime->buildProxy<v1::com::ihu::Diagnostics::DiagnosticsProxy>("local", "main", "DiagnosticsProxy");
}

void diagnosticsClient::connect() {
    while (!diagnosticsProxy->isAvailable()) {
        log_info() << "Attempting to connect to diagnostics service...";
        usleep(500000);
    }
    log_info() << "Connected!";

    v1::com::ihu::Diagnostics::DiagnosticsProxyBase::MessageToAppSelectiveEvent::Listener diagnosticsListener(
            std::bind(&diagnosticsClient::capiCallback, this, std::placeholders::_1, std::placeholders::_2));
    diagnosticsProxy->getMessageToAppSelectiveEvent().subscribe(diagnosticsListener);
}

void diagnosticsClient::printMessage(uint16_t id, std::vector<uint8_t> message) {
    std::ostringstream result;
    result << std::setw(2) << std::setfill('0') << std::hex << std::uppercase;
    std::copy(message.begin(), message.end(), std::ostream_iterator<uint32_t>(result, " "));
    log_info() << "Message ID:" << id << "Payload: " << result.str() << "Payload Size:" << message.size();
}

void diagnosticsClient::sendDiagnosticsMessage(uint16_t id, std::vector<uint8_t> message) {
    printMessage(id, message);
    CommonAPI::CallStatus callStatus;
    diagnosticsProxy->MessageToDiag(id, message, callStatus);
}

void diagnosticsClient::capiCallback(uint16_t key, std::vector<uint8_t> payload) {
    if (payload.size() == 0) {
        log_info() << "Got DID readout request for key " << key;
        for (auto DID : DIDList) {
            if (DID.first == key) {
                sendDiagnosticsMessage(key, DID.second);
            }
        }
    }
}

void diagnosticsClient::registerDID(uint16_t id, std::vector<uint8_t> values) {
    log_info() << "Registering DID " << id;
    DIDList.insert(std::pair<uint16_t, std::vector<uint8_t>>(id, values));
    CommonAPI::CallStatus callStatus;
    diagnosticsProxy->SubscribeOnKeys({id}, callStatus);
}

void diagnosticsClient::updateDID(uint16_t id, std::vector<uint8_t> values) {
    std::map<uint16_t, std::vector<uint8_t>>::iterator it = DIDList.find(id);
    if (it != DIDList.end()) {
        log_info() << "Updating DID " << id;
        it->second = values;
    } else {
        log_error() << "Could not locate DID to update" << id;
    }
}

std::map<uint16_t, std::vector<uint8_t>> diagnosticsClient::testGetDIDList() { return DIDList; }

void diagnosticsClient::runForever() {
    while (1) {
        usleep(100000);
    }
}
