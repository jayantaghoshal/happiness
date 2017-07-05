cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project(testLibAsn1)

set(REPOSITORY_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../..)

# Include shared cmake stuff
include(${REPOSITORY_ROOT}/cmake/shared.cmake)

# Add test_utils with gtest and gmock
add_subdirectory(${REPOSITORY_ROOT}/test_utils test_utils)
set(test_utils_SOURCE_DIR ${REPOSITORY_ROOT}/test_utils)

if(NOT DEFINED component_SOURCE_DIR)
	set(component_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../..)
endif()

enable_testing()

#add_definitions(-DASN_ENABLE_TESTCODE)
include_directories(${component_SOURCE_DIR}/include
					${test_utils_SOURCE_DIR}/gmock-gtest
                    ${testLibAsn1_SOURCE_DIR}
                    ${testLibAsn1_SOURCE_DIR}/mocks)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

###################################

add_executable(${PROJECT_NAME}_test_1
        ${component_SOURCE_DIR}/generated/infotainmentIpBus.c
        ${component_SOURCE_DIR}/asn_base/asn_base.c
        ${component_SOURCE_DIR}/src/asn_print.cpp
        testASN1.cpp
        )

target_link_libraries(${PROJECT_NAME}_test_1 googletest)

add_test(NAME ${PROJECT_NAME}_test_1 COMMAND ${PROJECT_NAME}_test_1)
