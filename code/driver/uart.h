#ifndef DRIVER_UART_H
#define DRIVER_UART_H

#include <stdint.h>
#include <stdbool.h>

#define UART_NUM_0 0
#define UART_NUM_1 1

#define UART_FIFO_LEN 128

typedef enum {
    UART_DATA_5_BITS,
    UART_DATA_6_BITS,
    UART_DATA_7_BITS,
    UART_DATA_8_BITS
} uart_word_length_t;

typedef enum {
    UART_STOP_BITS_1,
    UART_STOP_BITS_1_5,
    UART_STOP_BITS_2
} uart_stop_bits_t;

typedef enum {
    UART_PARITY_DISABLE,
    UART_PARITY_EVEN,
    UART_PARITY_ODD
} uart_parity_t;

typedef enum {
    UART_HW_FLOWCTRL_DISABLE,
    UART_HW_FLOWCTRL_RTS,
    UART_HW_FLOWCTRL_CTS,
    UART_HW_FLOWCTRL_CTS_RTS
} uart_hw_flowcontrol_t;

typedef struct {
    uint32_t baud_rate;
    uart_word_length_t data_bits;
    uart_parity_t parity;
    uart_stop_bits_t stop_bits;
    uart_hw_flowcontrol_t flow_ctrl;
} uart_config_t;

void uart_init(uint8_t uart_num, const uart_config_t* config);
void uart_write_bytes(uint8_t uart_num, const char* data, uint32_t size);
int uart_read_bytes(uint8_t uart_num, uint8_t* buffer, uint32_t size, uint32_t timeout_ms);
void uart_wait_tx_done(uint8_t uart_num, uint32_t timeout_ms);

#endif // DRIVER_UART_H

