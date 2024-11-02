//
//  communication_task.c
//  DroneFlightController
//
//  Created by Vishwanath Martur on 11/1/24.
//

#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include "failsafe/failsafe.h"
#include "communication/spi_driver.h"
#include "controllers/esc.h"

// Task configuration
#define COMM_TASK_STACK_SIZE 256
#define COMM_TASK_PRIORITY   3
#define COMM_CHECK_PERIOD_MS 100

// Communication status
typedef enum {
    COMM_STATUS_OK = 0,
    COMM_STATUS_ERROR,
    COMM_STATUS_TIMEOUT
} comm_status_t;

// Task handle
static TaskHandle_t commTaskHandle = NULL;

// Function prototypes
static void communication_task(void *pvParameters);
static comm_status_t check_communication_status(void);
static void handle_communication_failure(void);

// Initialize communication task
void communication_task_init(void) {
    // Create the communication task
    xTaskCreate(
        communication_task,
        "CommTask",
        COMM_TASK_STACK_SIZE,
        NULL,
        COMM_TASK_PRIORITY,
        &commTaskHandle
    );
}

// Main communication task function
static void communication_task(void *pvParameters) {
    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();

    while (1) {
        // Check communication status
        comm_status_t status = check_communication_status();
        
        if (status != COMM_STATUS_OK) {
            handle_communication_failure();
            // Update failsafe system
            if (isFailsafeActive()) {
                executeFailsafe();
            }
        } else {
            // Reset failsafe timer on successful communication
            failsafeUpdateSignal();
        }

        // Wait for the next check period
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(COMM_CHECK_PERIOD_MS));
    }
}

// Check communication links status
static comm_status_t check_communication_status(void) {
    // Check SPI communication
    if (spi_check_connection() != SPI_SUCCESS) {
        return COMM_STATUS_ERROR;
    }
    
    // Check ESC communication
    uint16_t throttle;
    if (esc_get_status(1, &throttle) != ESC_SUCCESS) {
        return COMM_STATUS_ERROR;
    }
    
    return COMM_STATUS_OK;
}

// Handle communication failures
static void handle_communication_failure(void) {
    // Attempt to reinitialize communication
    spi_init();
    
    // Notify system of communication failure
    // This could trigger failsafe modes or emergency procedures
}
