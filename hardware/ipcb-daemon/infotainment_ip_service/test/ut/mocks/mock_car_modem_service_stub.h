/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
#ifndef MOCK_CAR_MODEM_SERVICE_STUB_H
#define MOCK_CAR_MODEM_SERVICE_STUB_H

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <sstream>
#include <v0/org/volvo/connectivity/CarModemStub.hpp>
#include "services/car_modem_service_stub.h"

namespace gen_conn = v0::org::volvo::connectivity;

class MockCarModemServiceStubAdapter : public gen_conn::CarModemStubAdapter
{
public:
    MOCK_METHOD1(fireCarModemCurrentApnConfigurationAttributeChanged,
                 void(const gen_conn::CarModem::CarModem_ApnConfigurationStatus &));

    MOCK_METHOD1(
        fireCarModemConnectionStatusNotificationEvent,
        void(const ::v0::org::volvo::connectivity::IpCommandBusTypes::CarModem_ConnectionStatus &_outConnectionStatus));
    ;

    MOCK_METHOD1(fireCarModemNetworkRegistrationStatusNotificationEvent,
                 void(const ::v0::org::volvo::connectivity::IpCommandBusTypes::CarModem_RegistrationStatus
                          &_outRegistrationStatus));

    MOCK_METHOD1(
        fireCarModemPSIMStateNotificationEvent,
        void(const ::v0::org::volvo::connectivity::IpCommandBusTypes::CarModem_SIMCardStatus &_outSIMCardStatus));

    MOCK_METHOD1(fireCarModemSignalStrengthChangedNotificationEvent, void(const uint8_t &_outSignalStrength));

    MOCK_METHOD2(fireCarModemUssdNotificationEvent,
                 void(const gen_conn::CarModem::CarModem_UssdStatus &_ussdStatus, const std::string &_ussdReturnCode));

    MOCK_METHOD2(fireCarModemNetworkSelectionNotificationEvent,
                 void(const gen_conn::IpCommandBusTypes::CarModem_OperatorSelection &_status,
                      const gen_conn::IpCommandBusTypes::GenericOkStatus &_operatorSelectionResult));

    virtual void deactivateManagedInstances() override
    {
    }
};

class MockCarModemStub : public Connectivity::CarModemServiceStub
{
public:
    MockCarModemStub() : Connectivity::CarModemServiceStub()
    {
    }

    MOCK_METHOD2(GetCarModemPSIMDataCounter,
                 void(const std::shared_ptr<CommonAPI::ClientId> _client,
                      gen_conn::CarModemStub::GetCarModemPSIMDataCounterReply_t _reply));

    MOCK_METHOD2(ResetCarModemPSIMDataCounter,
                 void(const std::shared_ptr<CommonAPI::ClientId> _client,
                      gen_conn::CarModemStub::ResetCarModemPSIMDataCounterReply_t _reply));

    MOCK_METHOD3(EnterCarModemPSIMCode,
                 void(const std::shared_ptr<CommonAPI::ClientId> _client,
                      gen_conn::IpCommandBusTypes::CarModem_PinPukCommand _command,
                      gen_conn::CarModemStub::EnterCarModemPSIMCodeReply_t _reply));

    MOCK_METHOD1(fireCarModemConnectionStatusNotificationEvent,
                 void(const gen_conn::IpCommandBusTypes::CarModem_ConnectionStatus &_outConnectionStatus));

    MOCK_METHOD1(fireCarModemNetworkRegistrationStatusNotificationEvent,
                 void(const gen_conn::IpCommandBusTypes::CarModem_RegistrationStatus &_outRegistrationStatus));

    MOCK_METHOD1(fireCarModemPSIMStateNotificationEvent,
                 void(const gen_conn::IpCommandBusTypes::CarModem_SIMCardStatus &_outSIMCardStatus));

    MOCK_METHOD1(fireCarModemSignalStrengthChangedNotificationEvent, void(const uint8_t &_outSignalStrength));

    MOCK_METHOD2(fireCarModemUssdNotificationEvent,
                 void(const gen_conn::CarModem::CarModem_UssdStatus &_ussdStatus, const std::string &_ussdReturnCode));

    MOCK_METHOD1(
        getCarModemCurrentApnConfigurationAttribute,
        gen_conn::CarModem::CarModem_ApnConfigurationStatus &(const std::shared_ptr<CommonAPI::ClientId> _client));
};

#endif  // MOCK_CAR_MODEM_SERVICE_STUB_H
