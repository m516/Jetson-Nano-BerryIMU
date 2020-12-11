#pragma once

#include <unistd.h>    // Used for UART
#include <sys/fcntl.h> // Used for UART
#include <termios.h>   // Used for UART

#ifdef __cplusplus
extern "C" {
#endif


typedef struct uart_connection{
    int fid; //Negative numbers are dead, positive ones are alive and unique
} uart_connection_t;


/**
 * @brief Initiailizes a UART connection
 * 
 * @param uart_connection The destination address of the newly created UART connection
 * @param target A path to the file read by the library. For example, this value is 
 * usually "/dev/serial0" on Raspberry Pis and "/dev/ttyTHS1" on Jetson Nanos
 * @param speed The baud rate of the serial conenction. For example, 
 * "B9600" represents the baud rate of 9600, and its integer value is 15.
 * @param port_options Other configuration parameters for the UART connection.
 * @return int 0 if successful, negative otherwise
 */
int uart_connection_init(uart_connection_t* uart_connection, const char* target, speed_t speed, struct termios port_options);

int uart_connection_is_live(uart_connection_t uart_connection);

int uart_connection_write(uart_connection_t uart_connection, char* message, size_t message_length);

int uart_connection_readln(uart_connection_t uart_connection, char* destination, size_t max_message_length);

int uart_connection_destroy(uart_connection_t* uart_connection);


#ifdef __cplusplus
}
#endif