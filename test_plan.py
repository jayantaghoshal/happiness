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
    VTSTest("vendor/volvocars/hardware/ci/test/ct/smoketest",                  standard_caps),
    VTSTest("vendor/volvocars/hardware/signals/dataelements/test/ut",          standard_caps),
    VTSTest("vendor/volvocars/hardware/tarmac/eventloop/test/ct",              standard_caps),
    VTSTest("vendor/volvocars/hardware/localconfig/test/ct",                   standard_caps),
    VTSTest("vendor/volvocars/hardware/localconfig/test/ut",                   standard_caps),
    VTSTest("vendor/volvocars/hardware/netman/test/ct/ip_configuration",       standard_caps),
    VTSTest("vendor/volvocars/hardware/netman/test/ct/iptables",               standard_caps),
    VTSTest("vendor/volvocars/hardware/netman/test/ct/namespace_configuration",standard_caps),
    VTSTest("vendor/volvocars/hardware/uds/test/ct",                           standard_caps),
    VTSTest("vendor/volvocars/tools/test/daemon_test",                         standard_caps),
]

test_plan_hourly = [
    VTSTest("vendor/volvocars/hardware/ci/test/ct/smoketest",                   standard_caps),
    Disabled(VTSTest("vendor/volvocars/hardware/climate/test/ct",                      standard_caps),
             reason="Viewclients vc.dump() doesn't work when the 'you are in fullscreen'-popup is stealing focus, HMI to fix",
             jira_issue="",
             deadline="2017-10-14"
    ),
    Disabled(VTSTest("vendor/volvocars/hardware/netman/test/ct/blacklist",    standard_caps),
             reason="Blacklist functionality not yet implemented",
             jira_issue="",
             deadline="2017-10-19"
    ),
    Disabled(VTSTest("vendor/volvocars/hardware/netman/test/ct/most_interface",       standard_caps),
             reason="MR1 not yet on master",
             jira_issue="",
             deadline="2018-01-21"
    ),
    Disabled(VTSTest("vendor/volvocars/hardware/infotainmentIpBus/test/ut",            standard_caps),
             reason="Dead code in repo",
             jira_issue="",
             deadline="2017-11-01"
    ),
    Disabled(VTSTest("vendor/volvocars/hardware/dim/test/ct/apix_gate",       standard_caps),
             reason="APIX driver not yet integrated on Master",
             jira_issue="",
             deadline="2017-10-12"
    ),
    # Climate included twice because the test case dynamically detects if FR hardware is present
    VTSTest("vendor/volvocars/hardware/climate/test/ct",                        standard_caps | {cp.flexray}),
    VTSTest("vendor/volvocars/hardware/ipcbd/test/ct",                          standard_caps),
    VTSTest("vendor/volvocars/hardware/test/iplm",                              standard_caps),
    VTSTest("vendor/volvocars/hardware/test/gnss",                              standard_caps),
    VTSTest("vendor/volvocars/hardware/signals/vehiclesignalsdaemon/test/ut",   standard_caps),
    VTSTest("vendor/volvocars/hardware/netman/test/ct/boot",                  standard_caps),
    VTSTest("vendor/volvocars/hardware/netman/test/ct/kernel_ip_stack",        standard_caps),
    VTSTest("vendor/volvocars/hardware/signals/vehiclesignalsdaemon/test/ut", standard_caps),
    VTSTest("vendor/volvocars/hardware/signals/dataelements/test/ct/sendAndReceiveOneSignal", standard_caps | {cp.flexray}),
    VTSTest("vendor/volvocars/tools/test/ui_performance/volvolauncher",         standard_caps),
    Disabled(VTSTest("vendor/volvocars/hardware/netman/test/ct/usb2ethernet",       standard_caps),
             reason="Manual test which require usb2eth dongel",
             jira_issue="",
             deadline="2019-11-17"
    ),
    TradefedTest("vendor/volvocars/tools/test/log_analyzer", standard_caps),
    VTSTest("vendor/volvocars/hardware/powermoding/test/ct", standard_caps | {cp.flexray}),
]

test_plan_nightly = [

]
