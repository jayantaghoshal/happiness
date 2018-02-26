project (common)

set(TARGET_NAME ${PROJECT_NAME})

find_package(CommonAPI REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GLIB_PKG REQUIRED glib-2.0)
pkg_check_modules(IVI-LOGGING REQUIRED ivi-logging)
pkg_check_modules(DATA_ELEMENTS REQUIRED  dataelements)

set(HEADERS
    include/logging.h
    include/logging_basic.h
    include/dfsm.h
    include/idispatcher.h
    include/dispatcher.h
    include/vfc_helper.h
    include/commonApiEventSubscription.h
    include/local_config_helper.h
    include/itime_provider.h
    include/time_provider.h
    include/notifiable_property.h
    include/kpi_log.h
    include/appstorage.h
)

set(SOURCES
    src/dfsm.cpp
    src/dispatcher.cpp
    src/vfc_helper.cpp
    src/time_provider.cpp
    src/appstorage.cpp
)

link_directories(
    ${IVI-LOGGING_LIBRARY_DIRS}
    ${DATA_ELEMENTS_LIBRARY_DIRS}
)

add_library(${TARGET_NAME} STATIC
    ${HEADERS}
    ${SOURCES}
)

target_include_directories(${TARGET_NAME} PUBLIC
    include
)

target_include_directories(${TARGET_NAME} PUBLIC SYSTEM
    ${COMMONAPI_INCLUDE_DIRS}
    ${GLIB_PKG_INCLUDE_DIRS}
    ${IVI-LOGGING_INCLUDE_DIRS}
    ${DATA_ELEMENTS_INCLUDE_DIRS}
)

target_link_libraries(${TARGET_NAME} PUBLIC
    Qt5::Core
    CommonAPI
    ${IVI-LOGGING_LIBRARIES}
    ${GLIB_PKG_LDFLAGS}
    ${DATA_ELEMENTS_LIBRARIES}
)