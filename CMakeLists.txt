cmake_minimum_required(VERSION 3.10)
project(lab-6)

include_directories(${CMAKE_CURRENT_LIST_DIR})

set(SRC_MAIN ./main.cpp)

add_executable(lab-6 ${SRC_MAIN})

set(LIBUSB_PATH ${PROJECT_SOURCE_DIR}/libusb)
include_directories(${LIBUSB_PATH}/include)
add_library(libusb-1.0 STATIC IMPORTED)
set_property(TARGET libusb-1.0 PROPERTY IMPORTED_LOCATION ${LIBUSB_PATH}/VS2015/MS64/static/libusb-1.0.lib)
set_property(TARGET libusb-1.0 PROPERTY IMPORTED_IMPLIB ${LIBUSB_PATH}/VS2015/MS64/static/libusb-1.0.lib)

target_link_libraries(lab-6 libusb-1.0)
