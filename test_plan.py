from shipit.test_runner.test_types import VTSTest, Disabled, standard_caps, Capabilities as cp

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
    Disabled(VTSTest("vendor/volvocars/hardware/climate/test/ct",                      standard_caps),
             reason="Viewclients vc.dump() doesn't work when the 'you are in fullscreen'-popup is stealing focus, HMI to fix",
             jira_issue="",
             deadline="2017-09-28"
    ),
    Disabled(VTSTest("vendor/volvocars/hardware/netman/test/ct/blacklist",    standard_caps),
             reason="Blacklist functionality not yet implemented",
             jira_issue="",
             deadline="2017-10-12"
    ),
    VTSTest("vendor/volvocars/hardware/infotainmentIpBus/test/ut",            standard_caps),
    VTSTest("vendor/volvocars/hardware/infotainment-ip-service/test/ct",      standard_caps),
    VTSTest("vendor/volvocars/hardware/signals/dataelements/test/ut",         standard_caps),
    VTSTest("vendor/volvocars/hardware/tarmac/eventloop/test/ct",             standard_caps),
]

test_plan_hourly = [
    # Climate included twice because the test case dynamically detects if FR hardware is present
    VTSTest("vendor/volvocars/hardware/climate/test/ct",                      standard_caps | {cp.flexray}),
    VTSTest("vendor/volvocars/hardware/localconfig/test/ct",                  standard_caps),
    VTSTest("vendor/volvocars/hardware/localconfig/test/ut",                  standard_caps),
    VTSTest("vendor/volvocars/hardware/netman/test/ct/ip_configuration",      standard_caps),
    VTSTest("vendor/volvocars/hardware/netman/test/ct/iptables_configuration",standard_caps),
    VTSTest("vendor/volvocars/hardware/netman/test/ct/firewall_configuration",standard_caps),
    VTSTest("vendor/volvocars/hardware/netman/test/ct/namespace_configuration",standard_caps),
    VTSTest("vendor/volvocars/hardware/signals/vehiclesignalsdaemon/test/ut", standard_caps),
    VTSTest("vendor/volvocars/hardware/signals/dataelements/test/ct/sendAndReceiveOneSignal", standard_caps | {cp.flexray}),
]

test_plan_nightly = [

]
