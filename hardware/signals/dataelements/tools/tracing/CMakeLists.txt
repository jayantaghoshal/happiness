cmake_minimum_required(VERSION 3.2)
project (signaltrace)


set(TARGET_NAME ${PROJECT_NAME})

include(GNUInstallDirs)

find_package(PkgConfig REQUIRED)
find_package(Threads REQUIRED)
pkg_check_modules(LIBSYSTEMD libsystemd)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_executable(${TARGET_NAME}
  src/signaltracer.cpp
)

target_link_libraries(${TARGET_NAME}
  pthread
  ${LIBSYSTEMD_LIBRARIES}
)

install(TARGETS ${TARGET_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR})

