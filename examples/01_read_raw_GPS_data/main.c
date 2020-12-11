#include "berryIMU_GPS.h"
#include <stdio.h>


int main()
{
    printf("Hello World\n");

    berryIMU_GPS_init();

    printf("SERIAL Port Good to Go.\n");

    for(int i = 0; i < 100; i++){
        berryIMU_GPS_update();
        //printf("Return code: %d\n", berryIMU_GPS_update());
    }

    berryIMU_GPS_destroy();

    printf("Goodbye World\n");
}


int uart_connection_is_live(uart_connection_t uart_connection){
    return uart_connection.fid>=0;
}