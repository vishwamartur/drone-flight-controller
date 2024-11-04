#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "remote_control.h"

// Task handle
static TaskHandle_t remoteControlTaskHandle = NULL;

// Queue handle for remote control inputs
extern QueueHandle_t remoteControlQueue;

// Remote control task implementation
static void remote_control_task(void *pvParameters) {
    while(1) {
        // Wait for remote control input from queue
        remote_control_t remoteControlInput;
        if(xQueueReceive(remoteControlQueue, &remoteControlInput, portMAX_DELAY) == pdPASS) {
            // Process remote control input
            ProcessRemoteControlInput(&remoteControlInput);
        }
    }
}

// Initialize remote control task
BaseType_t InitRemoteControlTask(void) {
    // Create task
    BaseType_t result = xTaskCreate(
        remote_control_task,
        "Remote Control Task",
        configMINIMAL_STACK_SIZE,
        NULL,
        REMOTE_CONTROL_TASK_PRIORITY,
        &remoteControlTaskHandle
    );
    
    return result;
}
