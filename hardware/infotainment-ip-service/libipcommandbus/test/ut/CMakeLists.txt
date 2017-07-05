cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project(testLibIpCommandBus)

set(REPOSITORY_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../..)

# Include shared cmake stuff
include(${REPOSITORY_ROOT}/cmake/shared.cmake)

# Add test_utils with gtest and gmock
add_subdirectory(${REPOSITORY_ROOT}/test_utils test_utils)
set(test_utils_SOURCE_DIR ${REPOSITORY_ROOT}/test_utils)

if(NOT DEFINED COMPONENT_SOURCE_DIR)
    set(COMPONENT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../..)
endif()

find_package(PkgConfig REQUIRED)
pkg_check_modules(IVI-LOGGING REQUIRED ivi-logging)
enable_testing()

include_directories(${COMPONENT_SOURCE_DIR}/include
    ${COMPONENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/stubs
    ${CMAKE_CURRENT_SOURCE_DIR}/mocks
    ${test_utils_SOURCE_DIR}/gmock-gtest    
    ${IVI-LOGGING_INCLUDE_DIRS}
)
include_directories(SYSTEM
    ${IVI-LOGGING_INCLUDE_DIRS}
)
link_directories(
    ${IVI-LOGGING_LIBRARY_DIRS}
)


# Stubs in separate lib
add_library(stubs STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/stubs/cedric/cedric_localconfig.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/stubs/connectivity-sd/bus_references.h
    ${CMAKE_CURRENT_SOURCE_DIR}/stubs/connectivity-sd/common.h
    ${CMAKE_CURRENT_SOURCE_DIR}/stubs/connectivity-sd/event_references.h
    ${CMAKE_CURRENT_SOURCE_DIR}/stubs/connectivity-sd/mock_thread_dispatcher.h
    ${CMAKE_CURRENT_SOURCE_DIR}/stubs/connectivity-sd/mock_time_provider.h
    ${CMAKE_CURRENT_SOURCE_DIR}/stubs/connectivity-sd/mock_time_provider.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/stubs/connectivity-sd/sd_event_thread.h
    ${CMAKE_CURRENT_SOURCE_DIR}/stubs/connectivity-sd/thread_dispatcher.h
    ${CMAKE_CURRENT_SOURCE_DIR}/stubs/connectivity-sd/time_provider.h
    ${CMAKE_CURRENT_SOURCE_DIR}/stubs/connectivity-sd/time_provider.cpp
)

# Mocks in separate lib
add_library(mocks STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/mocks/MockIdiagnosticsClient.h
    ${CMAKE_CURRENT_SOURCE_DIR}/mocks/MockUdpSocket.h
)
set_target_properties(mocks PROPERTIES LINKER_LANGUAGE CXX) #Temp fix as mocks don't have any CPP files now,

target_include_directories(mocks PRIVATE ${REPOSITORY_ROOT}/libasn1/include)

add_executable(${PROJECT_NAME}_testTransportServices
    ${COMPONENT_SOURCE_DIR}/src/Pdu.cpp
    ${COMPONENT_SOURCE_DIR}/src/Message.cpp
    ${COMPONENT_SOURCE_DIR}/src/TimeoutInfo.cpp
    ${COMPONENT_SOURCE_DIR}/src/TransportServices.cpp
    ${COMPONENT_SOURCE_DIR}/src/log_context.cpp
    ${COMPONENT_SOURCE_DIR}/src/net_deserializer.cpp
    ${COMPONENT_SOURCE_DIR}/src/net_serializer.cpp
    ${COMPONENT_SOURCE_DIR}/src/vcc_pdu_header.cpp
    ${COMPONENT_SOURCE_DIR}/src/VccIpCmdApi.cpp
    ${testLibIpCommandBus_SOURCE_DIR}/stubs/LocalconfigMessageTimeoutsStub.cpp
    test_net_deserializer.cpp
    test_net_serializer.cpp
    test_vcc_pdu_header.cpp
    testTransportServices.cpp
)

target_link_libraries(${PROJECT_NAME}_testTransportServices
    googletest
    mocks
    stubs
    ${IVI-LOGGING_LIBRARIES}
)

add_test(NAME ${PROJECT_NAME}_testTransportServices COMMAND ${PROJECT_NAME}_testTransportServices)


add_executable(${PROJECT_NAME}_testMessageDispatcher
    ${COMPONENT_SOURCE_DIR}/src/Pdu.cpp
    ${COMPONENT_SOURCE_DIR}/src/Message.cpp
    ${COMPONENT_SOURCE_DIR}/src/MessageDispatcher.cpp
    ${COMPONENT_SOURCE_DIR}/src/TimeoutInfo.cpp
    ${COMPONENT_SOURCE_DIR}/src/log_context.cpp
    ${COMPONENT_SOURCE_DIR}/src/net_deserializer.cpp
    ${COMPONENT_SOURCE_DIR}/src/net_serializer.cpp
    ${COMPONENT_SOURCE_DIR}/src/vcc_pdu_header.cpp
    ${COMPONENT_SOURCE_DIR}/src/VccIpCmdApi.cpp
    ${COMPONENT_SOURCE_DIR}/src/local_config_parameters.cpp
    testMessageDispatcher.cpp
)

target_include_directories(${PROJECT_NAME}_testMessageDispatcher
    PRIVATE ${REPOSITORY_ROOT}/libasn1/include
)
target_include_directories(${PROJECT_NAME}_testMessageDispatcher SYSTEM
    PRIVATE ${IVI-LOGGING_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME}_testMessageDispatcher
    googletest
    mocks
    stubs
    ${IVI-LOGGING_LIBRARIES}
)

add_test(NAME ${PROJECT_NAME}_testMessageDispatcher COMMAND ${PROJECT_NAME}_testMessageDispatcher)


# Test TimeoutInfo
add_executable(test_timeoutinfo
    ${COMPONENT_SOURCE_DIR}/src/TimeoutInfo.cpp
    ${COMPONENT_SOURCE_DIR}/src/local_config_parameters.cpp
    ${COMPONENT_SOURCE_DIR}/src/VccIpCmdApi.cpp
    test_timeoutinfo.cpp
    

)

target_link_libraries(test_timeoutinfo
    googletest
    stubs
    ${IVI-LOGGING_LIBRARIES}
)

add_test(NAME test_timeoutinfo COMMAND test_timeoutinfo)

# Test LocalconfigMessageTimeouts
add_executable(test_localconfig_parameters
    ${COMPONENT_SOURCE_DIR}/src/local_config_parameters.cpp
    ${COMPONENT_SOURCE_DIR}/src/VccIpCmdApi.cpp
    test_localconfig_parameters.cpp

    # Stubs/mocks
    mocks/cedric/mock_cedric_localconfig.cpp
    mocks/cedric/mock_cedric_localconfig.h
)

target_link_libraries(test_localconfig_parameters 
    googletest 
    stubs
    ${IVI-LOGGING_LIBRARIES}
)

add_test(NAME test_localconfig_parameters COMMAND test_localconfig_parameters)
