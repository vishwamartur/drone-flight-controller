//
//  rtos_init.h
//  DroneFlightController
//
//  Created by Vishwanath Martur on 11/1/24.
//

#ifndef rtos_init_h
#define rtos_init_h

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

// Task priorities
#define SENSOR_TASK_PRIORITY      (tskIDLE_PRIORITY + 3)
#define CONTROL_TASK_PRIORITY     (tskIDLE_PRIORITY + 2) 
#define TELEMETRY_TASK_PRIORITY   (tskIDLE_PRIORITY + 1)

// Task stack sizes
#define SENSOR_STACK_SIZE         (configMINIMAL_STACK_SIZE * 2)
#define CONTROL_STACK_SIZE        (configMINIMAL_STACK_SIZE * 2)
#define TELEMETRY_STACK_SIZE      (configMINIMAL_STACK_SIZE * 2)

// Function declarations for task initialization
void init_rtos_tasks(void);
void init_rtos_queues(void);
void init_rtos_semaphores(void);

// Task handle declarations
extern TaskHandle_t sensor_task_handle;
extern TaskHandle_t control_task_handle;
extern TaskHandle_t telemetry_task_handle;

// Queue handle declarations
extern QueueHandle_t sensor_data_queue;
extern QueueHandle_t control_command_queue;

// Semaphore handle declarations
extern SemaphoreHandle_t sensor_mutex;
extern SemaphoreHandle_t telemetry_mutex;

#endif /* rtos_init_h */
