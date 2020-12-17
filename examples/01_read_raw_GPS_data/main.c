
// To build: gcc examples/minimum.c -lnmea -o minimum

// #include <stdlib.h>
// #include <stdio.h>
// #include <string.h>

// #include "nmea.h"
// #include"gpgll.h"

// int
// main(void)
// {
// 	// Sentence string to be parsed
// 	char sentence[] = "$GPGLL,4916.45,N,12311.12,W,225444,A,*1D\r\n";

// 	printf("Parsing NMEA sentence: %s", sentence);

// 	// Pointer to struct containing the parsed data. Should be freed manually.
// 	nmea_s *data;

// 	// Parse...
// 	data = nmea_parse(sentence, strlen(sentence), 0);

// 	if (NMEA_GPGLL == data->type) {
// 		nmea_gpgll_s *gpgll = (nmea_gpgll_s *) data;

// 		printf("GPGLL Sentence\n");
// 		printf("Longitude:\n");
// 		printf("  Degrees: %d\n", gpgll->longitude.degrees);
// 		printf("  Minutes: %f\n", gpgll->longitude.minutes);
// 		printf("  Cardinal: %c\n", (char) gpgll->longitude.cardinal);
// 		printf("Latitude:\n");
// 		printf("  Degrees: %d\n", gpgll->latitude.degrees);
// 		printf("  Minutes: %f\n", gpgll->latitude.minutes);
// 		printf("  Cardinal: %c\n", (char) gpgll->latitude.cardinal);
// 	}

// 	nmea_free(data);

// 	return 0;
// }



#include "GPS.h"
#include <stdio.h>
#include <time.h>

#define clear() printf("\033[H\033[J")

int main()
{
    printf("Hello World\n");

    berryIMU_GPS_init();

    printf("SERIAL Port Good to Go.\n");

    for(int i = 0; i < 1000; i++){
        berryIMU_GPS_update();
        clear();
        printf("i: %d\n", i);
        /* Print messages */
        printf("Valid status: %d\r\n", hgps.is_valid);
        printf("Latitude: %f degrees\r\n", hgps.latitude);
        printf("Longitude: %f degrees\r\n", hgps.longitude);
        printf("Altitude: %f meters\r\n", hgps.altitude);
    }

    berryIMU_GPS_destroy();

    printf("Goodbye World\n");
}

