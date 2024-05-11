#ifndef RAS_H
#define RAS_H

#include <stdint.h>
#include <stdbool.h>

#define RAS_MAX_DATA_SIZE 1024

typedef enum {
    RAS_SUCCESS,
    RAS_ERROR_INVALID_PARAMETER,
    RAS_ERROR_TRANSMISSION_FAILED,
    RAS_ERROR_RECEPTION_FAILED
} ras_status_t;

void ras_init(void);
ras_status_t ras_transmit_data(const uint8_t* data, uint32_t size);
ras_status_t ras_receive_data(uint8_t* buffer, uint32_t size, uint32_t* received_size);
void ras_handle_error(ras_status_t error_code);

#endif // RAS_H

