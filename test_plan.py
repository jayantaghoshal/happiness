# Copyright 2017-2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

from shipit.test_runner.test_types import VTSTest, TradefedTest, Disabled, standard_caps, Capabilities as cp

#
# Rules to simplify merge errors as many people will be editing this file:
# * Don't reformat
# * Keep alphabetical order
# * Always include a comma even for the last item in the list
#
# Other rules:
# * Don't remove or comment out test cases,
#   Insted, wrap them with Disabled(..., reason="abc", jira_issue="123", deadline="yyyy-mm-dd").


# Tests in the gate must be fast and rock solid - New test must be run in hourly first before moving to gate.
# These should mainly be used for system stability, ie when a failing module will
# block other developers from continuing development. Such as "unit not booting", "adb not working", "black screen", etc
test_plan_gate = [
    VTSTest("vendor/volvocars/tools/test/daemon_test",                              standard_caps),
    VTSTest("vendor/volvocars/hardware/ci/test/ct/smoketest",                       standard_caps),
    VTSTest("vendor/volvocars/hardware/signals/dataelements/test/ut",               standard_caps),
    VTSTest("vendor/volvocars/hardware/tarmac/eventloop/test/ct",                   standard_caps),
    VTSTest("vendor/volvocars/hardware/localconfig/test/ut",                        standard_caps),
    VTSTest("vendor/volvocars/hardware/netman/test/ct/ip_configuration",            standard_caps),
    VTSTest("vendor/volvocars/hardware/netman/test/ct/iptables",                    standard_caps),
    VTSTest("vendor/volvocars/hardware/netman/test/ct/namespace_configuration",     standard_caps),
    VTSTest("vendor/volvocars/hardware/uds/test/ct",                                standard_caps),
    VTSTest("vendor/volvocars/tools/test/ui_performance/volvolauncher",             standard_caps),
]

test_plan_hourly = test_plan_gate + [
    VTSTest("vendor/volvocars/hardware/climate/test/ct",                      standard_caps),
    VTSTest("vendor/volvocars/hardware/netman/test/ct/most_interface",       standard_caps | {cp.audio}),
    VTSTest("vendor/volvocars/hardware/netman/test/ct/apix_obd_traffic_split",         standard_caps),
    VTSTest("vendor/volvocars/hardware/dim/test/ct/apix",       standard_caps | {cp.apix}),
    VTSTest("vendor/volvocars/hardware/evs/test/ut",                            standard_caps),
    # Climate included twice because the test case dynamically detects if FR hardware is present
    VTSTest("vendor/volvocars/hardware/climate/test/ct",                        standard_caps | {cp.flexray}),
    VTSTest("vendor/volvocars/hardware/carconfig/test/ct",                       standard_caps | {cp.flexray}),
    VTSTest("vendor/volvocars/hardware/ipcbd/test/ct",                          standard_caps),
    VTSTest("vendor/volvocars/hardware/test/iplm",                              standard_caps),
    VTSTest("vendor/volvocars/hardware/test/gnss",                              standard_caps),
    VTSTest("vendor/volvocars/hardware/netman/test/ct/uds_diagnostics_test",    standard_caps),
    VTSTest("vendor/volvocars/hardware/infotainmentIpBus/test/ut",              standard_caps),
    VTSTest("vendor/volvocars/hardware/signals/vehiclesignalsdaemon/test/ut",   standard_caps),
    VTSTest("vendor/volvocars/hardware/netman/test/ct/boot",                    standard_caps),
    VTSTest("vendor/volvocars/hardware/netman/test/ct/kernel_ip_stack",         standard_caps | {cp.audio}),
    VTSTest("vendor/volvocars/hardware/signals/vehiclesignalsdaemon/test/ut",   standard_caps),
    VTSTest("vendor/volvocars/hardware/signals/dataelements/test/ct/sendAndReceiveOneSignal", standard_caps | {cp.flexray}),
    TradefedTest("vendor/volvocars/hardware/netman/test/ct/usb2ethernet", standard_caps),
    TradefedTest("vendor/volvocars/hardware/localconfig/test/ct/localconfig_java",       standard_caps),
    TradefedTest("vendor/volvocars/hardware/carconfig/carconfig_java_lib/test/ct/carconfig_api", standard_caps),
    TradefedTest("vendor/volvocars/tools/test/log_analyzer", standard_caps),
    VTSTest("vendor/volvocars/hardware/powermoding/test/ct", standard_caps | {cp.flexray}),
    VTSTest("vendor/volvocars/hardware/test/keypanel", standard_caps | {cp.flexray}),
    VTSTest("vendor/volvocars/hardware/localconfig/test/ct/localconfig_nativelib",  standard_caps),
    Disabled(TradefedTest("vendor/volvocars/hardware/audio/test/ct/audio_mic_control/app", standard_caps | {cp.audio}),
             reason="Microphone HAL not fully implemented. Currently no ETA from Aptiv",
             jira_issue="PSS370-15982",
             deadline="2018-04-28"
    ),
    TradefedTest("vendor/volvocars/hardware/audio/test/ct/audio_loopback/app",       standard_caps | {cp.audio}),
    VTSTest("vendor/volvocars/hardware/tarmac/common/test/ct",              standard_caps),
    VTSTest("vendor/volvocars/hardware/settings/test/ct",                   standard_caps),
    VTSTest("vendor/volvocars/hardware/profiles/test/ct",                   standard_caps),

    # This test will set the screen-always-on to ON which will prevent the screen to power off.
    # If your test relies on the screen to power off, either put it before this test or set
    # screen-always-on to OFF in your AndroidTest.xml. N.B., resetting of this flag will trigger
    # a reboot.
    TradefedTest("vendor/volvocars/packages/SoftwareUpdateService/test/ct/test_suites/HappyGetAssignments",          standard_caps),
    TradefedTest("vendor/volvocars/packages/SoftwareUpdateService/test/ct/test_suites/HappyGetPendingInstallations", standard_caps),
    TradefedTest("vendor/volvocars/packages/SoftwareUpdateService/test/ct/test_suites/HappyGetDownloads",            standard_caps),

    VTSTest("vendor/volvocars/hardware/vehicle/test/dai_setting",               standard_caps | {cp.flexray}),
    VTSTest("vendor/volvocars/hardware/vehicle/test/connectedsafety_setting",   standard_caps | {cp.flexray}),
    VTSTest("vendor/volvocars/hardware/profiles/test/user_profile",             standard_caps | {cp.cem}),
    TradefedTest("vendor/volvocars/packages/BrightnessService/test/ct/BrightnessServiceTests", standard_caps),
    VTSTest("vendor/volvocars/hardware/soundnotifications",                             standard_caps),
    VTSTest("vendor/volvocars/hardware/vehicle/test/Vhal_SmokeTest",                    standard_caps),

    # Unit Test
    TradefedTest("vendor/volvocars/packages/CloudService/test/ut/services", standard_caps),
    TradefedTest("vendor/volvocars/packages/BrightnessService/test/ut/BrightnessServiceTests", standard_caps),

]

test_plan_nightly = test_plan_hourly + [

]

test_plan_staging = [

]
