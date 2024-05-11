#include "../include/ras.h"
#include "../../driver/uart.h"
#include <stdio.h>
#include <string.h>

#define MODEM_BAUD_RATE 115200
#define MODEM_TIMEOUT_MS 1000
#define UART_TX_PIN 1
#define UART_RX_PIN 3
// #define UART_FIFO_LEN 256

static bool modem_init(void) {
    uart_config_t uart_config = {
        .baud_rate = MODEM_BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
    uart_init(UART_NUM_1, &uart_config);

    const char* init_cmd = "AT\r\n";
    uart_write_bytes(UART_NUM_1, init_cmd, strlen(init_cmd));
    uart_wait_tx_done(UART_NUM_1, MODEM_TIMEOUT_MS);

    uint8_t response[64];
    int len = uart_read_bytes(UART_NUM_1, response, sizeof(response), MODEM_TIMEOUT_MS);
    if (len <= 0 || !strstr((const char*)response, "OK")) {
        return false;
    }

    return true;
}

static bool modem_transmit(const uint8_t* data, uint32_t size) {
    const char* transmit_cmd = "AT+CIPSEND=%d\r\n";
    char cmd[32];
    snprintf(cmd, sizeof(cmd), transmit_cmd, size);
    uart_write_bytes(UART_NUM_1, cmd, strlen(cmd));
    uart_wait_tx_done(UART_NUM_1, MODEM_TIMEOUT_MS);

    uart_write_bytes(UART_NUM_1, (const char*)data, size);
    uart_wait_tx_done(UART_NUM_1, MODEM_TIMEOUT_MS);

    uint8_t response[64];
    int len = uart_read_bytes(UART_NUM_1, response, sizeof(response), MODEM_TIMEOUT_MS);
    if (len <= 0 || !strstr((const char*)response, "OK")) {
        return false;
    }

    return true;
}

static bool modem_receive(uint8_t* buffer, uint32_t size, uint32_t* received_size) {
    const char* receive_cmd = "AT+CIPRXGET=2,%d\r\n";
    char cmd[32];
    snprintf(cmd, sizeof(cmd), receive_cmd, size);
    uart_write_bytes(UART_NUM_1, cmd, strlen(cmd));
    uart_wait_tx_done(UART_NUM_1, MODEM_TIMEOUT_MS);

    uint8_t response[256];
    int len = uart_read_bytes(UART_NUM_1, response, sizeof(response), MODEM_TIMEOUT_MS);
    if (len <= 0 || !strstr((const char*)response, "+CIPRXGET: 2")) {
        return false;
    }

    char* data_start = strstr((const char*)response, "\r\n");
    if (data_start == NULL) {
        return false;
    }
    data_start += 2;

    uint32_t data_len = len - (data_start - (const char*)response);
    if (data_len > size) {
        data_len = size;
    }

    memcpy(buffer, data_start, data_len);
    *received_size = data_len;

    return true;
}

static void log_error(const char* message) {
    fprintf(stderr, "RAS Error: %s\n", message);
}

void ras_init(void) {
    if (!modem_init()) {
        log_error("Modem initialization failed.");
        return;
    }
    printf("RAS layer initialized.\n");
}

ras_status_t ras_transmit_data(const uint8_t* data, uint32_t size) {
    if (data == NULL || size == 0 || size > RAS_MAX_DATA_SIZE) {
        return RAS_ERROR_INVALID_PARAMETER;
    }

    if (!modem_transmit(data, size)) {
        return RAS_ERROR_TRANSMISSION_FAILED;
    }

    return RAS_SUCCESS;
}

ras_status_t ras_receive_data(uint8_t* buffer, uint32_t size, uint32_t* received_size) {
    if (buffer == NULL || size == 0 || size > RAS_MAX_DATA_SIZE || received_size == NULL) {
        return RAS_ERROR_INVALID_PARAMETER;
    }

    if (!modem_receive(buffer, size, received_size)) {
        return RAS_ERROR_RECEPTION_FAILED;
    }

    return RAS_SUCCESS;
}

void ras_handle_error(ras_status_t error_code) {
    switch (error_code) {
        case RAS_SUCCESS:
            break;
        case RAS_ERROR_INVALID_PARAMETER:
            log_error("Invalid parameter.");
            break;
        case RAS_ERROR_TRANSMISSION_FAILED:
            log_error("Data transmission failed.");
            break;
        case RAS_ERROR_RECEPTION_FAILED:
            log_error("Data reception failed.");
            break;
        default:
            log_error("Unknown error.");
            break;
    }
}

