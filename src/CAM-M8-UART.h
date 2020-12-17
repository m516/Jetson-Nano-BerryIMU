#pragma once

#include <unistd.h>    // Used for UART
#include <sys/fcntl.h> // Used for UART
#include <termios.h>   // Used for UART

#ifdef __cplusplus
extern "C" {
#endif


typedef struct CAM_M8_UART{
    int fid; //Negative numbers are dead, positive ones are alive and unique
} CAM_M8_UART_t;


/**
 * @brief Initiailizes a UART connection
 * 
 * @param CAM_M8_UART The destination address of the newly created UART connection
 * @param target A path to the file read by the library. For example, this value is 
 * usually "/dev/serial0" on Raspberry Pis and "/dev/ttyTHS1" on Jetson Nanos
 * @param speed The baud rate of the serial conenction. For example, 
 * "B9600" represents the baud rate of 9600, and its integer value is 15.
 * @param port_options Other configuration parameters for the UART connection.
 * @return int 0 if successful, negative otherwise
 */
int CAM_M8_UART_init(CAM_M8_UART_t* CAM_M8_UART, const char* target, speed_t speed);

int CAM_M8_UART_is_live(CAM_M8_UART_t CAM_M8_UART);

int CAM_M8_UART_write(CAM_M8_UART_t CAM_M8_UART, char* message, size_t message_length);

ssize_t CAM_M8_UART_read(CAM_M8_UART_t CAM_M8_UART, void * buf, size_t nbytes);

int CAM_M8_UART_readln(CAM_M8_UART_t CAM_M8_UART, char* destination, size_t max_message_length);

int CAM_M8_UART_destroy(CAM_M8_UART_t* CAM_M8_UART);


#ifdef __cplusplus
}
#endif