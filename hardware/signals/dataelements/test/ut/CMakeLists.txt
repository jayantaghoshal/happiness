cmake_minimum_required(VERSION 3.2)
project (test_dataelements)
set(TARGET_NAME ${CMAKE_PROJECT_NAME})
include(GNUInstallDirs)

enable_testing()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++14 -fPIC -DUNIT_TEST")

find_package(PkgConfig REQUIRED)
pkg_check_modules(IVI-LOGGING REQUIRED ivi-logging)

enable_testing()

set (G_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/../../test-utils/gmock-gtest)

file(GLOB SRC_GEN_FILES
    "${CMAKE_SOURCE_DIR}/../../generated/*.cpp"
)

include_directories(
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/../../include
  ${CMAKE_SOURCE_DIR}/../../generated
  ${CMAKE_SOURCE_DIR}/../../vector
  ${G_INCLUDE}
  ${IVI-LOGGING_INCLUDE_DIRS}
  ../../test-utils/gmock-gtest
)


link_directories(
  ${IVI-LOGGING_LIBRARY_DIRS}
)

add_executable(${TARGET_NAME}
  unittest.cpp
  ${CMAKE_SOURCE_DIR}/../../src/dataelemvalue.cpp
  ${CMAKE_SOURCE_DIR}/../../src/dataelementframework.cpp
  ${CMAKE_SOURCE_DIR}/../../src/dataelementcommbus_standalone.cpp
  ${CMAKE_SOURCE_DIR}/../../src/vipcomm/VipFramework.cpp
  ${CMAKE_SOURCE_DIR}/../../src/vipcomm/VipFramework_crc.cpp
  ${SRC_GEN_FILES}
  ${G_INCLUDE}/gmock-gtest-all.cc
  ${G_INCLUDE}/gmock-gtest-main.cc
)

target_link_libraries(${TARGET_NAME}
  pthread
  ${IVI-LOGGING_LIBRARIES}
)

add_test(test_dataelements ${TARGET_NAME})
