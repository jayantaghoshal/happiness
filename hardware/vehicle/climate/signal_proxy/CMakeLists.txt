project(signal_proxy)
set(TARGET_NAME ${PROJECT_NAME})

pkg_check_modules(DATA_ELEMENTS REQUIRED  dataelements)

set(HEADERS
    include/signal_proxy.h
)

set(SOURCES
)


add_library(${TARGET_NAME} STATIC
    ${SOURCES}
    ${HEADERS}
)

target_include_directories(${TARGET_NAME} SYSTEM PUBLIC
    ${DATA_ELEMENTS_INCLUDE_DIRS}
)

target_include_directories(${TARGET_NAME} PUBLIC
    include
)

target_link_libraries (${TARGET_NAME} PRIVATE
    ${DATA_ELEMENTS_LIBRARIES}
)



