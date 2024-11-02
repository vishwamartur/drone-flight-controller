//
//  control_task.c
//  DroneFlightController
//
//  Created by Vishwanath Martur on 11/1/24.
//

#include "control_task.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "sensors.h"
#include "pid_controller.h"
#include "motor_control.h"

// Task handle
static TaskHandle_t controlTaskHandle = NULL;

// Semaphore for sensor data access
static SemaphoreHandle_t sensorDataSemaphore = NULL;

// Control loop parameters
#define CONTROL_TASK_STACK_SIZE 2048
#define CONTROL_TASK_PRIORITY 3
#define CONTROL_LOOP_FREQUENCY_HZ 100
#define CONTROL_LOOP_PERIOD_MS (1000 / CONTROL_LOOP_FREQUENCY_HZ)

// Control task implementation
static void ControlTask(void *pvParameters) {
    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    
    while(1) {
        // Wait for sensor data to be ready
        if(xSemaphoreTake(sensorDataSemaphore, pdMS_TO_TICKS(10)) == pdTRUE) {
            // Read sensor data
            SensorData_t sensorData = GetSensorData();
            
            // Run PID calculations
            PIDOutput_t pidOutput = UpdatePIDControllers(&sensorData);
            
            // Update motor speeds
            UpdateMotorSpeeds(&pidOutput);
            
            xSemaphoreGive(sensorDataSemaphore);
        }
        
        // Wait for next control loop iteration
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(CONTROL_LOOP_PERIOD_MS));
    }
}

// Initialize control task
BaseType_t InitControlTask(void) {
    // Create semaphore
    sensorDataSemaphore = xSemaphoreCreateMutex();
    if(sensorDataSemaphore == NULL) {
        return pdFAIL;
    }
    
    // Create task
    BaseType_t result = xTaskCreate(
        ControlTask,
        "Control Task",
        CONTROL_TASK_STACK_SIZE,
        NULL,
        CONTROL_TASK_PRIORITY,
        &controlTaskHandle
    );
    
    return result;
}
