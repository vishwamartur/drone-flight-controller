//
//  sensor_fusion.h
//  DroneFlightController
//
//  Created by Vishwanath Martur on 11/1/24.
//

#ifndef sensor_fusion_h
#define sensor_fusion_h

#include <stdbool.h>

// Initialize the sensor fusion system
bool initializeSensorFusion(void);

// Update orientation estimates using sensor fusion
void updateOrientation(float dt);

// Get the current orientation estimates
void getOrientation(float* roll, float* pitch, float* yaw);

// Get the current angular rates
void getAngularRates(float* roll_rate, float* pitch_rate, float* yaw_rate);

// Reset the sensor fusion state
void resetSensorFusion(void);

// Internal Kalman filter update function
static void updateKalmanFilter(int index, float measurement, float gyro_rate, float dt);

#endif /* sensor_fusion_h */
