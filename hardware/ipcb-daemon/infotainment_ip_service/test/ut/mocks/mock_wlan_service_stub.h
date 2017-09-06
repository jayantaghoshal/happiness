/*===========================================================================*\
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
\*===========================================================================*/
/*
* This file was generated by the CommonAPI Generators.
* Used org.genivi.commonapi.core 3.1.3.v201507280808.
* Used org.franca.core 0.9.1.201412191134.
*
* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
* If a copy of the MPL was not distributed with this file, You can obtain one at
* http://mozilla.org/MPL/2.0/.
*/
/**
 * description: This is the WLAN interface on IP Command Bus
 */
#ifndef MOCK_WLAN_SERVICE_STUB_H
#define MOCK_WLAN_SERVICE_STUB_H

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <sstream>
#include <v0/org/volvo/connectivity/WlanStub.hpp>
#include "services/wlan_service_stub.h"

namespace gen_conn = v0::org::volvo::connectivity;

/**
 * Provides a default implementation for WlanStubRemoteEvent and
 * WlanStub. Method callbacks have an empty implementation,
 * remote set calls on attributes will always change the value of the attribute
 * to the one received.
 *
 * Override this stub if you only want to provide a subset of the functionality
 * that would be defined for this service, and/or if you do not need any non-default
 * behaviour.
 */
class MockWlanStub : public Connectivity::WlanStub
{
public:
    MockWlanStub() : Connectivity::WlanStub(nullptr)
    {
    }

    MockWlanStub(Connectivity::IWlanService *service) : Connectivity::WlanStub(service)
    {
    }

    /**
     * description: (0x040D WLANSTAStatus.Notification)
     */
    MOCK_METHOD2(
        fireWlanStaStatusNotificationEvent,
        void(const ::v0::org::volvo::connectivity::IpCommandBusTypes::Infotainment_IP_bus_WlanConnectStatus
                 &_outWlanConnectionStatus,
             const std::vector<
                 ::v0::org::volvo::connectivity::IpCommandBusTypes::Infotainment_IP_bus_AvailableNetworkInformation>
                 &_outAvailableNetworkInfoList));

    //    MOCK_METHOD2(fireWlanStaConnectNotificationEvent,
    //        void(const ::v0::org::volvo::connectivity::IpCommandBusTypes::Infotainment_IP_bus_Ssid &_outCurrentSsid,
    //             const ::v0::org::volvo::connectivity::IpCommandBusTypes::Infotainment_IP_bus_WlanConnectStatus
    //             &_outCurrentWlanConnectStatus));

    /**
     * description: (0x0408 WLANSTAConnect.)
     *              Event based notification of WLAN STA Connect
     */
    MOCK_METHOD2(
        fireWlanStaConnectNotificationEvent,
        void(const ::v0::org::volvo::connectivity::IpCommandBusTypes::Infotainment_IP_bus_Ssid &_outCurrentSsid,
             const ::v0::org::volvo::connectivity::IpCommandBusTypes::Infotainment_IP_bus_WlanConnectStatus
                 &_outCurrentWlanConnectStatus));

    /**
     * description: (0x0402 WLANChannelFreqInfo)
     */
    MOCK_METHOD1(fireWlanChannelFreqInfoAttributeChanged,
                 void(const ::v0::org::volvo::connectivity::Wlan::WlanChannelFreqInfo &wlanChannelFreqInfo));

    MOCK_METHOD1(getWlanChannelFreqInfoAttribute,
                 const ::v0::org::volvo::connectivity::Wlan::WlanChannelFreqInfo &(
                     const std::shared_ptr<CommonAPI::ClientId> _client));

    /**
     * description: (0x0400 WLANMode.Notification)
        Event based notification of current WLAN
     *   mode.
     */
    MOCK_METHOD1(
        fireWlanModeNotificationEvent,
        void(const ::v0::org::volvo::connectivity::IpCommandBusTypes::Infotainment_IP_bus_WlanMode &_wlanMode));
};

#endif  // MOCK_WLAN_SERVICE_STUB_H
