//
//  sensor_task.c
//  DroneFlightController
//
//  Created by Vishwanath Martur on 11/1/24.
//

#include "sensor_task.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "imu.h"
#include "barometer.h"
#include "gps.h"

// Task handle
static TaskHandle_t sensor_task_handle = NULL;

// Queue handles for sensor data
static QueueHandle_t imu_queue = NULL;
static QueueHandle_t baro_queue = NULL;
static QueueHandle_t gps_queue = NULL;

// Semaphores for sensor access
static SemaphoreHandle_t imu_semaphore = NULL;
static SemaphoreHandle_t baro_semaphore = NULL;
static SemaphoreHandle_t gps_semaphore = NULL;

// Sensor task function
static void sensor_task(void *pvParameters) {
    // Initialize sensors
    imu_init();
    barometer_init();
    gps_init();
    
    // Create queues
    imu_queue = xQueueCreate(5, sizeof(imu_data_t));
    baro_queue = xQueueCreate(5, sizeof(baro_data_t));
    gps_queue = xQueueCreate(5, sizeof(gps_data_t));
    
    // Create semaphores
    imu_semaphore = xSemaphoreCreateMutex();
    baro_semaphore = xSemaphoreCreateMutex();
    gps_semaphore = xSemaphoreCreateMutex();
    
    while(1) {
        // Read IMU data
        if(xSemaphoreTake(imu_semaphore, portMAX_DELAY) == pdTRUE) {
            imu_data_t imu_data;
            if(imu_read(&imu_data) == 0) {
                xQueueSend(imu_queue, &imu_data, 0);
            }
            xSemaphoreGive(imu_semaphore);
        }
        
        // Read barometer data
        if(xSemaphoreTake(baro_semaphore, portMAX_DELAY) == pdTRUE) {
            baro_data_t baro_data;
            if(barometer_read(&baro_data) == 0) {
                xQueueSend(baro_queue, &baro_data, 0);
            }
            xSemaphoreGive(baro_semaphore);
        }
        
        // Read GPS data
        if(xSemaphoreTake(gps_semaphore, portMAX_DELAY) == pdTRUE) {
            gps_data_t gps_data;
            if(gps_read(&gps_data) == 0) {
                xQueueSend(gps_queue, &gps_data, 0);
            }
            xSemaphoreGive(gps_semaphore);
        }
        
        // Delay for next sensor reading
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

// Function to start sensor task
void start_sensor_task(void) {
    xTaskCreate(sensor_task,
                "Sensor Task",
                configMINIMAL_STACK_SIZE,
                NULL,
                tskIDLE_PRIORITY + 1,
                &sensor_task_handle);
}

// Function to get queue handles
QueueHandle_t get_imu_queue(void) { return imu_queue; }
QueueHandle_t get_baro_queue(void) { return baro_queue; }
QueueHandle_t get_gps_queue(void) { return gps_queue; }

// Function to get semaphore handles
SemaphoreHandle_t get_imu_semaphore(void) { return imu_semaphore; }
SemaphoreHandle_t get_baro_semaphore(void) { return baro_semaphore; }
SemaphoreHandle_t get_gps_semaphore(void) { return gps_semaphore; }
