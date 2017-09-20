set(GTEST_RUN_FLAGS --gtest_color=yes STRING "Flags passed to GTest")

include_directories(
  ${GTEST_GMOCK_DIR}
  ${CORE_SRC_DIR}
  ${CORE_INCLUDE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/stubs
  ${CMAKE_CURRENT_SOURCE_DIR}/mocks
)

add_executable(cedric_reset_test
    cedric_reset_test.cpp
    ${CORE_SRC_DIR}/cedric_reset.cpp
    mocks/systemd/systemd_mock.cpp
    ${GTEST_GMOCK_DIR}/gmock-gtest-all.cc)

add_executable(cedric_localconfig_test
    cedric_localconfig_test.cpp
    ${CORE_SRC_DIR}/cedric_localconfig.cpp
    ${GTEST_GMOCK_DIR}/gmock-gtest-all.cc)

add_executable(cedric_hwid_test
    cedric_hwid_test.cpp
    ${CORE_SRC_DIR}/cedric_hwid.cpp
    ${GTEST_GMOCK_DIR}/gmock-gtest-all.cc)

add_executable(cedric_vipclockdomain_test
    cedric_vipclockdomain_test.cpp
    ${CORE_SRC_DIR}/cedric_vipclockdomain.cpp
    ${GTEST_GMOCK_DIR}/gmock-gtest-all.cc)

add_executable(cedric_file_monitor_test
    cedric_file_monitor_test.cpp
    ${CORE_SRC_DIR}/cedric_file_monitor.cpp
    ${GTEST_GMOCK_DIR}/gmock-gtest-all.cc)
  
add_executable(cedric_nodestate_test
    cedric_nodestate_test.cpp
    ${CORE_SRC_DIR}/cedric_nodestate.cpp
    ${CORE_SRC_DIR}/cedric_nodestate_vtable.c
    mocks/systemd/systemd_mock.cpp
    ${GTEST_GMOCK_DIR}/gmock-gtest-all.cc)

target_link_libraries(cedric_reset_test -lpthread)
target_link_libraries(cedric_localconfig_test -lpthread)
target_link_libraries(cedric_hwid_test -lpthread)
target_link_libraries(cedric_vipclockdomain_test -lpthread)
target_link_libraries(cedric_file_monitor_test -lpthread)
target_link_libraries(cedric_nodestate_test -lpthread)

add_test(CedricResetTest cedric_reset_test ${GTEST_RUN_FLAGS})
add_test(CedricLocalconfigTest cedric_localconfig_test ${GTEST_RUN_FLAGS})
add_test(CedricHwidTest cedric_hwid_test ${GTEST_RUN_FLAGS})
add_test(CedricVipClockDomain cedric_vipclockdomain_test ${GTEST_RUN_FLAGS})
add_test(CedricFileMonitorTest cedric_file_monitor_test ${GTEST_RUN_FLAGS})
add_test(CedricNodeState cedric_nodestate_test ${GTEST_RUN_FLAGS})
