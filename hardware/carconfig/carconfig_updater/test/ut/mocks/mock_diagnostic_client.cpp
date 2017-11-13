#include <gmock/gmock.h>
#include <mock_diagnostic_client.h>

diagnosticsClient::diagnosticsClient(){};

void diagnosticsClient::connect() { static_cast<MockDiagnosticClient*>(this)->connect(); }

void diagnosticsClient::printMessage(uint16_t id, std::vector<uint8_t> message) {
    static_cast<MockDiagnosticClient*>(this)->printMessage(id, message);
}

void diagnosticsClient::sendDiagnosticsMessage(uint16_t id, std::vector<uint8_t> message) {
    static_cast<MockDiagnosticClient*>(this)->sendDiagnosticsMessage(id, message);
}

void diagnosticsClient::registerDID(uint16_t id, std::vector<uint8_t> values) {
    static_cast<MockDiagnosticClient*>(this)->registerDID(id, values);
}

void diagnosticsClient::updateDID(uint16_t id, std::vector<uint8_t> values) {
    static_cast<MockDiagnosticClient*>(this)->updateDID(id, values);
}

void diagnosticsClient::runForever() { static_cast<MockDiagnosticClient*>(this)->runForever(); }
