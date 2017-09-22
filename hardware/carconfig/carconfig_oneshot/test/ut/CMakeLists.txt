set(GTEST_RUN_FLAGS --gtest_color=yes --gtest_repeat=20 --gtest_shuffle STRING "Flags passed to GTest")

include_directories(${GTEST_GMOCK_DIR}
    ${CARCONFIG_ONESHOT_SRC_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/mocks
    )

ADD_DEFINITIONS(-D_TESTFOLDER="${PROJECT_SOURCE_DIR}/test/ut/")
add_executable(carconfig_oneshot_test
  carconfig_oneshot_test.cpp
  ${GTEST_GMOCK_DIR}/gmock-gtest-all.cc)

target_link_libraries(carconfig_oneshot_test -lpthread)

add_test(CarconfigOneshotTest carconfig_oneshot_test ${GTEST_RUN_FLAGS})
