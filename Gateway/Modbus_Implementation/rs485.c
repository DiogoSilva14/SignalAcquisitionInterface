#include "rs485.h"

// RS-485 line baud rate
int baud_rate = 0;
int serial_port = 0;

int init_rs485(char* serial_port_device, int _baud_rate, uint8_t bits_per_frame, uint8_t parity_bit, uint8_t stop_bits, uint8_t block){
    serial_port = open(serial_port_device, O_RDWR | O_NOCTTY | O_NONBLOCK);

    if(serial_port < 0){
        printf("Error %i from open: %s\n", errno, strerror(errno));

        return INITIALIZATION_NOT_SUCCESSFUL;
    }

    struct termios tty;

    if(tcgetattr(serial_port, &tty) != 0){
        printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));

        return INITIALIZATION_NOT_SUCCESSFUL;
    }

    if(parity_bit){
        tty.c_cflag |= PARENB;
    }else{
        tty.c_cflag &= ~PARENB;
    }

    if(stop_bits > 1){
        tty.c_cflag |= CSTOPB;
    }else{
        tty.c_cflag &= ~CSTOPB;
    }
    
    tty.c_cflag &= ~CSIZE;

    switch(bits_per_frame){
        case 5:
            tty.c_cflag |= CS5;
            break;
        case 6:
            tty.c_cflag |= CS6;
            break;
        case 7:
            tty.c_cflag |= CS7;
            break;
        case 8:
            tty.c_cflag |= CS8;
            break;
        default:
            tty.c_cflag |= CS8;
            break;
    }
     
    tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
    tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)
    
    /*
    if(block){
        tty.c_cc[VTIME] = 0;
        tty.c_cc[VMIN] = 1;
    }else{
        tty.c_cc[VTIME] = 0;
        tty.c_cc[VMIN] = 0;
    }
    */

    tty.c_cc[VTIME] = 0;
    tty.c_cc[VMIN] = 0;

    if(_baud_rate == 115200){
        cfsetispeed(&tty, (speed_t)B115200);
        cfsetospeed(&tty, (speed_t)B115200);
    }else if(_baud_rate == 9600){
        cfsetispeed(&tty, (speed_t)B9600);
        cfsetospeed(&tty, (speed_t)B9600);
    }else{
        return BAUD_RATE_NOT_SUPPORTED;
    }
    

    cfmakeraw(&tty);
    tcflush(serial_port, TCIFLUSH);

    baud_rate = _baud_rate;

    tcsetattr(serial_port, TCSANOW, &tty);

    if(tcsetattr(serial_port, TCSANOW, &tty) != 0){
        printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));

        return INITIALIZATION_NOT_SUCCESSFUL;
    }

    return 0;
}

int sendByte(uint8_t byte_to_send){
    if(baud_rate == 0){
        return RS485_NOT_INITIALIZED;
    }

    write(serial_port, &byte_to_send, 1);

    return 0;
}

int sendBuffer(uint8_t* buffer, uint16_t length){
    if(baud_rate == 0){
        return RS485_NOT_INITIALIZED;
    }

    write(serial_port, buffer, length);

    return 0;
}

int getByte(uint8_t* received_byte){
    int ret = read(serial_port, received_byte, 1);

    if(ret < 0){
        return FAILED_GETTING_BYTE;
    }else{
        return ret;
    }
}

int getBaudRate(){
    return baud_rate;
}