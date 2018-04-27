# Copyright 2017-2018 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

from shipit.test_runner.test_types import AndroidVTS, VTSTest, TradefedTest, Disabled, standard_caps, Capabilities as cp

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
    VTSTest("vendor/volvocars/tools/test/daemon_test",                              standard_caps), #VtsDaemonTest, Krzysztof Wesolowski
    VTSTest("vendor/volvocars/hardware/ci/test/ct/smoketest",                       standard_caps,
            tests_to_run=["testCpuLoadShort", "testMemory", "testDisk"]),           #VtsCiSmokeTest, Kenneth Karlsson
    VTSTest("vendor/volvocars/hardware/pac/libdbg/test/ut",                         standard_caps), #libdbgTest, Carl Sjoberg
    VTSTest("vendor/volvocars/hardware/signals/dataelements/test/ut",               standard_caps), #VtsDEUnitTest, Magnus Larsson
    VTSTest("vendor/volvocars/hardware/tarmac/eventloop/test/ct",                   standard_caps), #VtsEventLoopCTTestCases, Magnus Larsson
    VTSTest("vendor/volvocars/hardware/localconfig/test/ut",                        standard_caps), #liblocalconfig_ut, Magnus Larsson
    VTSTest("vendor/volvocars/hardware/netman/test/ct/ip_configuration",            standard_caps), #VtsNetmandIpConfigurationComponentTest, Magnus Larsson
    VTSTest("vendor/volvocars/hardware/netman/test/ct/iptables",                    standard_caps), #VtsNetmandIptablesComponentTest, Magnus Larsson
    VTSTest("vendor/volvocars/hardware/netman/test/ct/namespace_configuration",     standard_caps), #VtsNetmandNamespaceComponentTest, Magnus Larsson
    VTSTest("vendor/volvocars/hardware/uds/test/ct",                                standard_caps), #udsdatacollector_ct, Krzysztof Wesolowski
    VTSTest("vendor/volvocars/tools/test/ui_performance/volvolauncher",             standard_caps), #GeneralPerformanceTest, Magnus Larsson
    VTSTest("vendor/volvocars/hardware/carconfig/carconfig/test/ut",                standard_caps), #Carconfig test, Johnny Karlsson
]

test_plan_hourly = test_plan_gate + [
    VTSTest("vendor/volvocars/hardware/dim/test/ct/apix",                           standard_caps | {cp.apix}), #Erik Dahlgren
    VTSTest("vendor/volvocars/hardware/pac/evs/hardware/test/ut",                   standard_caps), #VtsHalPacEvsHardware, Janne Petkovski
    VTSTest("vendor/volvocars/hardware/pac/evs/manager/test/ut",                    standard_caps), #VtsHalPacEvsManager, Mikael Olsson
    VTSTest("vendor/volvocars/hardware/pac/libpacconfig/test/ut",                   standard_caps), #VtsLibPacConfig, Mikael Olsson
    VTSTest("vendor/volvocars/hardware/pac/early_app/test/ut",                      standard_caps), #VtsPacEarlyApp, Staffan Rydén
    VTSTest("vendor/volvocars/hardware/pac/test/example/ut",                        standard_caps), #VtsPacExampleTest, Mikael Olsson
    VTSTest("vendor/volvocars/hardware/infotainmentIpBus/test/ut",                  standard_caps), #Martin Hansson
    VTSTest("vendor/volvocars/hardware/localconfig/test/ct/localconfig_nativelib",  standard_caps), #RRAJAGO1
    VTSTest("vendor/volvocars/hardware/netman/test/ct/apix_obd_traffic_split",      standard_caps), #Samuel Idowu
    VTSTest("vendor/volvocars/hardware/netman/test/ct/boot",                        standard_caps), #Uguudei
    VTSTest("vendor/volvocars/hardware/netman/test/ct/kernel_ip_stack",             standard_caps | {cp.audio}), #Uguudei
    VTSTest("vendor/volvocars/hardware/netman/test/ct/most_interface",              standard_caps | {cp.audio}), #Robin Touche
    VTSTest("vendor/volvocars/hardware/netman/test/ct/uds_diagnostics_test",        standard_caps), #Uguudei
    VTSTest("vendor/volvocars/hardware/tarmac/common/test/ct",                      standard_caps), #Torbjorn Sandsgard
    VTSTest("vendor/volvocars/hardware/profiles/test/user_profile",                 standard_caps | {cp.cem}), #Uguudei
    VTSTest("vendor/volvocars/hardware/soundnotifications/test/ut",                 standard_caps), #Johan Olsson
    VTSTest("vendor/volvocars/hardware/remotectrl/test/it/audioctrl_tests",         standard_caps), #Abhijeet Shirolikar
]

test_plan_nightly = test_plan_hourly + [
    VTSTest("vendor/volvocars/hardware/ci/test/ct/smoketest", standard_caps,
            tests_to_run=["testCpuLoadLong", "testMemory", "testDisk", "testCrashes"]),    # VtsCiSmokeTest, Kenneth Karlsson
]

#Put stable tests here so they can be tested often and then added to our gates
test_plan_incubator_hourly = [
    VTSTest("vendor/volvocars/hardware/ci/test/ct/smoketest_with_boot_time",        standard_caps), # Chrsitoffer Östlin
    TradefedTest("vendor/volvocars/packages/SoftwareUpdateService/test/ct/test_suites/HappyGetAssignments",          standard_caps), #Patrik Ingmarsson
    TradefedTest("vendor/volvocars/packages/SoftwareUpdateService/test/ct/test_suites/HappyGetDownloads",            standard_caps), #Matilda Bengtsson
    TradefedTest("vendor/volvocars/packages/SoftwareUpdateService/test/ct/test_suites/HappyAutoCommission",            standard_caps), #Matilda Bengtsson
    VTSTest("vendor/volvocars/packages/BrightnessService/test/ct/ScreenCleaning",   standard_caps), #Tobias Ohrstrom
    VTSTest("vendor/volvocars/hardware/vehicle/test/lane_keeping_aid_setting", standard_caps | {cp.flexray}), #Martin Stigels
    VTSTest("vendor/volvocars/hardware/carconfig/test/ct",                          standard_caps | {cp.flexray}), #Joel Gustafsson
    VTSTest("vendor/volvocars/hardware/signals/dataelements/test/ct/sendAndReceiveOneSignal", standard_caps | {cp.flexray}), #Torbjorn Sandsgard
    VTSTest("vendor/volvocars/hardware/vehicle/test/dai_setting",                   standard_caps | {cp.flexray}), #Uguudei
    VTSTest("vendor/volvocars/hardware/signals/dataelements/test/ct/toggleTest", standard_caps | {cp.flexray}), #Erik Elmeke
    VTSTest("vendor/volvocars/hardware/signals/dataelements/test/ct/flexray_burst_test", standard_caps | {cp.flexray}), #Erik Elmeke
    VTSTest("vendor/volvocars/hardware/test/soundnotifications",                    standard_caps | {cp.flexray}), #Hitesh Garg
]

#This is for unstable tests that does not need to be run often
test_plan_staging_daily = [
    TradefedTest("vendor/volvocars/hardware/audio/test/ct/audio_loopback/app",      standard_caps | {cp.audio}), #Robin Touche
    Disabled(TradefedTest("vendor/volvocars/hardware/audio/test/ct/audio_mic_control/app", standard_caps | {cp.audio}), #Robin Touche
             reason="Microphone HAL not fully implemented. Currently no ETA from Aptiv",
             jira_issue="PSS370-15982",
             deadline="2018-04-28"
    ),
    TradefedTest("vendor/volvocars/hardware/carconfig/carconfig_java_lib/test/ct/carconfig_api", standard_caps), #Uguudei
    VTSTest("vendor/volvocars/hardware/vehicle/test/climate_firstrow/ct",           standard_caps), #Joel Gustafsson
    VTSTest("vendor/volvocars/hardware/vehicle/test/climate_firstrow/ct",           standard_caps | {cp.flexray}), #Krzysztof Wesolowski
    VTSTest("vendor/volvocars/hardware/ipcbd/test/ct",                              standard_caps), #Andreas Bengtsson
    TradefedTest("vendor/volvocars/hardware/localconfig/test/ct/localconfig_java",  standard_caps), #lveeraku
    VTSTest("vendor/volvocars/hardware/powermoding/test/ct",                        standard_caps | {cp.flexray}), #Mats Berggrund
    VTSTest("vendor/volvocars/hardware/profiles/test/ct",                           standard_caps), #Torbjorn Sandsgard
    VTSTest("vendor/volvocars/hardware/settings/test/ct",                           standard_caps), #Erik Elmeke
    VTSTest("vendor/volvocars/hardware/signals/dataelements/test/ct/array_spam",    standard_caps | {cp.flexray}), #Erik Elmeke
    VTSTest("vendor/volvocars/hardware/signals/vehiclesignalsdaemon/test/ut",       standard_caps), #Uguudei
    VTSTest("vendor/volvocars/hardware/test/gnss",                                  standard_caps), #Andreas Bengtsson
    VTSTest("vendor/volvocars/hardware/test/iplm",                                  standard_caps), #Andreas Bengtsson
    VTSTest("vendor/volvocars/hardware/test/keypanel",                              standard_caps | {cp.flexray}), #Tobias Ohrstrom
    VTSTest("vendor/volvocars/hardware/vehicle/test/connectedsafety_setting",       standard_caps | {cp.flexray}), #Uguudei
    VTSTest("vendor/volvocars/hardware/vehicle/test/Vhal_SmokeTest",                standard_caps), #Praveen Kumar Khatri
    TradefedTest("vendor/volvocars/hardware/netman/test/ct/usb2ethernet",           standard_caps), #Uguudei
    VTSTest("vendor/volvocars/hardware/vehicle/test/speed_limit_adaptation", standard_caps | {cp.flexray}), #Justin Saler
    VTSTest("vendor/volvocars/hardware/netman/test/ct/rtp_audio_namespace_bridge", standard_caps), #Samuel Idowu
    VTSTest("vendor/volvocars/hardware/remotectrl/test/it/climatectrl_tests", standard_caps | {cp.flexray}), #Abhijeet Shirolikar

    # The following test will set the screen-always-on to ON which will prevent the screen to power off.
    # If your test relies on the screen to power off, either put it before this test or set
    # screen-always-on to OFF in your AndroidTest.xml. N.B., resetting of this flag will trigger
    # a reboot.
    TradefedTest("vendor/volvocars/packages/BrightnessService/test/ct/BrightnessServiceTests",          standard_caps), #Tobias Ohrstrom
    TradefedTest("vendor/volvocars/packages/BrightnessService/test/ut/BrightnessServiceTests",          standard_caps), #Tobias Ohrstrom
    TradefedTest("vendor/volvocars/packages/CloudService/test/ut/services",                             standard_caps), #Patrik Ingmarsson
    TradefedTest("vendor/volvocars/tools/test/log_analyzer",                                            standard_caps), #Erik Elmeke
    TradefedTest("vendor/volvocars/packages/ConnectivityManager/test/ct/ConnectivityManagerRelayApp",   standard_caps), #Patrik Ingmarsson
    VTSTest("vendor/volvocars/hardware/vehicle/test/climate_firstrow/ut",           standard_caps), #Joel Gustafsson
    AndroidVTS("VtsHalAudioV2_0Target",                                             standard_caps), #Gustav Svensson
    AndroidVTS("VtsHalBroadcastradioV1_0Target",                                    standard_caps), #Gustav Svensson
    AndroidVTS("VtsHalMediaOmxV1_0Host",                                            standard_caps), #Gustav Svensson
]
