set(GTEST_RUN_FLAGS --gtest_color=yes --gtest_repeat=20 --gtest_shuffle STRING "Flags passed to GTest")

include_directories(${GTEST_GMOCK_DIR}
    ${CARCONFIG_UPDATER_SRC_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/stubs
    ${CMAKE_CURRENT_SOURCE_DIR}/mocks
    ${CMAKE_CURRENT_SOURCE_DIR})

ADD_DEFINITIONS(-D_TESTFOLDER="${PROJECT_SOURCE_DIR}/test/ut/")

add_executable(carconfig_updater_test
    carconfig_updater_test.cpp
    ${CARCONFIG_UPDATER_SRC_DIR}/carconfig_updater.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/mocks/mock_diagnostic_client.cpp
    ${CARCONFIG_UPDATER_SRC_DIR}/restart_client.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}//mocks/mock_vipcom_client.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/stubs/vipcom/vipcom_api.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/stubs/CommonAPI_stub.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/stubs/ctime.c
    ${CMAKE_CURRENT_SOURCE_DIR}/stubs/dataelements/gen_dataelements.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/stubs/sd-bus.c
    ${GTEST_GMOCK_DIR}/gmock-gtest-all.cc)

add_executable(diagnostics_client_test
    diagnostics_client_test.cpp
    ${CARCONFIG_UPDATER_SRC_DIR}/diagnostics_client.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/stubs/CommonAPI_stub.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/stubs/v1/com/ihu/Diagnostics/DiagnosticsProxy.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/stubs/ctime.c
    ${GTEST_GMOCK_DIR}/gmock-gtest-all.cc)

add_executable(restart_client_test
    restart_client_test.cpp
    ${CARCONFIG_UPDATER_SRC_DIR}/restart_client.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/stubs/sd-bus.c
    ${GTEST_GMOCK_DIR}/gmock-gtest-all.cc)

add_executable(vipcom_client_test
    vipcom_client_test.cpp
    ${CARCONFIG_UPDATER_SRC_DIR}/vipcom_client.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/stubs/vipcom/vipcom_api.cpp
    ${GTEST_GMOCK_DIR}/gmock-gtest-all.cc)

target_link_libraries(carconfig_updater_test -lpthread)
target_link_libraries(diagnostics_client_test -lpthread)
target_link_libraries(restart_client_test -lpthread)
target_link_libraries(vipcom_client_test -lpthread)

add_test(CarconfigUpdaterTest carconfig_updater_test ${GTEST_RUN_FLAGS})
add_test(RestartClientTest restart_client_test ${GTEST_RUN_FLAGS})
add_test(VipComClientTest vipcom_client_test ${GTEST_RUN_FLAGS})
