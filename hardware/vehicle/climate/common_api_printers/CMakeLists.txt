project(common_api_printers)
set(TARGET_NAME ${PROJECT_NAME})

set(HEADERS
    include/common_types_printers.h
    include/first_row_printers.h
    include/parking_climate_printers.h
    include/second_and_third_row_printers.h
)

set(SOURCES
    source/common_types_printers.cpp
    source/first_row_printers.cpp
    source/parking_climate_printers.cpp
    source/second_and_third_row_printers.cpp
)

add_library(${TARGET_NAME} STATIC
    ${SOURCES}
    ${HEADERS}
)

target_include_directories(${TARGET_NAME}  PUBLIC
    include
)

target_link_libraries (${TARGET_NAME} PRIVATE
    common
)

if(UNIT_TEST)
# Hack to add include paths to capi stubs for unittest builds
target_link_libraries (${TARGET_NAME} PRIVATE
    capi_v0
)
endif()
