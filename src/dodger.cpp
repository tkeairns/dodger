#include <linux/types.h>
#include <cstdio>
#include <stdint.h>
#include <time.h>

#include "lidar/lidarlite_v3.hpp"

LIDARLite_v3 myLidarLite;

// Needs to be reachable from python ctypes
extern "C" {

uint8_t busyFlag;
uint16_t distance;
double distance_filt;


#define FACTOR 0.5 // Needs to be [0, 1] 
double lpf_exp(double input, double average, double factor)
{
    return (input * factor) + ((1 - factor) * average);
}

/// LPF Constant = LPF_CUT_OFF_FREQ * 2 * PI * CALL_PERIOD
#define FILTER_1_100 0.0628F
double lpf_freq(double input, double state, double lpf_constant)
{
    return state + ((input - state) * lpf_constant);
}


int foo()
{
    // Initialize i2c peripheral in the cpu core
    myLidarLite.i2c_init();

    // Optionally configure LIDAR-Lite
    myLidarLite.configure(0);

    bool init_filter = true;

    while(1)
    {
        // Each time through the loop, check BUSY
        busyFlag = myLidarLite.getBusyFlag();

        if (busyFlag == 0x00)
        {
            // When no longer busy, immediately initialize another measurement
            // and then read the distance data from the last measurement.
            // This method will result in faster I2C rep rates.
            myLidarLite.takeRange();

            if (init_filter)
            {
                init_filter = false;
                distance = myLidarLite.readDistance();
                distance_filt = distance;
            }
            else 
            {
                distance = myLidarLite.readDistance();
                distance_filt = lpf_freq(distance, distance_filt, FILTER_1_100);
            }

            printf("%4d,%4d\n", distance, (int)distance_filt); // This data can be stored in a *.csv with a stdout >> redirect to csv file
        }

        static const timespec poll_freq = {
            tv_sec : 0,
            tv_nsec : POLL_FREQUENCY,
        };
        nanosleep(&poll_freq, nullptr);
    }
}

} /* extern C */