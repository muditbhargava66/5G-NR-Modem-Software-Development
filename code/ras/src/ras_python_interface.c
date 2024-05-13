#include "ras_python_interface.h"
#include <Python.h>

static PyObject* ras_simulation_module = NULL;
static PyObject* ras_simulation_class = NULL;
static PyObject* ras_simulation_instance = NULL;

void ras_init_python_models(void) {
    Py_Initialize();

    // Add the project root directory to the Python module search path
    PyObject* sys_path = PySys_GetObject("path");
    PyList_Append(sys_path, PyUnicode_FromString("../simulations"));

    // Import the RASSimulation module
    ras_simulation_module = PyImport_ImportModule("ras_simulation");
    if (ras_simulation_module == NULL) {
        fprintf(stderr, "Failed to import the RASSimulation module.\n");
        PyErr_Print();
        return;
    }

    // Get the RASSimulation class from the module
    ras_simulation_class = PyObject_GetAttrString(ras_simulation_module, "RASSimulation");
    if (ras_simulation_class == NULL) {
        fprintf(stderr, "Failed to get the RASSimulation class.\n");
        PyErr_Print();
        return;
    }
}

void ras_destroy_python_models(void) {
    // Decrement the reference count of the RASSimulation instance, class, and module
    Py_XDECREF(ras_simulation_instance);
    Py_XDECREF(ras_simulation_class);
    Py_XDECREF(ras_simulation_module);

    // Finalize the Python interpreter
    Py_Finalize();
}

double ras_run_python_simulation(uint32_t num_subcarriers, uint32_t num_symbols, uint32_t num_antennas,
                                  const char* model_type, const char* channel_type, const char* modulation_type) {
    if (ras_simulation_class == NULL) {
        fprintf(stderr, "RASSimulation class is not initialized.\n");
        return -1.0;
    }

    // Create arguments tuple for the RASSimulation constructor
    PyObject* args = PyTuple_New(6);
    PyTuple_SetItem(args, 0, PyLong_FromUnsignedLong(num_subcarriers));
    PyTuple_SetItem(args, 1, PyLong_FromUnsignedLong(num_symbols));
    PyTuple_SetItem(args, 2, PyLong_FromUnsignedLong(num_antennas));
    PyTuple_SetItem(args, 3, PyUnicode_FromString(model_type));
    PyTuple_SetItem(args, 4, PyUnicode_FromString(channel_type));
    PyTuple_SetItem(args, 5, PyUnicode_FromString(modulation_type));

    // Create an instance of the RASSimulation class
    ras_simulation_instance = PyObject_CallObject(ras_simulation_class, args);
    Py_DECREF(args);

    if (ras_simulation_instance == NULL) {
        fprintf(stderr, "Failed to create an instance of the RASSimulation class.\n");
        PyErr_Print();
        return -1.0;
    }

    // Call the run_simulation method of the RASSimulation instance
    PyObject* result = PyObject_CallMethod(ras_simulation_instance, "run_simulation", "(ii)", 1000, 4);
    if (result == NULL) {
        fprintf(stderr, "Failed to call the run_simulation method.\n");
        PyErr_Print();
        return -1.0;
    }

    // Get the average BER result from the Python simulation
    double ber_result = PyFloat_AsDouble(result);
    Py_DECREF(result);

    // Check if an error occurred during the conversion
    if (PyErr_Occurred()) {
        fprintf(stderr, "Error occurred while getting the BER result.\n");
        PyErr_Print();
        return -1.0;
    }

    return ber_result;
}

