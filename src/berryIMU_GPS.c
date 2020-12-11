
#include "berryIMU_GPS.h"
#include <stdio.h>

uart_connection_t serial_connection;
char buff[1024];

int berryIMU_GPS_init(){

    struct termios port_options; // Create the structure

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
    //port_options.c_cflag &= ~CRTSCTS;           // No Hardware flow Control
    port_options.c_cflag |= CREAD | CLOCAL;                  // Enable receiver,Ignore Modem Control lines
    port_options.c_iflag &= ~(IXON | IXOFF | IXANY);         // Disable XON/XOFF flow control both input & output
    port_options.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG); // Non Cannonical mode
    port_options.c_oflag &= ~OPOST;                          // No Output Processing

    port_options.c_lflag = 0; //  enable raw input instead of canonical,

    port_options.c_cc[VMIN] = 1; // Read at least 1 character
    port_options.c_cc[VTIME] = 0;    // Wait indefinetly

    return uart_connection_init(&serial_connection, "/dev/ttyTHS1", B9600, port_options);
}

int berryIMU_GPS_update(){
    ssize_t message_length;
    do{
        message_length = uart_connection_readln(serial_connection, buff, 1024);
    } while(message_length==0);

    if(message_length<0){
        //TODO ERROR
    }

    printf("Message Received: %s\n", buff);
}

int berryIMU_GPS_destroy(){
    //TODO check for errors
    return uart_connection_destroy(&serial_connection);
}