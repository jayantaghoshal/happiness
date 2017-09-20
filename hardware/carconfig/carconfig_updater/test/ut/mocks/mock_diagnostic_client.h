/*
 * mock_diagnostic_client.h
 *
 *  Created on: Aug 26, 2016
 *      Author: bastian
 */

#ifndef CARCONFIG_UPDATER_TEST_UT_MOCKS_MOCK_DIAGNOSTIC_CLIENT_H_
#define CARCONFIG_UPDATER_TEST_UT_MOCKS_MOCK_DIAGNOSTIC_CLIENT_H_


#include <diagnostics_client.h>
#include <gmock/gmock.h>
#include <map>
#include <vector>

typedef std::vector<uint8_t> valueVec_t;
typedef std::map<uint16_t, valueVec_t> valueMap_t;


//
// overwrite the functions provided by diagnosticsClient
//
class MockDiagnosticClient : public diagnosticsClient
{
public:
    MOCK_METHOD0(connect, void());
    MOCK_METHOD2(sendDiagnosticsMessage, void(uint16_t id, valueVec_t message));
    MOCK_METHOD2(registerDID, void(uint16_t id, valueVec_t values));
    MOCK_METHOD2(updateDID, void(uint16_t id, valueVec_t values));
    MOCK_METHOD2(printMessage, void(uint16_t id, valueVec_t values));
    MOCK_METHOD0(runForever, void());
};


void diag_mock_init(MockDiagnosticClient * mockInstance);




#endif /* CARCONFIG_UPDATER_TEST_UT_MOCKS_MOCK_DIAGNOSTIC_CLIENT_H_ */
