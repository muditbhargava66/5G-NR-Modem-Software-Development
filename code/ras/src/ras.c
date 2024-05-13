#include "../include/ras.h"
#include "../../driver/uart.h"
#include <stdio.h>
#include <string.h>
#include "ras_python_interface.h"
#include <Python.h>

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

    // print the received data
    printf("Transmitting data: ");
    for (uint32_t i = 0; i < size; i++) {
        printf("%02X ", data[i]);
    }
    printf("\n");

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
    printf("Initializing RAS layer...\n");
    if (!modem_init()) {
        log_error("Modem initialization failed.");
        printf("RAS initialization failed.\n");
        return;
    }
    printf("RAS layer initialized successfully.\n");

    // Initialize Python
    Py_Initialize();

    // Redirect Python stdout and stderr to C stdout and stderr
    PySys_SetObject("stdout", PySys_GetObject("stderr"));

    // python module
    ras_init_python_models();
}

void ras_destroy(void) {
    ras_destroy_python_models();

    // Finalize Python
    Py_Finalize();
}

ras_status_t ras_transmit_data(const uint8_t* data, uint32_t size) {
    if (data == NULL || size == 0 || size > RAS_MAX_DATA_SIZE) {
        return RAS_ERROR_INVALID_PARAMETER;
    }

    if (!modem_transmit(data, size)) {
        return RAS_ERROR_TRANSMISSION_FAILED;
    }

    double ber_result = ras_run_python_simulation(64, 100, 4, "ras", "rayleigh", "qpsk");
    if (ber_result >= 0) {
        printf("Python simulation BER: %f\n", ber_result);
        if (ber_result > 0.1) {
            printf("Warning: BER exceeds the acceptable threshold of 0.1\n");
            printf("Consider adjusting the transmission parameters or improving the channel conditions\n");
        } else {
            printf("BER is within the acceptable range\n");
        }
    } else {
        printf("Python simulation failed.\n");
        printf("Error: Unable to obtain BER result from the Python simulation\n");
        printf("Please check the Python simulation setup and ensure it is running correctly\n");
        return RAS_ERROR_SIMULATION_FAILED;
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
        case RAS_ERROR_SIMULATION_FAILED:
            log_error("Python simulation failed.");
            break;
        default:
            log_error("Unknown error.");
            break;
    }
}

// Python-C API functions
double ras_run_python_simulation(uint32_t num_subcarriers, uint32_t num_symbols, uint32_t num_antennas,
                                 const char* model_type, const char* channel_type, const char* modulation_type) {
    PyObject* pModule = NULL;
    PyObject* pFunc = NULL;
    PyObject* pArgs = NULL;
    PyObject* pResult = NULL;

    // Import the Python module
    pModule = PyImport_ImportModule("ras_simulation");
    if (pModule == NULL) {
        PyErr_Print();
        fprintf(stderr, "Failed to import the RASSimulation module.\n");
        return -1.0;
    }

    // Get the Python function
    pFunc = PyObject_GetAttrString(pModule, "run_simulation");
    if (pFunc == NULL) {
        PyErr_Print();
        fprintf(stderr, "Failed to get the run_simulation function.\n");
        Py_DECREF(pModule);
        return -1.0;
    }

    // Create the argument tuple
    pArgs = PyTuple_New(6);
    PyTuple_SetItem(pArgs, 0, PyLong_FromUnsignedLong(num_subcarriers));
    PyTuple_SetItem(pArgs, 1, PyLong_FromUnsignedLong(num_symbols));
    PyTuple_SetItem(pArgs, 2, PyLong_FromUnsignedLong(num_antennas));
    PyTuple_SetItem(pArgs, 3, PyUnicode_FromString(model_type));
    PyTuple_SetItem(pArgs, 4, PyUnicode_FromString(channel_type));
    PyTuple_SetItem(pArgs, 5, PyUnicode_FromString(modulation_type));

    // Call the Python function
    pResult = PyObject_CallObject(pFunc, pArgs);
    if (pResult == NULL) {
        PyErr_Print();
        fprintf(stderr, "Failed to call the run_simulation function.\n");
        Py_DECREF(pArgs);
        Py_DECREF(pFunc);
        Py_DECREF(pModule);
        return -1.0;
    }

    // Get the result as a double
    double ber_result = PyFloat_AsDouble(pResult);
    Py_DECREF(pResult);
    Py_DECREF(pArgs);
    Py_DECREF(pFunc);
    Py_DECREF(pModule);

    return ber_result;
}

void ras_init_python_models(void) {
    // Initialize Python models
    // ...
}

void ras_destroy_python_models(void) {
    // Destroy Python models
    // ...
}

