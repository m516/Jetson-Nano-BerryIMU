#pragma once

#include "uart_connection.h"

#ifdef __cplusplus
extern "C" {
#endif

int berryIMU_GPS_init();

int berryIMU_GPS_update();

int berryIMU_GPS_destroy();

#ifdef __cplusplus
}
#endif