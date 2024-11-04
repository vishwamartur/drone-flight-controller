#include <stdio.h>
#include <stdint.h>
#include "STM32F401.h"
#include "pid_controller.h"
#include "imu_sensor.h"
#include "esc.h"
#include "utils/math_utils.h"
#include "utils/logger.h"

// Constants for target angles and channels
#define TARGET_ROLL     0.0f   // Target roll angle in degrees
#define TARGET_PITCH    0.0f   // Target pitch angle in degrees  
#define TARGET_YAW      0.0f   // Target yaw angle in degrees

#define ROLL_CHANNEL    0
#define PITCH_CHANNEL   1
#define YAW_CHANNEL     2

// Function prototypes
void SystemClock_Config(void);
void GPIO_Init(void);
void UART_Init(void);
void IMU_Init(void);
void ESC_Init(void);

int main(void) {
    // Initialize logger
    logger_init("flight.log", LOG_INFO);
    logger_log(LOG_INFO, __FILE__, __LINE__, "System startup");

    // Initialize the system clock
    SystemClock_Config();
    logger_log(LOG_INFO, __FILE__, __LINE__, "Clock configured");

    // Initialize peripherals
    GPIO_Init();
    UART_Init();
    IMU_Init();
    ESC_Init();
    
    logger_log(LOG_INFO, __FILE__, __LINE__, "All peripherals initialized");

    float prev_roll_output = 0.0f;
    float prev_pitch_output = 0.0f;
    float prev_yaw_output = 0.0f;

    // Main control loop
    while (1) {
        // Read IMU data
        float roll, pitch, yaw;
        ReadIMU(&roll, &pitch, &yaw);

        // Apply low-pass filter to smooth sensor readings
        roll = low_pass_filter(roll, prev_roll_output, 0.2f);
        pitch = low_pass_filter(pitch, prev_pitch_output, 0.2f);
        yaw = low_pass_filter(yaw, prev_yaw_output, 0.2f);

        // Update PID controllers for each axis
        float roll_output = UpdatePID(roll, TARGET_ROLL);
        float pitch_output = UpdatePID(pitch, TARGET_PITCH);
        float yaw_output = UpdatePID(yaw, TARGET_YAW);

        // Apply deadband to outputs
        roll_output = apply_deadband(roll_output, 0.05f);
        pitch_output = apply_deadband(pitch_output, 0.05f);
        yaw_output = apply_deadband(yaw_output, 0.05f);

        // Constrain outputs to valid range
        roll_output = constrain(roll_output, -1.0f, 1.0f);
        pitch_output = constrain(pitch_output, -1.0f, 1.0f);
        yaw_output = constrain(yaw_output, -1.0f, 1.0f);

        // Set ESC values based on PID output
        SetESC(ROLL_CHANNEL, roll_output);
        SetESC(PITCH_CHANNEL, pitch_output);
        SetESC(YAW_CHANNEL, yaw_output);

        // Store outputs for next iteration
        prev_roll_output = roll_output;
        prev_pitch_output = pitch_output;
        prev_yaw_output = yaw_output;
    }
}

// System clock configuration
void SystemClock_Config(void) {
    // Configure system clock for maximum performance
    // Implementation specific to STM32F401
}

// GPIO initialization 
void GPIO_Init(void) {
    // Configure GPIO pins for ESCs, sensors etc
    // Implementation specific to hardware setup
}

// UART initialization for debug output
void UART_Init(void) {
    // Configure UART for debugging
    // Implementation specific to hardware setup
}

// IMU sensor initialization
void IMU_Init(void) {
    // Initialize and calibrate IMU sensor
    IMUSensor imu;
    if (!imu.initialize()) {
        logger_log(LOG_ERROR, __FILE__, __LINE__, "IMU initialization failed");
        return;
    }
    if (!imu.calibrate()) {
        logger_log(LOG_ERROR, __FILE__, __LINE__, "IMU calibration failed");
        return;
    }
}

// ESC initialization
void ESC_Init(void) {
    // Initialize ESC communication
    esc_config_t esc_config = {1, 1000, 2000, 1500};
    if (esc_init(&esc_config) != ESC_SUCCESS) {
        logger_log(LOG_ERROR, __FILE__, __LINE__, "ESC initialization failed");
        return;
    }
}
