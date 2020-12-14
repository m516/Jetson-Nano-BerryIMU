<!-- omit in toc -->
# Lib BerryIMU

Lib BerryIMU is a C library that acts as a simple, intuitive interface between a Jetson Nano and a BerryIMU from [Ozzmaker](https://ozzmaker.com). It only depends on standard POSIX libraries; all other dependencies are contained and maintained in this Git repository through submodules.

**Please note that this library is a WORK IN PROGRESS and is undergoing many changes in content and structure.**  

<!-- omit in toc -->
## Table of Contents

- [Features](#features)
  - [GPS](#gps)
- [Future projects](#future-projects)
  - [Accelerometer](#accelerometer)
  - [Gyrometer](#gyrometer)
  - [Magnetometer](#magnetometer)
  - [Barometer/Altimeter](#barometeraltimeter)
  - [Thermometer](#thermometer)
  - [Raspberry Pi Support](#raspberry-pi-support)
  - [Installing the Library](#installing-the-library)
- [Licensing](#licensing)
- [Reference](#reference)
- [Contrubuting](#contrubuting)

## Features

The library currently supports the following module:

### GPS

- **Model:** u-blox CAM-M8
- **Found in:** the BerryGPS line of products
- **Connection type:** UART
  - **Default baud rate:** [9600](https://ozzmaker.com/berrygps_imu-faq/)
  - **Parity**: [none](https://en.wikipedia.org/wiki/NMEA_0183)
  - **Stop bits:** [1](https://en.wikipedia.org/wiki/NMEA_0183)
  - **Default data bits:** [8](https://en.wikipedia.org/wiki/NMEA_0183)
- **API:**
  - `berryIMU_GPS_init()`: Initializes the system's connection to the GPS.
  - `berryIMU_GPS_update()`: Reads and parses the data from the GPS through its UART connection.
  - `berryIMU_GPS_destroy()`: Deinitializes the system's connection wtih the GPS module
  - `berryIMU_GPS_current_coordinates()`: Retrieves the most recent measured GPS coordinates in latitude and longitude. (not implemented yet)

## Future projects

In future versions of this library, there will be support for these modules:

### Accelerometer

- **Model:** LSM6DSL
- **Found in:** BerryIMU v3
- **Connection type:** I2C
  - **Default address:** 0x6A

### Gyrometer

- **Model:** LSM6DSL
- **Found in:** BerryIMU v3
- **Connection type:** I2C
  - **Default address:** 0x6A

### Magnetometer

- **Model:** LIS3MDL
- **Found in:** BerryIMU v3
- **Connection type:** I2C
  - **Default address:** 0x1C

### Barometer/Altimeter

- **Model:** BM388
- **Found in:** BerryIMU v3
- **Connection type:** I2C
  - **Default address:** 0x77

### Thermometer

I'm guessing on these specs; I'm not sure if the BM388 has a temperature sensor at the moment

- **Model:** BM388
- **Found in:** BerryIMU v3
- **Connection type:** I2C
  - **Default address:** 0x77

### Raspberry Pi Support

I think this library could be useful for Raspberry Pi users. Unfortunately, I don't have a new Raspberry Pi to test this library on, so I haven't made an effort to try and write code that may or may not work.

For more information, please refer to the corresponding [issue](https://github.com/m516/libberryimu/issues/1).

### Installing the Library

CMake makes it easy to install C libraries. I'm not sure how that process works though. 

To use the library, just copy it (and all submodules!) into a subfolder of your project, and in your root-level CMakeLists.txt file, add something like `add_subdirectory(path_to_this_folder)` and a new library called `berryimu` can be linked with your project. (For examples, see [this repository](https://github.com/m516/Jetson-Nano-Sandbox)).

## Licensing

The contents of this repository are released under the [MIT license](LICENSE).

## Reference

The contents of this repository are heavily based on the official Ozzmaker instructions on [ozzmaker.com](https://ozzmaker.com) and their corresponding [Git repository](https://github.com/ozzmaker/BerryIMU).

## Contrubuting

- As always, the issues tab is open for bugs and feature requests. No relevant issue is a bad issue!  
- Pull requests are welcome and will very likely be accepted, especially for tasks marked "Future Projects," solutions to issues, and interfaces to new parts (even ones that aren't necessarily on the BerryIMU). My only asks are:
  - No syntax errors
  - All public headers belong in include/
  - All private headers and implemetation code belong in src/
  - Functionality for new sensors (like a new accelerometer part) are placed in a new folder whose name matches the part number.
  - All public-facing functions for sensors begin with the part number of that sensor (to avoid naming conflicts)
  - Feel free to make your own API. If you do, please document it so that others know how to use it!
- I'll especially need testing help from people with the Raspberry Pi 2, 3, and 4 and the BerryIMU v2 and v4 because I don't have any of these devices.
