#ifndef RAS_PYTHON_INTERFACE_H
#define RAS_PYTHON_INTERFACE_H

#include <stdint.h>

void ras_init_python_models(void);
void ras_destroy_python_models(void);
double ras_run_python_simulation(uint32_t num_subcarriers, uint32_t num_symbols, uint32_t num_antennas,
                                 const char* model_type, const char* channel_type, const char* modulation_type);

#endif // RAS_PYTHON_INTERFACE_H

