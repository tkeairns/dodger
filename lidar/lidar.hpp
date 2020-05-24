#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>



void lidar_poll_1kHz();

uint16_t lidar_get_distance_cm();

double lidar_get_distance_filt_cm();

#ifdef __cplusplus
} /* extern "C" */
#endif
