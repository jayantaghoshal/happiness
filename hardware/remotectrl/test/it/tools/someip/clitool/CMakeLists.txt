cmake_minimum_required(VERSION 3.8)

project(RemoteCtrlHostSim)
find_package(vsomeip 2.10.10 REQUIRED)
#find_package(vsomeip-sd)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Woverloaded-virtual")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wformat=2")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

# include directories
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/../../../test_base/src)
include_directories(${PROJECT_SOURCE_DIR}/../../../test_base/include)
include_directories(${PROJECT_SOURCE_DIR}/../../../../../common/include)
include_directories(${VSOMEIP_INCLUDE_DIRS})

set(SOURCES
../../../test_base/src/remotectrl_client_base.cpp
src/remotectrl_client_mediactrl.cpp
src/remotectrl_client_audioctrl.cpp
src/remotectrl_client_climatectrl.cpp
src/main.cpp
)

# create the lib
add_executable(${PROJECT_NAME} ${SOURCES})

# link
target_link_libraries(${PROJECT_NAME} ${VSOMEIP_LIBRARIES})