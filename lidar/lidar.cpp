#include <linux/types.h>
#include <cstdio>
#include <time.h>

#include "lidar/lidar.hpp"
#include "lidar/lidarlite_v3.hpp"



#define MS_TO_NS(ms)   ((ms) * (10 ^ 6))      
#define POLL_FREQUENCY_NS MS_TO_NS(1U)
#define FILTER_EXPONENTIAL_FACTOR 0.5
#define FILTER_FREQUENCY_1_100 0.0628F

LIDARLite_v3 lidar;

extern "C" {

uint16_t distance_cm;
double distance_filt_cm;

/// LPF Factor needs to be (0, 1)
double lpf_exp(double input, double average, double factor)
{
    return (input * factor) + ((1 - factor) * average);
}

/// LPF Constant = LPF_CUT_OFF_FREQ * 2 * PI * CALL_PERIOD
double lpf_freq(double input, double state, double lpf_constant)
{
    return state + ((input - state) * lpf_constant);
}

void lidar_poll_1kHz()
{
    // Initialize i2c peripheral in the cpu core
    lidar.i2c_init();

    // Optionally configure LIDAR-Lite
    lidar.configure(0);

    bool init_filter = true;

    while (1)
    {
        if (0x00 == lidar.getBusyFlag())
        {
            // When no longer busy, immediately initialize another measurement
            // and then read the distance data from the last measurement.
            // This method will result in faster I2C rep rates.
            lidar.takeRange();

            if (init_filter)
            {
                init_filter = false;
                distance_cm = lidar.readDistance();
                distance_filt_cm = distance_cm;
            }
            else
            {
                distance_cm = lidar.readDistance();
                distance_filt_cm = lpf_freq(distance_cm, distance_filt_cm, FILTER_FREQUENCY_1_100);
            }

            // printf("%4d,%4d\n", distance_cm, (int)distance_filt_cm); // This data can be stored in a *.csv with a stdout >> redirect to csv file
        }

        static const timespec poll_freq = {
            tv_sec : 0,
            tv_nsec : POLL_FREQUENCY_NS,
        };
        nanosleep(&poll_freq, nullptr);
    }
}

uint16_t lidar_get_distance_cm()
{
    return distance_cm;
}

double lidar_get_distance_filt_cm()
{
    return distance_filt_cm;
}

} /* extern C */