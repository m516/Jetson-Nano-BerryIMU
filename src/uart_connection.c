#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "uart_connection.h"

#define UART_CONNECTION_RUNTIME_ERROR(...) {                                                              \
    fprintf(stderr, "A runtime error related to a uart_connection has occured.\n");                       \
    fprintf(stderr, "Encountered on %s:%d\n", __FILE__, __LINE__);                                        \
    fprintf(stderr, "In function %s\n", __func__);                                                        \
    fprintf(stderr, __VA_ARGS__);                                                                         \
}                                                                                                         \

int uart_connection_init(uart_connection_t* uart_connection, const char* target, speed_t speed, struct termios port_options){
    uart_connection->fid = -1;


    //------------------------------------------------
    //  OPEN THE UART
    //------------------------------------------------
    // The flags (defined in fcntl.h):
    //	Access modes (use 1 of these):
    //		O_RDONLY - Open for reading only.
    //		O_RDWR   - Open for reading and writing.
    //		O_WRONLY - Open for writing only.
    //	    O_NDELAY / O_NONBLOCK (same function)
    //               - Enables nonblocking mode. When set read requests on the file can return immediately with a failure status
    //                 if there is no input immediately available (instead of blocking). Likewise, write requests can also return
    //				   immediately with a failure status if the output can't be written immediately.
    //                 Caution: VMIN and VTIME flags are ignored if O_NONBLOCK flag is set.
    //	    O_NOCTTY - When set and path identifies a terminal device, open() shall not cause the terminal device to become the controlling terminal for the process.fid = open("/dev/ttyTHS1", O_RDWR | O_NOCTTY | O_NDELAY);		//Open in non blocking read/write mode

    uart_connection->fid = open(target, O_RDWR | O_NOCTTY);

    tcflush(uart_connection->fid, TCIFLUSH);
    tcflush(uart_connection->fid, TCIOFLUSH);

    if (uart_connection->fid == -1)
    {
        UART_CONNECTION_RUNTIME_ERROR("Error - Unable to open UART.  Ensure it is not in use by another application\n");
        return -1;
    }


    cfsetispeed(&port_options, speed); // Set Read  Speed
    cfsetospeed(&port_options, speed); // Set Write Speed

    // Set the attributes to the termios structure
    int att = tcsetattr(uart_connection->fid, TCSANOW, &port_options);

    if (att != 0)
    {
        UART_CONNECTION_RUNTIME_ERROR("Error in Setting port attributes\n");
        return -2;
    }

    // Flush Buffers
    tcflush(uart_connection->fid, TCIFLUSH);
    tcflush(uart_connection->fid, TCIOFLUSH);

    return 0;
}


int uart_connection_write(uart_connection_t uart_connection, char* message, size_t message_length){
    if(uart_connection.fid<0){
        UART_CONNECTION_RUNTIME_ERROR("Connection not initialized. fid = %d\n", uart_connection.fid);
        return -1;
    }
    return write(uart_connection.fid, &message, message_length); //Filestream, bytes to write, number of bytes to write
}

int uart_connection_readln(uart_connection_t uart_connection, char* destination, size_t max_message_length){
    
    if(uart_connection.fid<0){
        UART_CONNECTION_RUNTIME_ERROR("Connection not initialized. fid = %d\n", uart_connection.fid);
        return -1;
    }

    char* i = destination;
    char* end_of_message = destination+max_message_length;
    ssize_t size_read = 0;


    while(i<end_of_message){
        ssize_t read_result = read(uart_connection.fid, (void *)i, 1);
        if(i==destination && (isspace(*i) ||  iscntrl(*i))) continue; //Strip beginning whitespace and null characters
        if(read_result!=1) {
            UART_CONNECTION_RUNTIME_ERROR("Failed to read a character. read() returned %ld\n", read_result);
            return -1;
        }
        if(*i=='\n') break;
        if(*i=='\r') break;
        if(*i=='\0') break;
        i++;
        size_read++;
    }
    if(i!=end_of_message){
        i++;
        size_read++;
    }
    *i='\0';
    return size_read-1;
}

int uart_connection_destroy(uart_connection_t* uart_connection){
    int r = close(uart_connection->fid);
    uart_connection->fid = -1;
    return r;
}