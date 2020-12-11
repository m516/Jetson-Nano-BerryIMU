cmake_minimum_required (VERSION 2.8)
project(libBerryIMU C)

#Include libraries
add_subdirectory(extern/libnmea)

#Find files
set(LIBBERRYIMU_HEADERS
"include/berryIMU_GPS.h"
"include/uart_connection.h"
)
set(LIBBERRYIMU_SOURCES
"src/berryIMU_GPS.c"
"src/uart_connection.c"
)




#Include directory in header search
include_directories(include)

#Create the libberryimu library
add_library(
        berryimu STATIC
        ${LIBBERRYIMU_HEADERS}
        ${LIBBERRYIMU_SOURCES}
)

target_include_directories(berryimu PRIVATE src)
target_link_libraries(berryimu PRIVATE nmea)

#TODO move to separate CMAKE file
set(proj_sources
"examples/01_read_raw_GPS_data/main.c"
)
add_executable(01_RAW_GPS ${proj_sources})
target_link_libraries(01_RAW_GPS berryimu)