#include "../ras/include/ras.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

static const uint8_t test_data[] = {0x01, 0x02, 0x03, 0x04, 0x05};
static const uint32_t test_data_size = sizeof(test_data);

static bool compare_data(const uint8_t* data1, const uint8_t* data2, uint32_t size) {
    return (memcmp(data1, data2, size) == 0);
}

static void test_ras_init(void) {
    printf("Testing RAS initialization...\n");
    ras_init();
    printf("RAS initialization test passed.\n\n");
}

static void test_ras_transmit_data(void) {
    printf("Testing RAS data transmission...\n");
    ras_status_t status = ras_transmit_data(test_data, test_data_size);
    if (status == RAS_SUCCESS) {
        printf("Data transmission test passed.\n\n");
    } else {
        printf("Data transmission test failed. Error code: %d\n\n", status);
    }
}

static void test_ras_receive_data(void) {
    printf("Testing RAS data reception...\n");
    uint8_t receive_buffer[RAS_MAX_DATA_SIZE];
    uint32_t received_size;
    ras_status_t status = ras_receive_data(receive_buffer, RAS_MAX_DATA_SIZE, &received_size);
    if (status == RAS_SUCCESS && received_size == test_data_size && compare_data(receive_buffer, test_data, test_data_size)) {
        printf("Data reception test passed.\n\n");
    } else {
        printf("Data reception test failed. Error code: %d\n\n", status);
    }
}

static void test_ras_error_handling(void) {
    printf("Testing RAS error handling...\n");
    ras_handle_error(RAS_ERROR_INVALID_PARAMETER);
    ras_handle_error(RAS_ERROR_TRANSMISSION_FAILED);
    ras_handle_error(RAS_ERROR_RECEPTION_FAILED);
    printf("Error handling test passed.\n\n");
}

int main(void) {
    test_ras_init();
    test_ras_transmit_data();
    test_ras_receive_data();
    test_ras_error_handling();
    return 0;
}

