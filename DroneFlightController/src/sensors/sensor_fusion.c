//
//  sensor_fusion.c
//  DroneFlightController
//
//  Created by Vishwanath Martur on 11/1/24.
//

#include "sensor_fusion.h"
#include "imu_sensor.h"
#include "FreeRTOS.h"
#include "semphr.h"

// Kalman filter state variables
static float angle[3] = {0.0f, 0.0f, 0.0f}; // Roll, pitch, yaw
static float bias[3] = {0.0f, 0.0f, 0.0f};  // Gyro bias estimates
static float P[3][2][2];  // Error covariance matrix

// Kalman filter parameters
static const float Q_angle = 0.001f;    // Process noise for angle
static const float Q_bias = 0.003f;     // Process noise for bias
static const float R_measure = 0.03f;   // Measurement noise

// IMU sensor instance
static IMUSensor imu;

bool initializeSensorFusion() {
    // Initialize IMU
    if (!imu.initialize()) {
        return false;
    }
    
    // Initialize error covariance matrices
    for (int i = 0; i < 3; i++) {
        P[i][0][0] = 0.0f;
        P[i][0][1] = 0.0f;
        P[i][1][0] = 0.0f;
        P[i][1][1] = 0.0f;
    }
    
    return true;
}

void updateOrientation(float dt) {
    float accel_x, accel_y, accel_z;
    float gyro_x, gyro_y, gyro_z;
    
    // Read sensor data
    imu.readAccelerometer(accel_x, accel_y, accel_z);
    imu.readGyroscope(gyro_x, gyro_y, gyro_z);
    
    // Calculate angles from accelerometer
    float accel_roll = atan2f(accel_y, accel_z);
    float accel_pitch = atan2f(-accel_x, sqrtf(accel_y * accel_y + accel_z * accel_z));
    
    // Update each angle using Kalman filter
    updateKalmanFilter(0, accel_roll, gyro_x, dt);  // Roll
    updateKalmanFilter(1, accel_pitch, gyro_y, dt); // Pitch
    
    // Simple complementary filter for yaw using gyro
    angle[2] += (gyro_z - bias[2]) * dt;
}

static void updateKalmanFilter(int index, float measurement, float gyro_rate, float dt) {
    // Predict
    float rate = gyro_rate - bias[index];
    angle[index] += dt * rate;
    
    P[index][0][0] += dt * (dt * P[index][1][1] - P[index][0][1] - P[index][1][0] + Q_angle);
    P[index][0][1] -= dt * P[index][1][1];
    P[index][1][0] -= dt * P[index][1][1];
    P[index][1][1] += Q_bias * dt;
    
    // Update
    float y = measurement - angle[index];
    float S = P[index][0][0] + R_measure;
    float K[2] = {P[index][0][0] / S, P[index][1][0] / S};
    
    angle[index] += K[0] * y;
    bias[index] += K[1] * y;
    
    float P00_temp = P[index][0][0];
    float P01_temp = P[index][0][1];
    
    P[index][0][0] -= K[0] * P00_temp;
    P[index][0][1] -= K[0] * P01_temp;
    P[index][1][0] -= K[1] * P00_temp;
    P[index][1][1] -= K[1] * P01_temp;
}

void getFilteredOrientation(float* roll, float* pitch, float* yaw) {
    *roll = angle[0];
    *pitch = angle[1];
    *yaw = angle[2];
}
