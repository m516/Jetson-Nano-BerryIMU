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

int uart_connection_init(uart_connection_t* uart_connection, const char* target, speed_t speed){
    uart_connection->fid = -1;
    struct termios port_options; // Create the structure


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
    //	    O_NOCTTY - When set and path identifies a terminal device, open() shall not cause the terminal device to become the controlling terminal for the process.

    uart_connection->fid = open(target, O_RDWR | O_NOCTTY);
    tcgetattr(uart_connection->fid, &port_options);



 

    tcflush(uart_connection->fid, TCIFLUSH);
    tcflush(uart_connection->fid, TCIOFLUSH);

    if (uart_connection->fid == -1)
    {
        UART_CONNECTION_RUNTIME_ERROR("Error - Unable to open UART.  Ensure it is not in use by another application\n");
        return -1;
    }


    cfsetispeed(&port_options, speed); // Set Read  Speed
    cfsetospeed(&port_options, speed); // Set Write Speed

    //------------------------------------------------
    // CONFIGURE THE UART
    //------------------------------------------------
    // flags defined in /usr/include/termios.h - see http://pubs.opengroup.org/onlinepubs/007908799/xsh/termios.h.html
    //	Baud rate:
    //         - B1200, B2400, B4800, B9600, B19200, B38400, B57600, B115200,
    //           B230400, B460800, B500000, B576000, B921600, B1000000, B1152000,
    //           B1500000, B2000000, B2500000, B3000000, B3500000, B4000000
    //	CSIZE: - CS5, CS6, CS7, CS8
    //	CLOCAL - Ignore modem status lines
    //	CREAD  - Enable receiver
    //	IGNPAR = Ignore characters with parity errors
    //	ICRNL  - Map CR to NL on input (Use for ASCII comms where you want to auto correct end of line characters - don't use for bianry comms!)
    //	PARENB - Parity enable
    //	PARODD - Odd parity (else even)

    port_options.c_cflag &= ~PARENB; // Disables the Parity Enable bit(PARENB),So No Parity
    port_options.c_cflag &= ~CSTOPB; // CSTOPB = 2 Stop bits,here it is cleared so 1 Stop bit
    port_options.c_cflag &= ~CSIZE;  // Clears the mask for setting the data size
    port_options.c_cflag |= CS8;     // Set the data bits = 8
    port_options.c_cflag |= CREAD | CLOCAL;                  // Enable receiver,Ignore Modem Control lines
    port_options.c_oflag = 0; // No Output Processing
    port_options.c_lflag = 0; // No terminal functions

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
    if(!uart_connection_is_live(uart_connection)){
        UART_CONNECTION_RUNTIME_ERROR("Connection not initialized. fid = %d\n", uart_connection.fid);
        return -1;
    }
    return write(uart_connection.fid, &message, message_length); //Filestream, bytes to write, number of bytes to write
}


ssize_t uart_connection_read(uart_connection_t uart_connection, void * buf, size_t nbytes){
    if(!uart_connection_is_live(uart_connection)){
        UART_CONNECTION_RUNTIME_ERROR("Connection not initialized. fid = %d\n", uart_connection.fid);
        return -1;
    }
    return read(uart_connection.fid, buf, nbytes);
}

int uart_connection_readln(uart_connection_t uart_connection, char* destination, size_t max_message_length){
    
    if(!uart_connection_is_live(uart_connection)){
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