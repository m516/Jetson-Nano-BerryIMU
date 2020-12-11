#include <stdio.h>

#include "berryIMU_GPS.h"
#include "gpgll.h"
#include "gpgga.h"
#include "gprmc.h"

#define BUFFER_LENGTH NMEA_MAX_LENGTH

//Globals
//TODO place in struct in case there are multiple GPS modules
uart_connection_t serial_connection;
char buffer[BUFFER_LENGTH];
int current_num_bytes = 0;

int berryIMU_GPS_init()
{
    //The BerryIMU GPS is connected to /dev/ttyTHS1 and has a default baud rate of 9600
    return uart_connection_init(&serial_connection, "/dev/ttyTHS1", B9600);
}

int berryIMU_GPS_update()
{

    char buf[BUFFER_LENGTH]; //For parsing and printing times

    //Fill up the buffer with data from the UART port
    int read_bytes = uart_connection_read(serial_connection, buffer + current_num_bytes, BUFFER_LENGTH - current_num_bytes);
    if (read_bytes == -1)
    {
        fprintf(stderr, "Failed to read from GPS UART connection.\n");
        return -1;
    }
    // if(read_bytes==0){
    //     fprintf(stderr, "GPS UART connection closed.\n");
    //     return -2;
    // }
    current_num_bytes += read_bytes;

    //Parse NMEA sentences in the buffer
    char *start;            //This will be the address of the first character in an NMEA sentence. (Always '$')
    char *end = buffer; //This will be the address of the last character in an NMEA sentence.

    //For each NMEA sentence in the buffer
    while (1)
    {
        //Find where the first valid NMEA sentence starts (must begin with $)
        start = memchr(end, '$', current_num_bytes - (end - buffer));
        //Check if the start character was found
        if (start == NULL)
        { //Failed to find '$'
            printf("\n------------\nFailed to find start character in:\n'%s'\n--------------\n", end+1);
            //Reset the buffer
            current_num_bytes = 0;
            memset(buffer, 0, sizeof(buffer));
            return -3;
        }
        //Find the end of the NMEA sentence if it exists
        for(end = start; end < buffer+current_num_bytes-1; end++){
            if(*end=='\0'){
                end = NULL;
                break;
            }
            if(*end==NMEA_END_CHAR_1 || *end==NMEA_END_CHAR_2){
                if(*end+1==NMEA_END_CHAR_1 || *end+1==NMEA_END_CHAR_2){
                    end++;
                }
                break;
            }
        }
        end++;
        if(end==buffer+current_num_bytes) end=NULL;
        //If no end was found
        if (end == NULL)
        {
            //Move the bytes at the end of the buffer back to the start, discarding all read/invalid NMEA sentences
            current_num_bytes -= start - buffer;
            memcpy(buffer, start, current_num_bytes);
            memset(buffer+current_num_bytes, 0, sizeof(buffer)-current_num_bytes);
            return 0;
        }

        /* handle data */
        nmea_s *data = nmea_parse(start, end - start + 1, 0);
        printf("Message: ");
        for(char* i = start; i < end; i++) printf("%c", *i);
        if (data == NULL)
        {
            continue;
        }
        if (0 < data->errors)
        {
            printf("WARN: The sentence struct contains parse errors!\n");
        }

        if (NMEA_GPGGA == data->type)
        {
            printf("GPGGA sentence\n");
            nmea_gpgga_s *gpgga = (nmea_gpgga_s *)data;
            printf("Number of satellites: %d\n", gpgga->n_satellites);
            printf("Altitude: %d %c\n", gpgga->altitude, gpgga->altitude_unit);
        }

        if (NMEA_GPGLL == data->type)
        {
            printf("GPGLL sentence\n");
            nmea_gpgll_s *pos = (nmea_gpgll_s *)data;
            printf("Longitude:\n");
            printf("  Degrees: %d\n", pos->longitude.degrees);
            printf("  Minutes: %f\n", pos->longitude.minutes);
            printf("  Cardinal: %c\n", (char)pos->longitude.cardinal);
            printf("Latitude:\n");
            printf("  Degrees: %d\n", pos->latitude.degrees);
            printf("  Minutes: %f\n", pos->latitude.minutes);
            printf("  Cardinal: %c\n", (char)pos->latitude.cardinal);
            strftime(buf, sizeof(buf), "%H:%M:%S", &pos->time);
            printf("Time: %s\n", buf);
        }

        if (NMEA_GPRMC == data->type)
        {
            printf("GPRMC sentence\n");
            nmea_gprmc_s *pos = (nmea_gprmc_s *)data;
            printf("Longitude:\n");
            printf("  Degrees: %d\n", pos->longitude.degrees);
            printf("  Minutes: %f\n", pos->longitude.minutes);
            printf("  Cardinal: %c\n", (char)pos->longitude.cardinal);
            printf("Latitude:\n");
            printf("  Degrees: %d\n", pos->latitude.degrees);
            printf("  Minutes: %f\n", pos->latitude.minutes);
            printf("  Cardinal: %c\n", (char)pos->latitude.cardinal);
            strftime(buf, sizeof(buf), "%H:%M:%S", &pos->time);
            printf("Time: %s\n", buf);
        }
        nmea_free(data);
    }
}

int berryIMU_GPS_destroy()
{
    //TODO check for errors
    return uart_connection_destroy(&serial_connection);
}