//
//  math_utils.c
//  DroneFlightController
//
//  Created by Vishwanath Martur on 11/1/24.
//

#include "math_utils.h"
#include <math.h>

// Constrain a value between min and max
float constrain(float value, float min, float max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

// Map a value from one range to another
float map(float value, float in_min, float in_max, float out_min, float out_max) {
    return (value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// Calculate moving average
float moving_average(float new_value, float *buffer, int buffer_size, int *buffer_index) {
    buffer[*buffer_index] = new_value;
    *buffer_index = (*buffer_index + 1) % buffer_size;
    
    float sum = 0;
    for(int i = 0; i < buffer_size; i++) {
        sum += buffer[i];
    }
    return sum / buffer_size;
}

// Convert degrees to radians
float deg_to_rad(float degrees) {
    return degrees * M_PI / 180.0f;
}

// Convert radians to degrees
float rad_to_deg(float radians) {
    return radians * 180.0f / M_PI;
}

// Calculate Euclidean distance between two 3D points
float euclidean_distance_3d(float x1, float y1, float z1, float x2, float y2, float z2) {
    float dx = x2 - x1;
    float dy = y2 - y1;
    float dz = z2 - z1;
    return sqrt(dx*dx + dy*dy + dz*dz);
}

// Low-pass filter implementation
float low_pass_filter(float input, float prev_output, float alpha) {
    return alpha * input + (1.0f - alpha) * prev_output;
}

// Deadband function to ignore small values
float apply_deadband(float value, float deadband) {
    if (fabs(value) < deadband) {
        return 0;
    }
    return value;
}
