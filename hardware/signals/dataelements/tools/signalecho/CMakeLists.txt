cmake_minimum_required(VERSION 3.2)
project (signalecho)


set(TARGET_NAME ${PROJECT_NAME})

include(GNUInstallDirs)

find_package(PkgConfig REQUIRED)
find_package(Threads REQUIRED)
pkg_check_modules(LIBSYSTEMD libsystemd)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

add_executable(${TARGET_NAME}
   signalecho.cpp
)

target_link_libraries(${TARGET_NAME}
  pthread
  dataelements
  ${LIBSYSTEMD_LIBRARIES}
)

install(TARGETS ${TARGET_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR})

