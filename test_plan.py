# Copyright 2017 Volvo Car Corporation
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
    VTSTest("vendor/volvocars/hardware/ci/test/ct/smoketest",                       standard_caps),
    VTSTest("vendor/volvocars/hardware/signals/dataelements/test/ut",               standard_caps),
    VTSTest("vendor/volvocars/hardware/tarmac/eventloop/test/ct",                   standard_caps),
    VTSTest("vendor/volvocars/hardware/localconfig/test/ct/localconfig_nativelib",  standard_caps),
    VTSTest("vendor/volvocars/hardware/localconfig/test/ut",                        standard_caps),
    VTSTest("vendor/volvocars/hardware/netman/test/ct/ip_configuration",            standard_caps),
    VTSTest("vendor/volvocars/hardware/netman/test/ct/iptables",                    standard_caps),
    VTSTest("vendor/volvocars/hardware/netman/test/ct/namespace_configuration",     standard_caps),
    VTSTest("vendor/volvocars/hardware/uds/test/ct",                                standard_caps),
    VTSTest("vendor/volvocars/tools/test/daemon_test",                              standard_caps),
    VTSTest("vendor/volvocars/tools/test/ui_performance/volvolauncher",             standard_caps),
]

test_plan_hourly = test_plan_gate + [
    Disabled(VTSTest("vendor/volvocars/hardware/climate/test/ct",                      standard_caps),
             reason="Viewclients vc.dump() doesn't work when the 'you are in fullscreen'-popup is stealing focus, HMI to fix",
             jira_issue="",
             deadline="2018-01-31"
    ),
    Disabled(VTSTest("vendor/volvocars/hardware/netman/test/ct/most_interface",       standard_caps),
             reason="MR1 not yet on master",
             jira_issue="",
             deadline="2018-01-31"
    ),
    VTSTest("vendor/volvocars/hardware/netman/test/ct/apix_obd_traffic_split",         standard_caps),
    VTSTest("vendor/volvocars/hardware/dim/test/ct/apix",       standard_caps | {cp.apix}),
    # Climate included twice because the test case dynamically detects if FR hardware is present
    VTSTest("vendor/volvocars/hardware/climate/test/ct",                        standard_caps | {cp.flexray}),
    Disabled(VTSTest("vendor/volvocars/hardware/ipcbd/test/ct",                  standard_caps),
             reason="Team slytherin working on the failing tests",
             jira_issue= "",
             deadline= "2018-02-09"
    ),
    Disabled(VTSTest("vendor/volvocars/hardware/test/iplm",                      standard_caps),
             reason="Team slytherin working on the failing tests",
             jira_issue = "",
             deadline = "2018-02-09"
    ),
    VTSTest("vendor/volvocars/hardware/test/gnss",                              standard_caps),
    VTSTest("vendor/volvocars/hardware/infotainmentIpBus/test/ut",              standard_caps),
    VTSTest("vendor/volvocars/hardware/signals/vehiclesignalsdaemon/test/ut",   standard_caps),
    Disabled(VTSTest("vendor/volvocars/hardware/netman/test/ct/boot",       standard_caps),
             reason="Reboot is still unstable",
             jira_issue="",
             deadline="2018-02-09"
    ),
    Disabled(VTSTest("vendor/volvocars/hardware/netman/test/ct/kernel_ip_stack",       standard_caps),
             reason="Reboot is still unstable",
             jira_issue="",
             deadline="2018-02-09"
    ),
    VTSTest("vendor/volvocars/hardware/signals/vehiclesignalsdaemon/test/ut", standard_caps),
    VTSTest("vendor/volvocars/hardware/signals/dataelements/test/ct/sendAndReceiveOneSignal", standard_caps | {cp.flexray}),
    TradefedTest("vendor/volvocars/hardware/netman/test/ct/usb2ethernet",       standard_caps),
    TradefedTest("vendor/volvocars/hardware/localconfig/test/ct/localconfig_java",       standard_caps),
    TradefedTest("vendor/volvocars/hardware/carconfig/carconfig_java_lib/test/ct/carconfig_api", standard_caps),
    TradefedTest("vendor/volvocars/hardware/netman/test/ct/InterfaceTest",       standard_caps),
    TradefedTest("vendor/volvocars/tools/test/log_analyzer", standard_caps),
    VTSTest("vendor/volvocars/hardware/powermoding/test/ct", standard_caps | {cp.flexray}),
    VTSTest("vendor/volvocars/hardware/test/keypanel", standard_caps | {cp.flexray}),
    TradefedTest("vendor/volvocars/hardware/audio/test/ct/audio_mic_control/app", standard_caps | {cp.audio}),
    Disabled(TradefedTest("vendor/volvocars/hardware/audio/test/ct/audio_loopback/app",       standard_caps),
             reason="Waiting for fully working MOST drivers",
             jira_issue="",
             deadline="2018-01-31"
    ),
    VTSTest("vendor/volvocars/hardware/tarmac/common/test/ct",              standard_caps),
    VTSTest("vendor/volvocars/hardware/settings/test/ct",                   standard_caps),


    Disabled(VTSTest("vendor/volvocars/hardware/cloudd/test/ct/vtscloud", standard_caps),
             reason="Waiting for local web server in ci",
             jira_issue="",
             deadline="2018-03-01"
    )
]

test_plan_nightly = test_plan_hourly + [

]
