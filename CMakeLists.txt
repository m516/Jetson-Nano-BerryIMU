cmake_minimum_required (VERSION 2.8)
project(libBerryIMU C)

#Include libraries
include(extern/add_lwgps_library.cmake)

#Find files
set(LIBBERRYIMU_HEADERS
"include/GPS.h"
"include/IMU-backend.h"
"include/IMU.h"
)
set(LIBBERRYIMU_SOURCES
"src/CAM-M8-UART.c"
"src/CAM-M8-UART.h"
"src/GPS.c"
"src/i2c-dev.h"
"src/IMU-backend.c"
"src/IMU.c"
"src/LIS3MDL.h"
"src/LSM6DSL.h"
"src/LSM9DS0.h"
"src/LSM9DS1.h"
)

#Include directory in header search
include_directories(include/)

#Create the libberryimu library
add_library(
        berryimu STATIC
        ${LIBBERRYIMU_HEADERS}
        ${LIBBERRYIMU_SOURCES}
)

target_include_directories(berryimu BEFORE INTERFACE src/)
target_link_libraries(berryimu PRIVATE lwgps)

#Manually place all binaries into a folder under bin/
set(BUILD_FULL_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/examples/bin/)
message("Example Output directory: ${BUILD_FULL_DIRECTORY}")
# First for the generic no-config case (e.g. with mingw)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY  ${BUILD_FULL_DIRECTORY})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  ${BUILD_FULL_DIRECTORY})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY  ${BUILD_FULL_DIRECTORY})
# Second, for multi-config builds (e.g. msvc)
foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
    string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${BUILD_FULL_DIRECTORY})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${BUILD_FULL_DIRECTORY})
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${BUILD_FULL_DIRECTORY})
endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )

#TODO move to separate CMAKE file
set(proj_sources
"examples/01_read_raw_GPS_data/main.c"
)
add_executable(01_RAW_GPS ${proj_sources})
target_link_libraries(01_RAW_GPS berryimu)