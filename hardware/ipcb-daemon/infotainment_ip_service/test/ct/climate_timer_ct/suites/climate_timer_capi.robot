*** Settings ***
Documentation   Test suite for climate timer CAPI side
Suite Setup     Suite Startup
Suite Teardown  Suite Reset
Test Setup      Reset Assistance Services
Test Teardown   Cleanup
Library         ../resources/SCPLibrary.py
Library         ../resources/ClimateTimerCAPILibrary.py

*** Test Cases ***
Send Request Timer Response
    [Documentation]             Emit  request timer list response
    emit_request_timer_list_response_with     callstatus=3
    #manually check result

Receive asyncCallBack
    [Documentation]             wait for manual trigger async callback request and check the result
    #manually start sending msg on IP commond bus
    Pause Execution
    asyncCallBack should be

*** Keywords ***

Suite Startup
    Open Connection     198.18.34.1    username=root    password=root
    Get File            /etc/dbus-1/system.d/org.volvocars.climate.ParkingClimate.conf              resources/org.volvocars.climate.ParkingClimate.conf
    Put File            resources/org.volvocars.climate.ParkingClimate.conf.TestConfig              /etc/dbus-1/system.d/org.volvocars.climate.ParkingClimate.conf
    Get File            /etc/dbus-1/system.d/org.volvocars.InfotainmentIpService.conf    resources/org.volvocars.InfotainmentIpService.conf
    Put File            resources/org.volvocars.InfotainmentIpService.conf.TestConfig    /etc/dbus-1/system.d/org.volvocars.InfotainmentIpService.conf

Suite Reset
    Put File            resources/org.volvocars.climate.ParkingClimate.conf             /etc/dbus-1/org.volvocars.climate.ParkingClimate.conf
    Put File            resources/org.volvocars.InfotainmentIpService.conf    /etc/dbus-1/system.d/org.volvocars.InfotainmentIpService.conf
    Close Connection

Cleanup

*** Variables ***
