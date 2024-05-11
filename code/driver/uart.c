#include "uart.h"
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/ioctl.h>

static int uart_fds[2] = {-1, -1};

void uart_init(uint8_t uart_num, const uart_config_t* config) {
    if (uart_num == UART_NUM_0) {
        uart_fds[uart_num] = open("/dev/tty.usbserial-1420", O_RDWR | O_NOCTTY | O_NDELAY);
    } else if (uart_num == UART_NUM_1) {
        uart_fds[uart_num] = open("/dev/tty.usbserial-1420", O_RDWR | O_NOCTTY | O_NDELAY);
    }

    if (uart_fds[uart_num] == -1) {
        return;
    }

    struct termios options;
    tcgetattr(uart_fds[uart_num], &options);

    options.c_cflag = CS8 | CREAD | CLOCAL;
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;

    switch (config->data_bits) {
        case UART_DATA_5_BITS:
            options.c_cflag |= CS5;
            break;
        case UART_DATA_6_BITS:
            options.c_cflag |= CS6;
            break;
        case UART_DATA_7_BITS:
            options.c_cflag |= CS7;
            break;
        case UART_DATA_8_BITS:
            options.c_cflag |= CS8;
            break;
    }

    switch (config->parity) {
        case UART_PARITY_DISABLE:
            options.c_cflag &= ~PARENB;
            break;
        case UART_PARITY_EVEN:
            options.c_cflag |= PARENB;
            options.c_cflag &= ~PARODD;
            break;
        case UART_PARITY_ODD:
            options.c_cflag |= PARENB | PARODD;
            break;
    }

    switch (config->stop_bits) {
        case UART_STOP_BITS_1:
            options.c_cflag &= ~CSTOPB;
            break;
        case UART_STOP_BITS_1_5:
            // Not supported, fallback to 1 stop bit
            options.c_cflag &= ~CSTOPB;
            break;
        case UART_STOP_BITS_2:
            options.c_cflag |= CSTOPB;
            break;
    }

    switch (config->flow_ctrl) {
        case UART_HW_FLOWCTRL_DISABLE:
            options.c_cflag &= ~CRTSCTS;
            break;
        case UART_HW_FLOWCTRL_RTS:
        case UART_HW_FLOWCTRL_CTS:
            // Not supported, fallback to no flow control
            options.c_cflag &= ~CRTSCTS;
            break;
        case UART_HW_FLOWCTRL_CTS_RTS:
            options.c_cflag |= CRTSCTS;
            break;
    }


    speed_t baud_rate;
    switch (config->baud_rate) {
        case 9600:
            baud_rate = B9600;
            break;
        case 19200:
            baud_rate = B19200;
            break;
        case 38400:
            baud_rate = B38400;
            break;
        case 57600:
            baud_rate = B57600;
            break;
        case 115200:
            baud_rate = B115200;
            break;
        default:
            baud_rate = B9600;
            break;
    }

    cfsetispeed(&options, baud_rate);
    cfsetospeed(&options, baud_rate);

    options.c_cc[VTIME] = 10;
    options.c_cc[VMIN] = 0;

    tcflush(uart_fds[uart_num], TCIFLUSH);
    tcsetattr(uart_fds[uart_num], TCSANOW, &options);
}

void uart_write_bytes(uint8_t uart_num, const char* data, uint32_t size) {
    if (uart_fds[uart_num] != -1) {
        write(uart_fds[uart_num], data, size);
    }
}

int uart_read_bytes(uint8_t uart_num, uint8_t* buffer, uint32_t size, uint32_t timeout_ms) {
    if (uart_fds[uart_num] != -1) {
        fd_set rfds;
        struct timeval tv;
        int ret;

        FD_ZERO(&rfds);
        FD_SET(uart_fds[uart_num], &rfds);

        tv.tv_sec = timeout_ms / 1000;
        tv.tv_usec = (timeout_ms % 1000) * 1000;

        ret = select(uart_fds[uart_num] + 1, &rfds, NULL, NULL, &tv);

        if (ret > 0) {
            if (FD_ISSET(uart_fds[uart_num], &rfds)) {
                return read(uart_fds[uart_num], buffer, size);
            }
        }
    }
    return -1;
}

void uart_wait_tx_done(uint8_t uart_num, uint32_t timeout_ms) {
    if (uart_fds[uart_num] != -1) {
        struct timeval timeout;
        timeout.tv_sec = timeout_ms / 1000;
        timeout.tv_usec = (timeout_ms % 1000) * 1000;
        select(0, NULL, NULL, NULL, &timeout);
    }
}

