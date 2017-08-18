cmake_minimum_required(VERSION 3.2)
project (test_dataelements_commbus_dbus)
include(GNUInstallDirs)

enable_testing()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -fPIC -g -DUNIT_TEST")

find_package(PkgConfig REQUIRED)
pkg_check_modules(IVI-LOGGING REQUIRED ivi-logging)

enable_testing()

set (G_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/../../../test-utils/gmock-gtest)
set (MOCK_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/../../../test-utils/mocks)

file(GLOB SRC_GEN_FILES
    "${CMAKE_SOURCE_DIR}/../../generated/*.cpp"
)

include_directories(
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/..
  ${CMAKE_SOURCE_DIR}/../../../include
  ${CMAKE_SOURCE_DIR}/../../../src
  ${CMAKE_SOURCE_DIR}/../../../generated
  ${CMAKE_SOURCE_DIR}/../../../vector
  ${G_INCLUDE}
  ${MOCK_INCLUDE}
  ${IVI-LOGGING_INCLUDE_DIRS}
  ../../../test-utils/gmock-gtest
)


link_directories(
  ${IVI-LOGGING_LIBRARY_DIRS}
)

### Commbus DBUS Receiver test

set(TARGET_NAME_DBUS_REC ${CMAKE_PROJECT_NAME}_receiver)
add_executable(${TARGET_NAME_DBUS_REC}
  commbus_dbus_receiver_test.cpp
  ${CMAKE_SOURCE_DIR}/../../../src/dataelementcommbus_dbus_receiver.cpp
  ${CMAKE_SOURCE_DIR}/../../../src/dataelementcommbus_dbus_common.cpp
  ${CMAKE_SOURCE_DIR}/../../../test-utils/mocks/systemd/systemd_mock.cpp
  ${CMAKE_SOURCE_DIR}/../../../test-utils/mocks/pipe_mock.cpp
 
  ${G_INCLUDE}/gmock-gtest-all.cc
  ${G_INCLUDE}/gmock-gtest-main.cc
)

target_link_libraries(${TARGET_NAME_DBUS_REC}
  pthread
  ${IVI-LOGGING_LIBRARIES}
)

add_test(test_dataelements_commbus_dbus_receiver ${TARGET_NAME_DBUS_REC})

### Commbus DBUS test 
set(TARGET_NAME_DBUS ${CMAKE_PROJECT_NAME})
add_executable(${TARGET_NAME_DBUS}
  commbus_dbus_test.cpp
  ${CMAKE_SOURCE_DIR}/../../../src/dataelementcommbus_dbus.cpp
  ${CMAKE_SOURCE_DIR}/../../../src/dataelementcommbus_dbus_common.cpp
  ${CMAKE_SOURCE_DIR}/../../../test-utils/mocks/systemd/systemd_mock.cpp
  ${CMAKE_SOURCE_DIR}/../../../test-utils/mocks/pipe_mock.cpp
  ${CMAKE_SOURCE_DIR}/../../../test-utils/mocks/dbus_receiver_mock.cpp
 
  ${G_INCLUDE}/gmock-gtest-all.cc
  ${G_INCLUDE}/gmock-gtest-main.cc
)

target_link_libraries(${TARGET_NAME_DBUS}
  pthread
  ${IVI-LOGGING_LIBRARIES}
)

add_test(test_dataelements_commbus_dbus_receiver ${TARGET_NAME_DBUS})

