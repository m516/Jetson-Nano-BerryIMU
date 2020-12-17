#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <fcntl.h>

#include "GPS.h"
#include "CAM-M8-UART.h"


//GPS handle
lwgps_t hgps;
#define NMEA_MAX_LENGTH 82
#define BUFFER_LENGTH NMEA_MAX_LENGTH * 5

#define GPS_RUNTIME_ERROR(...) {                                                                          \
    fprintf(stderr, "A runtime error related to the GPS component has occured.\n");                       \
    fprintf(stderr, "Encountered on %s:%d\n", __FILE__, __LINE__);                                        \
    fprintf(stderr, "In function %s\n", __func__);                                                        \
    fprintf(stderr, __VA_ARGS__);                                                                         \
}    

//Globals
//TODO place in struct in case there are multiple GPS modules
CAM_M8_UART_t serial_connection;
char buffer[BUFFER_LENGTH];
int current_num_bytes = 0;


int read_bytes, total_bytes = 0;
char *start, *end;

int berryIMU_GPS_init()
{
    //The BerryIMU GPS is connected to /dev/ttyTHS1 and has a default baud rate of 9600
    return CAM_M8_UART_init(&serial_connection, "/dev/ttyTHS1", B9600);

    //Init GPS
    lwgps_init(&hgps);
}

int berryIMU_GPS_update()
{

    char buf[255];

    read_bytes = read(serial_connection.fid, buffer + total_bytes, 20);
    if (-1 == read_bytes) {
        GPS_RUNTIME_ERROR("Unable to read the from the GPS!\n");
        return -1;
    }
    if (0 == read_bytes) {
        GPS_RUNTIME_ERROR("Unable to read the from the GPS!\n");
        return -1;
    }

    total_bytes += read_bytes;

    /* find start (a dollar $ign) */
    start = memchr(buffer, '$', total_bytes);
    if (NULL == start) {
        total_bytes = 0;
        return 0;
    }

    /* find end of line */
    end = memchr(start, '\r', total_bytes - (start - buffer));
    if (NULL == end) {
        return 0;
    }
    if ('\n' != *(++end)) {
        return 0;
    }

    /* handle data */
    lwgps_process(&hgps, start, strlen(start));    

    /* buffer empty? */
    if (end == buffer + total_bytes) {
        total_bytes = 0;
        return 0;
    }

    /* copy rest of buffer to beginning */
    if (buffer != memmove(buffer, end, total_bytes - (end - buffer))) {
        total_bytes = 0;
        return 0;
    }

    total_bytes -= end - buffer;

    
    return 0;
}


int berryIMU_GPS_destroy()
{
    //TODO check for errors
    return CAM_M8_UART_destroy(&serial_connection);
}