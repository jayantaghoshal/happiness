set(GTEST_RUN_FLAGS --gtest_color=yes --gtest_repeat=20 --gtest_shuffle CACHE STRING "Flags passed to GTest")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g")
add_definitions(-DUNIT_TEST)

include_directories(${vehiclesignalsmanager_SOURCE_DIR}/vehiclesignalsmanager/src
    ${CMAKE_CURRENT_SOURCE_DIR}/stubs
    ${CMAKE_CURRENT_SOURCE_DIR}/mocks
    ${vehiclesignalsmanager_SOURCE_DIR}/test-utils/gtest_gmock_1.7.0)

link_directories()

#
# vsm_test
#
add_executable(vsm_test
    vsm_test.cpp
    mocks/vsm_inject_mock.cpp
    mocks/pthread_mock.cpp
    mocks/misc_mock.cpp
    ${vehiclesignalsmanager_SOURCE_DIR}/vehiclesignalsmanager/src/vsm.cpp
    ${vehiclesignalsmanager_SOURCE_DIR}/test-utils/gtest_gmock_1.7.0/gmock-gtest-all.cc)

target_link_libraries(vsm_test
    -lpthread)

add_test(VehicleSignalsManagerTest vsm_test ${GTEST_RUN_FLAGS})

#
# message_inject_test
#
add_executable(message_inject_test
    message_inject_test.cpp
    mocks/vsm_inject_mock.cpp
    mocks/vsm_sink_mock.cpp
    ${vehiclesignalsmanager_SOURCE_DIR}/vehiclesignalsmanager/src/message_inject.cpp
    ${vehiclesignalsmanager_SOURCE_DIR}/test-utils/gtest_gmock_1.7.0/gmock-gtest-all.cc)

target_link_libraries(message_inject_test
    -lpthread)

add_test(MessageInjectTest message_inject_test ${GTEST_RUN_FLAGS})

#
# dbus server test
#
add_executable(dbus_server_test
    dbus_server_test.cpp
    ${vehiclesignalsmanager_SOURCE_DIR}/vehiclesignalsmanager/src/sd_bus_signal_server.cpp
    ${vehiclesignalsmanager_SOURCE_DIR}/vehiclesignalsmanager/src/signallog.cpp
    ${vehiclesignalsmanager_SOURCE_DIR}/vehiclesignalsmanager/src/vsm_dbus_vtable_signals.c
    ${vehiclesignalsmanager_SOURCE_DIR}/test-utils/gtest_gmock_1.7.0/gmock-gtest-all.cc
    mocks/systemd/systemd_mock.cpp
    stubs/VipFramework_stub.cpp)
    

target_link_libraries(dbus_server_test
    -lpthread)

add_test(DBusServerTest dbus_server_test ${GTEST_RUN_FLAGS})
