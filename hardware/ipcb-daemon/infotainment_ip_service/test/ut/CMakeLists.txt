cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project(testInfotainment_ip_service)

set(REPOSITORY_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../..)

# Add test_utils with gtest and gmock
add_subdirectory(${REPOSITORY_ROOT}/test_utils test_utils)
set(test_utils_SOURCE_DIR ${REPOSITORY_ROOT}/test_utils)

if(NOT DEFINED COMPONENT_SOURCE_DIR)
	set(COMPONENT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../..)
endif()

if(NOT DEFINED LIBIP_COMMAND_BUS_DIR)
    set(LIBIP_COMMAND_BUS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../libipcommandbus)
endif()

if(NOT DEFINED LIBASN1_DIR)
    set(LIBASN1_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../libasn1)
endif()

find_package(PkgConfig REQUIRED)
find_package(CommonAPI REQUIRED)


find_package(PkgConfig REQUIRED)
pkg_check_modules(IVI-LOGGING REQUIRED ivi-logging)

# Build a REGULAR libasn1.
# I.e. avoid involving its own unit tests and possible path conflicts.
# Will _not_ affect our coverage values since path to libasn1 is outside the test root dir.
unset (ENABLE_COVERAGE CACHE)
unset (ENABLE_TESTS CACHE)

add_subdirectory( ${LIBASN1_DIR} ${CMAKE_CURRENT_BINARY_DIR}/libasn1 )

# And now enable unit testing!
set (ENABLE_COVERAGE 1)
set (ENABLE_TESTS 1)
enable_testing()

# Include shared cmake stuff
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/shared.cmake)

# Necessary for CommonAPI includes when used in this context...
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-ignored-qualifiers")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-parameter")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/mocks
    ${CMAKE_CURRENT_SOURCE_DIR}/stubs
    ${CMAKE_CURRENT_SOURCE_DIR}/stubs/common_api
    ${test_utils_SOURCE_DIR}/gmock-gtest
    ${COMPONENT_SOURCE_DIR}/src
    ${LIBIP_COMMAND_BUS_DIR}/include
    ${LIBASN1_DIR}/include
)

include_directories(SYSTEM
    ${COMMONAPI_INCLUDE_DIRS}
    ${IVI-LOGGING_INCLUDE_DIRS}
)

link_directories(
    ${IVI-LOGGING_LIBRARY_DIRS}
)

# ==========================================================================================
# Common libraries
add_library(libipcommandbus_partial
            STATIC
            ${LIBIP_COMMAND_BUS_DIR}/src/Pdu.cpp
            ${LIBIP_COMMAND_BUS_DIR}/include/ipcommandbus/Pdu.h
            ${LIBIP_COMMAND_BUS_DIR}/src/VccIpCmdApi.cpp
            ${LIBIP_COMMAND_BUS_DIR}/include/ipcommandbus/VccIpCmdApi.h
            ${LIBIP_COMMAND_BUS_DIR}/src/vcc_pdu_header.cpp
            ${LIBIP_COMMAND_BUS_DIR}/include/ipcommandbus/vcc_pdu_header.h
            ${LIBIP_COMMAND_BUS_DIR}/src/net_serializer.cpp
            ${LIBIP_COMMAND_BUS_DIR}/include/ipcommandbus/net_serializer.h
            ${LIBIP_COMMAND_BUS_DIR}/src/net_deserializer.cpp
            ${LIBIP_COMMAND_BUS_DIR}/include/ipcommandbus/net_deserializer.h
            ${LIBIP_COMMAND_BUS_DIR}/src/Message.cpp
            ${LIBIP_COMMAND_BUS_DIR}/include/ipcommandbus/Message.h
            ${LIBIP_COMMAND_BUS_DIR}/src/TimeoutInfo.cpp
            ${LIBIP_COMMAND_BUS_DIR}/include/ipcommandbus/TimeoutInfo.h
            ${LIBIP_COMMAND_BUS_DIR}/src/local_config_parameters.cpp
            ${LIBIP_COMMAND_BUS_DIR}/include/ipcommandbus/local_config_parameters.h)



add_library(libtype-conversion-helper-stubs
            STATIC
            ${CMAKE_CURRENT_SOURCE_DIR}/stubs/util/type_conversion_helpers.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/stubs/util/type_conversion_helpers.h)


find_package(Qt5Core REQUIRED)
add_library(libdataelements-stubs
            STATIC
            stubs/dataelements/Application_dataelement.h
            stubs/dataelements/ECD_dataelement.h
            stubs/dataelements/dataelementcommbus_standalone.cpp
            stubs/dataelements/dataelementframework.cpp
            stubs/dataelements/dataelemvalue.cpp
            stubs/dataelements/internalsignals.cpp
            stubs/dataelements/gen_dataelements.cpp
            stubs/dataelements/gen_jsonencdec.cpp)
target_include_directories(libdataelements-stubs SYSTEM PRIVATE
    ${Qt5Core_INCLUDE_DIRS} #Required for QByteArray (#TODO: Maybe there is a new stub of DataElements available, DataElements had a major rework dec 2016)
)
target_link_libraries(libdataelements-stubs
    ${Qt5Core_LIBRARIES}
)

add_library(libexternal-mocks
            STATIC
            ${LIBIP_COMMAND_BUS_DIR}/src/Message.cpp
            mocks/mock_ip_service.cpp
            mocks/mock_ip_service.h
            mocks/MockLibSdBus.cpp
            mocks/MockLibSdBus.h
            mocks/MockMessageDispatcher.cpp
            mocks/MockMessageDispatcher.h
            mocks/cedric/mock_cedric_localconfig.cpp
            mocks/cedric/mock_cedric_localconfig.h)

# ==========================================================================================
# Test Wlan service
add_executable(test_wlan_service
    # Files under test:
    ${COMPONENT_SOURCE_DIR}/src/services/iwlan_service.h
    ${COMPONENT_SOURCE_DIR}/src/services/wlan_service.cpp
    ${COMPONENT_SOURCE_DIR}/src/services/wlan_service.h

    # Stubs/mocks:
    ${CMAKE_CURRENT_SOURCE_DIR}/test_wlan_service.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/mocks/mock_wlan_service_stub.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/mocks/mock_wlan_service_stub.h
    ${CMAKE_CURRENT_SOURCE_DIR}/stubs/ipcommandbus/TimeoutInfo.cpp

    # Regular helper classes.
)

target_link_libraries(
    test_wlan_service

    # Deps
    googletest
    asn1
    libipcommandbus_partial
    ${IVI-LOGGING_LIBRARIES}
    libtype-conversion-helper-stubs
    libexternal-mocks
    CommonAPI)

add_test(NAME test_wlan_service COMMAND test_wlan_service)

# ==========================================================================================

# Test Rvdc client
add_executable(test_rvdc_client
    ${CMAKE_CURRENT_SOURCE_DIR}/test_rvdc_client.cpp
    # Files under test:
    ${COMPONENT_SOURCE_DIR}/src/services/rvdc_client.cpp
    ${COMPONENT_SOURCE_DIR}/src/IpService.cpp

    # Stubs/mocks:
    ${CMAKE_CURRENT_SOURCE_DIR}/mocks/MockMessageDispatcher.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/mocks/cedric/mock_cedric_localconfig.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/stubs/util/type_conversion_helpers.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/stubs/util/type_conversion_helpers.h

    ${LIBIP_COMMAND_BUS_DIR}/src/Message.cpp
    ${LIBIP_COMMAND_BUS_DIR}/src/Pdu.cpp
    ${LIBIP_COMMAND_BUS_DIR}/include/ipcommandbus/Pdu.h
    ${LIBIP_COMMAND_BUS_DIR}/src/VccIpCmdApi.cpp
    ${LIBIP_COMMAND_BUS_DIR}/include/ipcommandbus/VccIpCmdApi.h
    ${LIBIP_COMMAND_BUS_DIR}/src/vcc_pdu_header.cpp
    ${LIBIP_COMMAND_BUS_DIR}/include/ipcommandbus/vcc_pdu_header.h
    ${LIBIP_COMMAND_BUS_DIR}/src/net_serializer.cpp
    ${LIBIP_COMMAND_BUS_DIR}/include/ipcommandbus/net_serializer.h
    ${LIBIP_COMMAND_BUS_DIR}/src/net_deserializer.cpp
    ${LIBIP_COMMAND_BUS_DIR}/include/ipcommandbus/net_deserializer.h
    ${LIBIP_COMMAND_BUS_DIR}/src/TimeoutInfo.cpp
    ${LIBIP_COMMAND_BUS_DIR}/include/ipcommandbus/TimeoutInfo.h
    ${LIBIP_COMMAND_BUS_DIR}/src/local_config_parameters.cpp
)

target_link_libraries(
    test_rvdc_client

    # Deps
    googletest
    asn1
    ${IVI-LOGGING_LIBRARIES}
    libdataelements-stubs
    libexternal-mocks

    CommonAPI)

add_test(NAME test_rvdc_client COMMAND test_rvdc_client)

# ==========================================================================================
# Test type conversion helpers
add_executable(test_type_conversion_helpers
    # Files under test:
    ${COMPONENT_SOURCE_DIR}/src/util/type_conversion_helpers.cpp
    ${COMPONENT_SOURCE_DIR}/src/util/type_conversion_helpers.h
    ${CMAKE_CURRENT_SOURCE_DIR}/test_type_conversion_helpers.cpp
    ${COMMON_SOURCES}

    # Stubs/mocks:

    # Regular helper classes.
    # NOTE! MUST be outside coverage root, or under exclude folder to not mess up coverage values!
)

target_link_libraries(
    test_type_conversion_helpers

    # Deps
    googletest
    asn1
    ${IVI-LOGGING_LIBRARIES}
    CommonAPI
)

add_test(NAME test_type_conversion_helpers COMMAND test_type_conversion_helpers)

# ==========================================================================================
# Test gnss service
add_executable(test_gnss_service
    test_gnss_service.cpp
    # Files under test:
    ${COMPONENT_SOURCE_DIR}/src/services/gnss_service_stub.cpp
    ${COMPONENT_SOURCE_DIR}/src/services/gnss_service_stub.h
    ${COMPONENT_SOURCE_DIR}/src/services/gnss_service.cpp
    ${COMPONENT_SOURCE_DIR}/src/services/gnss_service.h

    # Stubs/mocks:
    mocks/mock_gnss_service_stub.h

    stubs/StubTransportServices.cpp
    stubs/ipcommandbus/TimeoutInfo.cpp
    stubs/connectivity-sd/time_provider.cpp
    stubs/connectivity-sd/mock_time_provider.cpp

    # Regular helper classes.
)

target_link_libraries(
    test_gnss_service

    # Deps
    googletest
    asn1
    ${IVI-LOGGING_LIBRARIES}
    libexternal-mocks
    CommonAPI)

add_test(NAME test_gnss_service COMMAND test_gnss_service)

# ==========================================================================================
# Test enhanced pos client
add_executable(test_enhanced_pos_client
    test_enhanced_pos_client.cpp
    # Files under test:
    ${COMPONENT_SOURCE_DIR}/src/services/enhanced_pos_client.cpp
    ${COMPONENT_SOURCE_DIR}/src/services/enhanced_pos_client.h
    ${LIBIP_COMMAND_BUS_DIR}/src/VccIpCmdApi.cpp
    ${LIBIP_COMMAND_BUS_DIR}/include/ipcommandbus/VccIpCmdApi.h

    # Stubs/mocks:
    stubs/ipcommandbus/Pdu.cpp
    stubs/ipcommandbus/Message.cpp
    stubs/ipcommandbus/TimeoutInfo.cpp
    stubs/stub_enhanced_position_proxy.cpp
    stubs/util/type_conversion_helpers.cpp    
    stubs/connectivity-sd/time_provider.cpp
    stubs/connectivity-sd/mock_time_provider.cpp

    # Regular helper classes.
)

target_link_libraries(
    test_enhanced_pos_client

    # Deps
    googletest
    asn1
    ${IVI-LOGGING_LIBRARIES}
    libexternal-mocks    
    libtype-conversion-helper-stubs
    CommonAPI)

add_test(NAME test_enhanced_pos_client COMMAND test_enhanced_pos_client)

# ==========================================================================================
# Test OTA service
add_executable(test_ota_service
    # Files under test:
    ${COMPONENT_SOURCE_DIR}/src/services/iota_service.h
    ${COMPONENT_SOURCE_DIR}/src/services/ota_service.cpp
    ${COMPONENT_SOURCE_DIR}/src/services/ota_service.h

    # Stubs/mocks:
    ${CMAKE_CURRENT_SOURCE_DIR}/test_ota_service.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/mocks/mock_ota_service_stub.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/mocks/mock_ota_service_stub.h
    ${CMAKE_CURRENT_SOURCE_DIR}/stubs/ipcommandbus/TimeoutInfo.cpp

    # Regular helper classes.
)

target_link_libraries(
    test_ota_service

    # Deps
    googletest
    asn1
    libipcommandbus_partial
    ${IVI-LOGGING_LIBRARIES}
    libtype-conversion-helper-stubs
    libexternal-mocks
    CommonAPI)

add_test(NAME test_ota_service COMMAND test_ota_service)

# ==========================================================================================
# Test iplm service
add_executable(test_iplm_service
    test_iplm_service.cpp
    # Files under test:
    ${COMPONENT_SOURCE_DIR}/src/services/iplm_service.cpp
    ${COMPONENT_SOURCE_DIR}/src/services/iplm_service.h

    # Source file helper classes
    ${LIBIP_COMMAND_BUS_DIR}/src/Message.cpp
    ${LIBIP_COMMAND_BUS_DIR}/include/ipcommandbus/Message.h
    ${LIBIP_COMMAND_BUS_DIR}/src/TimeoutInfo.cpp
    ${LIBIP_COMMAND_BUS_DIR}/include/ipcommandbus/TimeoutInfo.h
    ${LIBIP_COMMAND_BUS_DIR}/src/local_config_parameters.cpp
    ${LIBIP_COMMAND_BUS_DIR}/include/ipcommandbus/local_config_parameters.h
    ${LIBIP_COMMAND_BUS_DIR}/src/VccIpCmdApi.cpp
    ${LIBIP_COMMAND_BUS_DIR}/include/ipcommandbus/VccIpCmdApi.h

    # Stubs/mocks:
    mocks/mock_iplm_service_stub.cpp
    mocks/mock_iplm_service_stub.h
    stubs/ipcommandbus/Pdu.cpp
    stubs/NodeStateTypes.h
    stubs/cedric/cedric_localconfig.cpp
    stubs/stub_diagnostics_client.cpp
    stubs/cedric/cedric_nodestate.h
    stubs/util/local_config.cpp

    stubs/connectivity-sd/time_provider.cpp
    stubs/connectivity-sd/mock_time_provider.cpp

    # Regular helper classes.
)

target_include_directories(
    test_iplm_service
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/external_dep
)

target_link_libraries(
    test_iplm_service

    # Deps
    googletest
    asn1
    ${IVI-LOGGING_LIBRARIES}
    libdataelements-stubs
    libexternal-mocks
    libtype-conversion-helper-stubs

    CommonAPI
    )

add_test(NAME test_iplm_service COMMAND test_iplm_service)

# ==========================================================================================
# Test ota swlm service
add_executable(test_ota_swlm_service
    # Files under test:
    ${COMPONENT_SOURCE_DIR}/src/services/ota_swlm_service.h
    ${COMPONENT_SOURCE_DIR}/src/services/ota_swlm_service.cpp

    # Stubs/mocks:
    test_ota_swlm_service.cpp
    stubs/stub_ota_swlm_proxy.cpp
    stubs/ipcommandbus/local_config_parameters.cpp
    stubs/ipcommandbus/Message.cpp

    # Regular helper classes.
    ${LIBIP_COMMAND_BUS_DIR}/include/ipcommandbus/TimeoutInfo.h
)

target_link_libraries(
    test_ota_swlm_service

    # Functional libs.
    #log_context

    # Deps
    googletest
    asn1
    libipcommandbus_partial
    ${IVI-LOGGING_LIBRARIES}
    libtype-conversion-helper-stubs
    libexternal-mocks
    CommonAPI)

add_test(NAME test_ota_swlm_service COMMAND test_ota_swlm_service)


# ==========================================================================================
# Test diagnostics client
add_executable(test_diagnostics_client
    test_diagnostics_client.cpp
    # Files under test:
    ${COMPONENT_SOURCE_DIR}/src/services/diagnostics_client.cpp
    ${COMPONENT_SOURCE_DIR}/src/services/diagnostics_client.h
    ${LIBIP_COMMAND_BUS_DIR}/src/VccIpCmdApi.cpp
    ${LIBIP_COMMAND_BUS_DIR}/include/ipcommandbus/VccIpCmdApi.h

    # Stubs/mocks:
    stubs/stub_diagnostics_client_proxy.cpp
    stubs/ipcommandbus/TimeoutInfo.cpp
    stubs/cedric/cedric_localconfig.cpp
    stubs/stub_iplm_service_stub.cpp    
    mocks/mock_iplm_service.h

    # Regular helper classes.
    ${LIBIP_COMMAND_BUS_DIR}/src/Message.cpp
    ${LIBIP_COMMAND_BUS_DIR}/src/local_config_parameters.cpp
)

target_link_libraries(
    test_diagnostics_client

    # Deps
    googletest
    asn1
    ${IVI-LOGGING_LIBRARIES}
    libdataelements-stubs
    libexternal-mocks
    libtype-conversion-helper-stubs

    CommonAPI
)

add_test(NAME test_diagnostics_client COMMAND test_diagnostics_client)

add_executable(test_diagnostic_management
    test_diagnostic_management_service.cpp
    test_diagnostic_management_client.cpp
    # Files under test:
    ${COMPONENT_SOURCE_DIR}/src/services/diagnostic_management_client.h
    ${COMPONENT_SOURCE_DIR}/src/services/diagnostic_management_client.cpp
    ${COMPONENT_SOURCE_DIR}/src/services/diagnostic_management_interfaces.h
    ${COMPONENT_SOURCE_DIR}/src/services/diagnostic_management_service.h
    ${COMPONENT_SOURCE_DIR}/src/services/diagnostic_management_service.cpp
    ${COMPONENT_SOURCE_DIR}/src/services/diagnostic_management_stub.h
    ${COMPONENT_SOURCE_DIR}/src/services/diagnostic_management_stub.cpp
    ${COMPONENT_SOURCE_DIR}/src/IpService.cpp
    ${COMPONENT_SOURCE_DIR}/src/IpService.h    
    ${COMMON_SOURCES}

    # Stubs/mocks:
    #${CMAKE_CURRENT_SOURCE_DIR}/mocks/mock_ip_service.cpp
    #${CMAKE_CURRENT_SOURCE_DIR}/mocks/mock_ip_service.h
    ${CMAKE_CURRENT_SOURCE_DIR}/mocks/MockMessageDispatcher.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/mocks/MockMessageDispatcher.h
    ${CMAKE_CURRENT_SOURCE_DIR}/mocks/cedric/mock_cedric_localconfig.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/mocks/cedric/mock_cedric_localconfig.h
    ${CMAKE_CURRENT_SOURCE_DIR}/stubs/util/type_conversion_helpers.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/stubs/util/type_conversion_helpers.h
    ${CMAKE_CURRENT_SOURCE_DIR}/stubs/ipcommandbus/local_config_parameters.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/stubs/ipcommandbus/Message.cpp

    # Regular helper classes.
    # NOTE! MUST be outside coverage root, or under exclude folder to not mess up coverage values!
    ${LIBIP_COMMAND_BUS_DIR}/src/Pdu.cpp
    ${LIBIP_COMMAND_BUS_DIR}/include/ipcommandbus/Pdu.h
    ${LIBIP_COMMAND_BUS_DIR}/src/VccIpCmdApi.cpp
    ${LIBIP_COMMAND_BUS_DIR}/include/ipcommandbus/VccIpCmdApi.h
    ${LIBIP_COMMAND_BUS_DIR}/src/vcc_pdu_header.cpp
    ${LIBIP_COMMAND_BUS_DIR}/include/ipcommandbus/vcc_pdu_header.h
    ${LIBIP_COMMAND_BUS_DIR}/src/net_serializer.cpp
    ${LIBIP_COMMAND_BUS_DIR}/include/ipcommandbus/net_serializer.h
    ${LIBIP_COMMAND_BUS_DIR}/src/net_deserializer.cpp
    ${LIBIP_COMMAND_BUS_DIR}/include/ipcommandbus/net_deserializer.h
    ${LIBIP_COMMAND_BUS_DIR}/src/TimeoutInfo.cpp
    ${LIBIP_COMMAND_BUS_DIR}/include/ipcommandbus/TimeoutInfo.h
)

target_link_libraries(
        test_diagnostic_management

        # Functional libs.
        #log_context

        # Deps
        ${IVI-LOGGING_LIBRARIES}
        googletest
        asn1
        CommonAPI
 )

add_test(NAME test_diagnostic_management COMMAND test_diagnostic_management)

# ==========================================================================================
# Test ip service
add_executable(test_ip_service
    test_ip_service.cpp

    # Files under test:
    ${COMPONENT_SOURCE_DIR}/src/IpService.cpp
    ${COMPONENT_SOURCE_DIR}/src/IpService.h
    ${LIBIP_COMMAND_BUS_DIR}/src/VccIpCmdApi.cpp
    ${LIBIP_COMMAND_BUS_DIR}/include/ipcommandbus/VccIpCmdApi.h

    # Stubs/mocks:
    stubs/ipcommandbus/Message.cpp
    stubs/ipcommandbus/local_config_parameters.cpp
    stubs/ipcommandbus/TimeoutInfo.cpp
    stubs/ipcommandbus/Pdu.cpp

    # Regular helper classes.
)

target_link_libraries(
    test_ip_service

    # Functional libs.
    asn1
    ${IVI-LOGGING_LIBRARIES}
    libexternal-mocks
    libtype-conversion-helper-stubs

    # Deps
    googletest)

add_test(NAME test_ip_service COMMAND test_ip_service)

# ==========================================================================================
# Test connectivity
add_executable(test_connectivity
    test_connectivity.cpp

    # Files under test:
    ${COMPONENT_SOURCE_DIR}/src/Connectivity.cpp
    ${COMPONENT_SOURCE_DIR}/src/Connectivity.h
    ${LIBIP_COMMAND_BUS_DIR}/src/VccIpCmdApi.cpp
    ${LIBIP_COMMAND_BUS_DIR}/include/ipcommandbus/VccIpCmdApi.h

    # Stubs/mocks:
    stubs/ipcommandbus/Message.cpp
    stubs/ipcommandbus/local_config_parameters.cpp
    stubs/ipcommandbus/Pdu.cpp
    stubs/ipcommandbus/TimeoutInfo.cpp
    stubs/util/type_conversion_helpers.cpp
    stubs/util/type_conversion_helpers.h

    # Regular helper classes.
)

target_link_libraries(
    test_connectivity

    # Functional libs.
    asn1
    ${IVI-LOGGING_LIBRARIES}
    libexternal-mocks
    libtype-conversion-helper-stubs
    # Deps
    googletest)

add_test(NAME test_connectivity COMMAND test_connectivity)

# ==========================================================================================
# Test connectivity service
SET_SOURCE_FILES_PROPERTIES(
    ${COMPONENT_SOURCE_DIR}/src/services/connectivity_service.cpp
    PROPERTIES COMPILE_FLAGS -fexceptions)

add_executable(test_connectivity_service
    test_connectivity_service.cpp

    # Files under test:
    ${COMPONENT_SOURCE_DIR}/src/services/connectivity_service.cpp
    ${COMPONENT_SOURCE_DIR}/src/services/connectivity_service.h
    ${COMPONENT_SOURCE_DIR}/src/services/connectivity_service_stub.cpp
    ${COMPONENT_SOURCE_DIR}/src/services/connectivity_service_stub.h
    ${LIBIP_COMMAND_BUS_DIR}/src/VccIpCmdApi.cpp
    ${LIBIP_COMMAND_BUS_DIR}/include/ipcommandbus/VccIpCmdApi.h
    ${COMPONENT_SOURCE_DIR}/src/Connectivity.cpp
    ${COMPONENT_SOURCE_DIR}/src/Connectivity.h

    # Stubs/mocks:
    stubs/ipcommandbus/Message.cpp
    stubs/ipcommandbus/local_config_parameters.cpp
    stubs/ipcommandbus/TimeoutInfo.cpp
    stubs/ipcommandbus/Pdu.cpp

    # Regular helper classes.
)

target_include_directories(
    test_connectivity_service
    PRIVATE
    ${COMPONENT_SOURCE_DIR}/src/services
    ${COMPONENT_SOURCE_DIR}/test/common_dep/common_api
)

target_link_libraries(
    test_connectivity_service

    # Functional libs.
    asn1
    ${IVI-LOGGING_LIBRARIES}
    libtype-conversion-helper-stubs
    libexternal-mocks
    
    # Deps
    googletest
    CommonAPI)

add_test(NAME test_connectivity_service COMMAND test_connectivity_service)

# ==========================================================================================
# Test local config
add_executable(test_local_config
    test_local_config.cpp

    # Files under test:
    ${COMPONENT_SOURCE_DIR}/src/util/local_config.cpp
    ${COMPONENT_SOURCE_DIR}/src/util/local_config.h

    # Stubs/mocks:

    # Regular helper classes.
)

target_include_directories(
    test_local_config
    BEFORE
    PRIVATE
    ${COMPONENT_SOURCE_DIR}/src
)

target_link_libraries(
    test_local_config

    # Functional libs.
    ${IVI-LOGGING_LIBRARIES}

    # Deps
    libexternal-mocks
    googletest)


add_test(NAME test_local_config COMMAND test_local_config)


# ==========================================================================================
# Test service manager
add_executable(test_service_manager
    test_service_manager.cpp

    # Files under test:
    ${COMPONENT_SOURCE_DIR}/src/service_manager.cpp
    ${COMPONENT_SOURCE_DIR}/src/service_manager.h
    ${LIBIP_COMMAND_BUS_DIR}/src/VccIpCmdApi.cpp
    ${LIBIP_COMMAND_BUS_DIR}/include/ipcommandbus/VccIpCmdApi.h

    # Stubs/mocks:
    stubs/stub_gnss_service.cpp
    stubs/stub_gnss_service_stub.cpp
    stubs/stub_enhanced_pos_client.cpp
    stubs/stub_diagnostic_management_client.cpp
    stubs/stub_diagnostic_management_service.cpp
    stubs/stub_diagnostics_client.cpp
    stubs/stub_iplm_service.cpp
    stubs/stub_iplm_service_stub.cpp
    stubs/stub_connectivity_service.cpp
    stubs/stub_wlan_service.cpp
    stubs/stub_wifi_accesspoint_service.cpp
    stubs/stub_ota_service.cpp
    stubs/stub_ota_swlm_service.cpp
    stubs/stub_rvdc_client.cpp
    stubs/stub_telematics_service.cpp
    stubs/stub_common_phone_tele_service.cpp
    stubs/stub_common_phone_tele_proxy.cpp
    stubs/stub_Connectivity.cpp
    stubs/stub_car_modem_service.cpp
    stubs/stub_voc_backend_proxy.cpp
    stubs/stub_climate_timer.cpp
    stubs/stub_global_reset.cpp
    stubs/stub_workshop_wlan_service.cpp
    stubs/ipcommandbus/Message.cpp
    stubs/ipcommandbus/local_config_parameters.cpp
    stubs/ipcommandbus/TimeoutInfo.cpp
    stubs/ipcommandbus/Pdu.cpp
    stubs/util/local_config.cpp
    stubs/connectivity-sd/time_provider.cpp
    stubs/connectivity-sd/mock_time_provider.cpp

    # Regular helper classes.
)

target_include_directories(
    test_service_manager
    BEFORE
    PRIVATE
    ${COMPONENT_SOURCE_DIR}/test/common_dep/common_api
    ${COMPONENT_SOURCE_DIR}/src/services
)

target_link_libraries(
    test_service_manager

    # Functional libs.

    # Deps
    googletest
    asn1
    ${IVI-LOGGING_LIBRARIES}
    libtype-conversion-helper-stubs
    libdataelements-stubs
    libexternal-mocks

    CommonAPI
)


add_test(NAME test_service_manager COMMAND test_service_manager)

# ==========================================================================================
# Telematics service
add_executable(test_telematics_service
    # Files under test:
    ${COMPONENT_SOURCE_DIR}/src/services/itelematics_service.h
    ${COMPONENT_SOURCE_DIR}/src/services/telematics_service.cpp
    ${COMPONENT_SOURCE_DIR}/src/services/telematics_service.h

    # mocks
    ${CMAKE_CURRENT_SOURCE_DIR}/mocks/mock_telematics_service_stub.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/mocks/mock_telematics_service_stub.h

    # stubs
    ${CMAKE_CURRENT_SOURCE_DIR}/stubs/ipcommandbus/TimeoutInfo.cpp

    # Regular helper classes.

    # suits
    ${CMAKE_CURRENT_SOURCE_DIR}/test_telematics_service.cpp
)

target_include_directories(test_telematics_service
    BEFORE
    PRIVATE
    ${COMPONENT_SOURCE_DIR}/test/common_dep/common_api
    ${COMPONENT_SOURCE_DIR}/src/services
)

target_link_libraries(test_telematics_service
    # deps
    googletest
    asn1
    libipcommandbus_partial
    ${IVI-LOGGING_LIBRARIES}
    libtype-conversion-helper-stubs
    libexternal-mocks
    CommonAPI)

add_test(NAME test_telematics_service COMMAND test_telematics_service)

# ==========================================================================================
# Common phone tele service
add_executable(test_common_phone_tele_service
    # Files under test:
    ${COMPONENT_SOURCE_DIR}/src/services/icommon_phone_tele_service.h
    ${COMPONENT_SOURCE_DIR}/src/services/common_phone_tele_service.cpp
    ${COMPONENT_SOURCE_DIR}/src/services/common_phone_tele_service.h

    # mocks
    ${CMAKE_CURRENT_SOURCE_DIR}/mocks/mock_common_phone_tele_service_stub.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/mocks/mock_common_phone_tele_service_stub.h

    # stubs

    # Regular helper classes.

    # suits
    ${CMAKE_CURRENT_SOURCE_DIR}/test_common_phone_tele_service.cpp
)

target_include_directories(test_common_phone_tele_service
    BEFORE
    PRIVATE
    ${COMPONENT_SOURCE_DIR}/test/common_dep/common_api
    ${COMPONENT_SOURCE_DIR}/src/services
)

target_link_libraries(test_common_phone_tele_service
    # deps
    googletest
    asn1
    libipcommandbus_partial
    ${IVI-LOGGING_LIBRARIES}
    libtype-conversion-helper-stubs
    libexternal-mocks
    CommonAPI)

add_test(NAME test_common_phone_tele_service COMMAND test_common_phone_tele_service)

# ==========================================================================================
# Climate timer
add_executable(test_climate_timer_proxy
    ${COMPONENT_SOURCE_DIR}/src/services/climate_timer_proxy.cpp
    ${COMPONENT_SOURCE_DIR}/src/services/climate_timer_proxy.h

    # mocks and stubs
    ${CMAKE_CURRENT_SOURCE_DIR}/stubs/stub_ota_swlm_proxy.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/stubs/ipcommandbus/Message.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/mocks/mock_parking_climate_proxy.h

    ${CMAKE_CURRENT_SOURCE_DIR}/stubs/stub_capi_attribute.hpp

    ${LIBIP_COMMAND_BUS_DIR}/include/ipcommandbus/TimeoutInfo.h

    #test suites
    ${CMAKE_CURRENT_SOURCE_DIR}/test_climate_timer_proxy.cpp
)

target_include_directories(test_climate_timer_proxy
    BEFORE
    PRIVATE
    ${COMPONENT_SOURCE_DIR}/test/common_dep/common_api
    ${COMPONENT_SOURCE_DIR}/src/services
)

target_link_libraries(test_climate_timer_proxy
    #deps
    googletest
    asn1
    libipcommandbus_partial
    ${IVI-LOGGING_LIBRARIES}
    libtype-conversion-helper-stubs
    libexternal-mocks

    CommonAPI
)

add_test(NAME test_climate_timer_proxy COMMAND test_climate_timer_proxy)

# ==========================================================================================
# CarModem Service
SET_SOURCE_FILES_PROPERTIES(
    ${COMPONENT_SOURCE_DIR}/src/services/car_modem_apn_service.cpp
    PROPERTIES COMPILE_FLAGS -fexceptions)

add_executable(test_carmodem_service
    # Files under test:
    ${COMPONENT_SOURCE_DIR}/src/services/car_modem_service.cpp
    ${COMPONENT_SOURCE_DIR}/src/services/car_modem_service.h
    ${COMPONENT_SOURCE_DIR}/src/services/car_modem_apn_service.cpp
    ${COMPONENT_SOURCE_DIR}/src/services/car_modem_apn_service.h
    ${COMPONENT_SOURCE_DIR}/src/services/car_modem_service_stub.cpp
    ${COMPONENT_SOURCE_DIR}/src/services/car_modem_service_stub.h

    # mocks
    ${CMAKE_CURRENT_SOURCE_DIR}/mocks/mock_car_modem_service_stub.h
    ${CMAKE_CURRENT_SOURCE_DIR}/mocks/mock_car_modem_service_stub.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/mocks/mock_notify_system_ui.h
    ${CMAKE_CURRENT_SOURCE_DIR}/mocks/mock_notify_system_ui.cpp

    # stubs

    # Regular helper classes.

    # suites
    ${CMAKE_CURRENT_SOURCE_DIR}/test_carmodem_apn_service.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/test_carmodem_service.cpp
)

target_include_directories(test_carmodem_service
    BEFORE
    PRIVATE
    ${COMPONENT_SOURCE_DIR}/test/common_dep/common_api
    ${COMPONENT_SOURCE_DIR}/src/services
)

target_link_libraries(test_carmodem_service
    # deps
    googletest
    asn1
    ${IVI-LOGGING_LIBRARIES}
    libtype-conversion-helper-stubs
    libexternal-mocks

    CommonAPI)

add_test(NAME test_carmodem_service COMMAND test_carmodem_service)

# ==========================================================================================
# VolvoOnCallBackendProxy
add_executable(test_voc_backend_proxy
    ${COMPONENT_SOURCE_DIR}/src/services/voc_backend_proxy.cpp
    ${COMPONENT_SOURCE_DIR}/src/services/voc_backend_proxy.h
    ${COMMON_SOURCES}

    #mocks and stubs
    ${CMAKE_CURRENT_SOURCE_DIR}/stubs/stub_VolvoOnCallBackendProxy.cpp

    #test suites
    ${CMAKE_CURRENT_SOURCE_DIR}/test_voc_backend_proxy.cpp
)

target_include_directories(test_voc_backend_proxy
    BEFORE
    PRIVATE
    ${COMPONENT_SOURCE_DIR}/test/common_dep/common_api
    ${COMPONENT_SOURCE_DIR}/src/services
)

target_link_libraries(test_voc_backend_proxy
    #deps
    googletest
    asn1
    ${IVI-LOGGING_LIBRARIES}
    libtype-conversion-helper-stubs
    libipcommandbus_partial
    libexternal-mocks
    CommonAPI
)

add_test(NAME test_voc_backend_proxy COMMAND test_voc_backend_proxy)

# ==========================================================================================
# CommonPhoneTeleProxy
add_executable(test_common_phone_tele_proxy
    ${COMPONENT_SOURCE_DIR}/src/services/common_phone_tele_proxy.cpp
    ${COMPONENT_SOURCE_DIR}/src/services/common_phone_tele_proxy.h
    ${COMMON_SOURCES}

    #mocks and stubs
    ${CMAKE_CURRENT_SOURCE_DIR}/stubs/stub_CommonPhoneTelematicsBackendProxy.cpp

    #test suites
    ${CMAKE_CURRENT_SOURCE_DIR}/test_common_phone_tele_proxy.cpp
)

target_include_directories(test_common_phone_tele_proxy
    BEFORE
    PRIVATE
    ${COMPONENT_SOURCE_DIR}/test/common_dep/common_api
    ${COMPONENT_SOURCE_DIR}/src/services
)

target_link_libraries(test_common_phone_tele_proxy
    #deps
    googletest
    asn1
    ${IVI-LOGGING_LIBRARIES}
    libtype-conversion-helper-stubs
    libipcommandbus_partial
    libexternal-mocks
    CommonAPI
)

add_test(NAME test_common_phone_tele_proxy COMMAND test_common_phone_tele_proxy)

# ==========================================================================================
# Notification
add_executable(test_notify_system_ui
    # Files under test:
    ${COMPONENT_SOURCE_DIR}/src/util/notify_system_ui.cpp
    ${COMPONENT_SOURCE_DIR}/src/util/notify_system_ui.h
    ${COMMON_SOURCES}

    ${CMAKE_CURRENT_SOURCE_DIR}/test_notify_system_ui.cpp

    # Stubs/mocks:
    ${CMAKE_CURRENT_SOURCE_DIR}/mocks/MockLibSdBus.cpp

    # Regular helper classes.
    # NOTE! MUST be outside coverage root, or under exclude folder to not mess up coverage values!
)


target_include_directories(test_notify_system_ui
    BEFORE
    PRIVATE
)

target_link_libraries(test_notify_system_ui
    # Deps
    googletest
    ${IVI-LOGGING_LIBRARIES}
)

add_test(NAME test_notify_system_ui COMMAND test_notify_system_ui)

# ==========================================================================================
# Global Reset
add_executable(test_global_reset
    # Files under test:
    ${COMPONENT_SOURCE_DIR}/src/services/global_reset.cpp
    ${COMPONENT_SOURCE_DIR}/src/services/global_reset.h
    ${COMMON_SOURCES}

    ${CMAKE_CURRENT_SOURCE_DIR}/test_global_reset.cpp

    # Stubs/mocks:
    ${CMAKE_CURRENT_SOURCE_DIR}/stubs/ipcommandbus/Message.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/stubs/stub_iplm_service.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/stubs/stub_iplm_service_stub.cpp
    stubs/connectivity-sd/time_provider.cpp
    stubs/connectivity-sd/mock_time_provider.cpp

    # Regular helper classes.
    # NOTE! MUST be outside coverage root, or under exclude folder to not mess up coverage values!
)


target_include_directories(test_global_reset
    BEFORE
    PRIVATE
    ${COMPONENT_SOURCE_DIR}/test/common_dep/common_api
    ${COMPONENT_SOURCE_DIR}/src/services
)

target_link_libraries(test_global_reset
    # deps
    googletest
    asn1
    ${IVI-LOGGING_LIBRARIES}
    libdataelements-stubs
    libtype-conversion-helper-stubs
    libexternal-mocks

    CommonAPI)

add_test(NAME test_global_reset COMMAND test_global_reset)

# ==========================================================================================
# Wifi Access Point
add_executable(test_wifi_accesspoint
    ${COMPONENT_SOURCE_DIR}/src/services/wifi_accesspoint.cpp
    ${COMPONENT_SOURCE_DIR}/src/services/wifi_accesspoint.h
    ${COMPONENT_SOURCE_DIR}/src/IpService.cpp
    ${COMPONENT_SOURCE_DIR}/src/IpService.h
    ${CMAKE_CURRENT_SOURCE_DIR}/test_wifi_accesspoint_service.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/stubs/ipcommandbus/Message.cpp
)
target_include_directories(test_wifi_accesspoint
    BEFORE
    PRIVATE
    ${COMPONENT_SOURCE_DIR}/test/common_dep/common_api
    ${COMPONENT_SOURCE_DIR}/src/services
)
target_link_libraries(test_wifi_accesspoint
    googletest
    asn1
    ${IVI-LOGGING_LIBRARIES}
    libtype-conversion-helper-stubs
    libexternal-mocks
    libipcommandbus_partial
    CommonAPI
)

add_test(NAME test_wifi_accesspoint COMMAND test_wifi_accesspoint)

# ==========================================================================================
# WorkshopWlan
add_executable(test_workshop_wlan_service
    # Files under test:
    ${COMPONENT_SOURCE_DIR}/src/services/iworkshop_wlan_service.hpp
    ${COMPONENT_SOURCE_DIR}/src/services/workshop_wlan_service.cpp
    ${COMPONENT_SOURCE_DIR}/src/services/workshop_wlan_service.hpp

    # mocks
    ${CMAKE_CURRENT_SOURCE_DIR}/mocks/mock_workshop_wlan_stub.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/mocks/mock_workshop_wlan_stub.hpp

    # test
    ${CMAKE_CURRENT_SOURCE_DIR}/test_workshop_wlan_service.cpp
)

target_include_directories(test_workshop_wlan_service
    BEFORE
    PRIVATE
    ${COMPONENT_SOURCE_DIR}/test/common_dep/common_api
    ${COMPONENT_SOURCE_DIR}/src/services
)

target_link_libraries(test_workshop_wlan_service
    # deps
    googletest
    asn1
    libipcommandbus_partial
    ${IVI-LOGGING_LIBRARIES}
    libtype-conversion-helper-stubs
    libexternal-mocks
    CommonAPI)

add_test(NAME test_workshop_wlan_service COMMAND test_workshop_wlan_service)
