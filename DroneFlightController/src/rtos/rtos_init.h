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
#define REMOTE_CONTROL_TASK_PRIORITY (tskIDLE_PRIORITY + 4) // Added for remote control task

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
extern QueueHandle_t pid_command_queue; // Added for PID command queue
extern QueueHandle_t remote_control_queue; // Added for remote control queue

// Semaphore handle declarations
extern SemaphoreHandle_t sensor_mutex;
extern SemaphoreHandle_t telemetry_mutex;

#endif /* rtos_init_h */
