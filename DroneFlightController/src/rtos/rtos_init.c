#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "pid_controller.h"

// Task handles
TaskHandle_t sensorTaskHandle;
TaskHandle_t controlTaskHandle;
TaskHandle_t telemetryTaskHandle;
TaskHandle_t pidTaskHandle; // Added for PID task
TaskHandle_t remoteControlTaskHandle; // Added for remote control task

// Queue handles
QueueHandle_t sensorDataQueue;
QueueHandle_t controlCommandQueue;
QueueHandle_t pidCommandQueue; // Added for PID command queue
QueueHandle_t remoteControlQueue; // Added for remote control queue

// Semaphore handles
SemaphoreHandle_t i2cBusSemaphore;
SemaphoreHandle_t spiBusSemaphore;

// PID task function to handle PID updates
void pid_task(void *pvParameters) {
    // Set initial PID values
    set_initial_pid_values(1.0f, 0.1f, 0.01f,  // Pitch PID values
                           1.0f, 0.1f, 0.01f,  // Roll PID values
                           1.0f, 0.1f, 0.01f); // Yaw PID values

    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();

    while(1) {
        // Update PID loop at a fixed frequency
        // Add your PID update logic here

        // Wait for the next cycle
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(10)); // 10ms delay for 100Hz update rate
    }
}

// RTOS initialization function
void rtos_init(void) {
    // Create queues
    sensorDataQueue = xQueueCreate(10, sizeof(sensor_data_t));
    controlCommandQueue = xQueueCreate(5, sizeof(control_command_t));
    pidCommandQueue = xQueueCreate(5, sizeof(pid_command_t)); // Added for PID command queue
    remoteControlQueue = xQueueCreate(5, sizeof(remote_control_t)); // Added for remote control queue
    
    // Create semaphores
    i2cBusSemaphore = xSemaphoreCreateMutex();
    spiBusSemaphore = xSemaphoreCreateMutex();
    
    // Create tasks
    xTaskCreate(sensor_task,
                "SensorTask",
                configMINIMAL_STACK_SIZE * 2,
                NULL,
                tskIDLE_PRIORITY + 3,
                &sensorTaskHandle);
                
    xTaskCreate(control_task,
                "ControlTask",
                configMINIMAL_STACK_SIZE * 2,
                NULL,
                tskIDLE_PRIORITY + 2,
                &controlTaskHandle);
                
    xTaskCreate(telemetry_task,
                "TelemetryTask",
                configMINIMAL_STACK_SIZE * 2,
                NULL,
                tskIDLE_PRIORITY + 1,
                &telemetryTaskHandle);
                
    xTaskCreate(pid_task, // Added for PID task
                "PIDTask",
                configMINIMAL_STACK_SIZE * 2,
                NULL,
                CONTROL_TASK_PRIORITY,
                &pidTaskHandle);
                
    xTaskCreate(remote_control_task, // Added for remote control task
                "RemoteControlTask",
                configMINIMAL_STACK_SIZE * 2,
                NULL,
                REMOTE_CONTROL_TASK_PRIORITY,
                &remoteControlTaskHandle);
                
    // Start the scheduler
    vTaskStartScheduler();
    
    // Should never reach here
    while(1) {}
}
