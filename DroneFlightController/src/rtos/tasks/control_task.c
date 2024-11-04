#include "control_task.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "sensors.h"
#include "pid_controller.h"
#include "motor_control.h"
#include "queue.h"

// Task handle
static TaskHandle_t controlTaskHandle = NULL;

// Semaphore for sensor data access
static SemaphoreHandle_t sensorDataSemaphore = NULL;

// Queue handle for PID commands
extern QueueHandle_t pidCommandQueue;

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
        
        // Handle PID commands from queue
        pid_command_t pidCommand;
        if(xQueueReceive(pidCommandQueue, &pidCommand, 0) == pdPASS) {
            // Process PID command
            ProcessPIDCommand(&pidCommand);
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

// PID task function to handle PID updates
void pid_task(void *pvParameters) {
    while(1) {
        // Wait for PID command from queue
        pid_command_t pidCommand;
        if(xQueueReceive(pidCommandQueue, &pidCommand, portMAX_DELAY) == pdPASS) {
            // Process PID command
            ProcessPIDCommand(&pidCommand);
        }
    }
}
