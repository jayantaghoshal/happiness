cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project (ota_services_test_client)

include(../../../../cmake/shared.cmake)
find_package(PkgConfig REQUIRED)

find_package(CommonAPI REQUIRED)
find_package(CommonAPI-DBus REQUIRED)
find_package(CommonAPICodeGen REQUIRED)

pkg_check_modules(DBus REQUIRED dbus-1>=1.8.2)

pkg_check_modules(IVI-LOGGING REQUIRED ivi-logging)

SET(requiredlibs ${requiredlibs} ${IVI-LOGGING_LDFLAGS} )

set (SRC_GEN_DIR "src-gen")
set (SRC "src")
set (SRC_GEN_CODE_URI "v0/org/volvo/connectivity")

unset(CMAKE_CXX_FLAGS)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Wno-variadic-macros")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Woverloaded-virtual")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wformat=1")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")

use_commonapi_service(OTA_COMMONAPI org.volvocars.connectivity/ota)

add_executable(${PROJECT_NAME}
    ${SRC}/ota_services_test_client.cpp
    ${SRC}/log_context.cpp
    ${SRC}/log_context.hpp
    ${SRC_GEN_DIR}/${SRC_GEN_CODE_URI}/OTADBusProxy.cpp
    ${SRC_GEN_DIR}/${SRC_GEN_CODE_URI}/OTADBusDeployment.cpp
  )

target_include_directories(${PROJECT_NAME} PUBLIC
    ${IVI-LOGGING_INCLUDE_DIRS}
    ${SRC_GEN_DIR}
    ${COMMONAPI_INCLUDE_DIRS}
    ${COMMONAPI_DBUS_INCLUDE_DIRS}
    ${DBus_INCLUDE_DIRS}
 )

target_link_libraries(${PROJECT_NAME} ${requiredlibs}
    ${DBus_LDFLAGS}
    CommonAPI-DBus
    CommonAPI
    ${CONNECTIVITY_COMMONAPI_LIBRARIES}
    ${OTA_COMMONAPI_LIBRARIES}
)
