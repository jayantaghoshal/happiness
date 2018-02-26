project(test_time_provider)

set(TARGET_NAME ${PROJECT_NAME})

find_package(PkgConfig REQUIRED)
pkg_check_modules(GLIB_PKG REQUIRED glib-2.0)

set(COMMON_DIR ${REPO_ROOT_DIR}/common)

set(TEST_FILES
    test_timer_system.cpp
)

set(HEADER_FILES
    ${COMMON_DIR}/include/itime_provider.h
    ${COMMON_DIR}/include/time_provider.h
    ${COMMON_DIR}/include/dispatcher.h
)

set(SOURCE_FILES
    ${COMMON_DIR}/src/time_provider.cpp
    ${COMMON_DIR}/src/dispatcher.cpp
)

add_executable(${TARGET_NAME}
    ${TEST_FILES}
    ${HEADER_FILES}
    ${SOURCE_FILES}
)

target_include_directories(${TARGET_NAME} SYSTEM PRIVATE
    ${GLIB_PKG_INCLUDE_DIRS}
    ${COMMON_DIR}/include
)

target_link_libraries(${TARGET_NAME}
    gtest-main
    ${GLIB_PKG_LDFLAGS}
)

add_test(${TARGET_NAME} ${TARGET_NAME})
