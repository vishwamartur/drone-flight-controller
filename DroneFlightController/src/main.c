#include <stdio.h>
#include <stdint.h>
#include "STM32F401.h"
#include "pid_controller.h"
#include "imu_sensor.h"
#include "esc.h"
#include "utils/math_utils.h"
#include "utils/logger.h"
#include "failsafe.h" // Added for emergency stop

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
    logger_init("flight.log", LOG_INFO, LOG_TO_FLASH);
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

    // Set initial PID values
    set_initial_pid_values(1.0f, 0.1f, 0.01f,  // Pitch PID values
                           1.0f, 0.1f, 0.01f,  // Roll PID values
                           1.0f, 0.1f, 0.01f); // Yaw PID values

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
        float roll_output = pid_compute(TARGET_ROLL, roll, 0.01f);
        float pitch_output = pid_compute(TARGET_PITCH, pitch, 0.01f);
        float yaw_output = pid_compute(TARGET_YAW, yaw, 0.01f);

        // Apply deadband to outputs
        roll_output = apply_deadband(roll_output, 0.05f);
        pitch_output = apply_deadband(pitch_output, 0.05f);
        yaw_output = apply_deadband(yaw_output, 0.05f);

        // Constrain outputs to valid range
        roll_output = constrain(roll_output, -1.0f, 1.0f);
        pitch_output = constrain(pitch_output, -1.0f, 1.0f);
        yaw_output = constrain(yaw_output, -1.0f, 1.0f);

        // Scale PID outputs to ESC input range (1000-2000)
        roll_output = map(roll_output, -1.0f, 1.0f, 1000.0f, 2000.0f);
        pitch_output = map(pitch_output, -1.0f, 1.0f, 1000.0f, 2000.0f);
        yaw_output = map(yaw_output, -1.0f, 1.0f, 1000.0f, 2000.0f);

        // Set ESC values based on PID output
        SetESC(ROLL_CHANNEL, roll_output);
        SetESC(PITCH_CHANNEL, pitch_output);
        SetESC(YAW_CHANNEL, yaw_output);

        // Store outputs for next iteration
        prev_roll_output = roll_output;
        prev_pitch_output = pitch_output;
        prev_yaw_output = yaw_output;

        // Log sensor readings, PID outputs, and motor speeds
        logger_log(LOG_INFO, __FILE__, __LINE__, "Sensor Readings - Roll: %f, Pitch: %f, Yaw: %f", roll, pitch, yaw);
        logger_log(LOG_INFO, __FILE__, __LINE__, "PID Outputs - Roll: %f, Pitch: %f, Yaw: %f", roll_output, pitch_output, yaw_output);
        logger_log(LOG_INFO, __FILE__, __LINE__, "Motor Speeds - Roll: %f, Pitch: %f, Yaw: %f", roll_output, pitch_output, yaw_output);

        // Check for emergency stop
        if (isFailsafeActive()) {
            emergency_stop();
        }

        // Delay to maintain fixed control loop frequency
        HAL_Delay(10); // 10ms delay for 100Hz control loop
    }
}

// System clock configuration
void SystemClock_Config(void) {
    // Configure system clock for 133MHz
    RCC->CR |= RCC_CR_HSEON; // Enable HSE
    while (!(RCC->CR & RCC_CR_HSERDY)); // Wait for HSE to be ready

    RCC->PLLCFGR = (8 << RCC_PLLCFGR_PLLM_Pos) | // PLLM = 8
                   (266 << RCC_PLLCFGR_PLLN_Pos) | // PLLN = 266
                   (0 << RCC_PLLCFGR_PLLP_Pos) | // PLLP = 2
                   (RCC_PLLCFGR_PLLSRC_HSE) | // PLL source = HSE
                   (7 << RCC_PLLCFGR_PLLQ_Pos); // PLLQ = 7

    RCC->CR |= RCC_CR_PLLON; // Enable PLL
    while (!(RCC->CR & RCC_CR_PLLRDY)); // Wait for PLL to be ready

    RCC->CFGR |= RCC_CFGR_SW_PLL; // Select PLL as system clock source
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL); // Wait for PLL to be used as system clock source
}

// GPIO initialization 
void GPIO_Init(void) {
    // Enable GPIO clocks
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();

    // Configure GPIO pins for sensor inputs, ESC outputs, and auxiliary functions
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // Sensor inputs
    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // ESC outputs
    GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    // Auxiliary functions
    GPIO_InitStruct.Pin = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
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
