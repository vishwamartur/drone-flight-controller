//
//  rtos_init.c
//  DroneFlightController
//
//  Created by Vishwanath Martur on 11/1/24.
//

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

// Task handles
TaskHandle_t sensorTaskHandle;
TaskHandle_t controlTaskHandle;
TaskHandle_t telemetryTaskHandle;

// Queue handles
QueueHandle_t sensorDataQueue;
QueueHandle_t controlCommandQueue;

// Semaphore handles
SemaphoreHandle_t i2cBusSemaphore;
SemaphoreHandle_t spiBusSemaphore;

// RTOS initialization function
void rtos_init(void) {
    // Create queues
    sensorDataQueue = xQueueCreate(10, sizeof(sensor_data_t));
    controlCommandQueue = xQueueCreate(5, sizeof(control_command_t));
    
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
                
    // Start the scheduler
    vTaskStartScheduler();
    
    // Should never reach here
    while(1) {}
}
