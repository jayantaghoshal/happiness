
project (test_common)

find_package(Qt5Core)
find_package(Qt5Qml)

set(TARGET_NAME ${PROJECT_NAME})

set (COMMON_DIR ${REPO_ROOT_DIR}/common)

set (COMMON_SOURCES
    ${COMMON_DIR}/src/appstorage.cpp
)

set (TEST_SOURCES
#    test_dispatcher.cpp
    test_appstorage.cpp
    test_notifiable_property.cpp
    test_lcfg.cpp
    test_vfc_helper.cpp
    test_dfsm.cpp
)

add_executable(${TARGET_NAME}
    ${COMMON_SOURCES}
    ${TEST_SOURCES}
)

target_link_libraries(${TARGET_NAME}
    gtest-main
    Qt5::Core
    Qt5::Qml
    cedric-core
    dataelements
    common
)

add_test(${TARGET_NAME} ${TARGET_NAME})
