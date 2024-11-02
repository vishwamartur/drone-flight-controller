//
//  math_utils.h
//  DroneFlightController
//
//  Created by Vishwanath Martur on 11/1/24.
//

#ifndef math_utils_h
#define math_utils_h

// Constrain a value between min and max
float constrain(float value, float min, float max);

// Map a value from one range to another
float map(float value, float in_min, float in_max, float out_min, float out_max);

// Calculate moving average
float moving_average(float new_value, float *buffer, int buffer_size, int *buffer_index);

// Convert degrees to radians
float deg_to_rad(float degrees);

// Convert radians to degrees 
float rad_to_deg(float radians);

// Calculate Euclidean distance between two 3D points
float euclidean_distance_3d(float x1, float y1, float z1, float x2, float y2, float z2);

// Low-pass filter implementation
float low_pass_filter(float input, float prev_output, float alpha);

// Deadband function to ignore small values
float apply_deadband(float value, float deadband);

#endif /* math_utils_h */
