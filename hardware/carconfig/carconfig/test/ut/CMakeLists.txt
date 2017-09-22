set(GTEST_RUN_FLAGS --gtest_color=yes --gtest_repeat=20 --gtest_shuffle STRING "Flags passed to GTest")

include_directories(${GTEST_GMOCK_DIR}
    ${CARCONFIG_SRC_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR})

ADD_DEFINITIONS(-D_CSVFOLDER="${PROJECT_SOURCE_DIR}/test/ut/csv/")

add_executable(carconfig_file_test
    carconfig_file_test.cpp
    ${CARCONFIG_SRC_DIR}/carconfig_file.cpp
    ${GTEST_GMOCK_DIR}/gmock-gtest-all.cc)

add_executable(carconfig_writer_test
    carconfig_writer_test.cpp
    ${CARCONFIG_SRC_DIR}/carconfig_writer.cpp
    ${CARCONFIG_SRC_DIR}/carconfig_file.cpp
    ${GTEST_GMOCK_DIR}/gmock-gtest-all.cc)

add_executable(carconfig_reader_test
    carconfig_reader_test.cpp
    ${CARCONFIG_SRC_DIR}/carconfig_reader.cpp
    ${CARCONFIG_SRC_DIR}/carconfig_writer.cpp
    ${CARCONFIG_SRC_DIR}/carconfig_file.cpp
    ${GTEST_GMOCK_DIR}/gmock-gtest-all.cc)

target_link_libraries(carconfig_file_test -lpthread)
target_link_libraries(carconfig_writer_test -lpthread rt)
target_link_libraries(carconfig_reader_test -lpthread rt)

add_test(CarConfigFileTest carconfig_file_test ${GTEST_RUN_FLAGS})
add_test(CarConfigWriterTest carconfig_writer_test ${GTEST_RUN_FLAGS})
add_test(CarConfigReaderTest carconfig_reader_test ${GTEST_RUN_FLAGS})
