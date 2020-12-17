#pragma once

#ifdef __cplusplus
extern "C" {
#endif


#include "lwgps/lwgps.h"

extern lwgps_t hgps;

int berryIMU_GPS_init();

int berryIMU_GPS_update();

int berryIMU_GPS_destroy();

#ifdef __cplusplus
}
#endif