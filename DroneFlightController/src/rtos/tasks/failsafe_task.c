//
//  failsafe_task.c
//  DroneFlightController
//
//  Created by Vishwanath Martur on 11/1/24.
//

#include "failsafe_task.h"
#include "FreeRTOS.h"
#include "task.h"
#include "sensors.h"
#include "flight_controller.h"
#include "battery.h"

// Failsafe thresholds
#define BATTERY_VOLTAGE_THRESHOLD    3.5f   // Volts per cell
#define SIGNAL_LOSS_TIMEOUT         1000    // Milliseconds
#define CRITICAL_ANGLE_THRESHOLD    45.0f   // Degrees

static TaskHandle_t failsafeTaskHandle = NULL;

// Failsafe states
typedef enum {
    FAILSAFE_NORMAL,
    FAILSAFE_WARNING,
    FAILSAFE_CRITICAL
} FailsafeState;

static FailsafeState currentState = FAILSAFE_NORMAL;

// Function prototypes
static void checkBatteryVoltage(void);
static void checkSignalStrength(void);
static void checkDroneOrientation(void);
static void initiateEmergencyLanding(void);

void failsafeTask(void *pvParameters) {
    while(1) {
        // Check various safety parameters
        checkBatteryVoltage();
        checkSignalStrength();
        checkDroneOrientation();
        
        // Sleep for 100ms
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

static void checkBatteryVoltage(void) {
    float voltage = getBatteryVoltage();
    if(voltage < BATTERY_VOLTAGE_THRESHOLD) {
        currentState = FAILSAFE_WARNING;
        initiateEmergencyLanding();
    }
}

static void checkSignalStrength(void) {
    uint32_t lastSignalTime = getLastReceivedSignalTime();
    if((xTaskGetTickCount() - lastSignalTime) > pdMS_TO_TICKS(SIGNAL_LOSS_TIMEOUT)) {
        currentState = FAILSAFE_CRITICAL;
        initiateEmergencyLanding();
    }
}

static void checkDroneOrientation(void) {
    float pitch = getSensorPitch();
    float roll = getSensorRoll();
    
    if(abs(pitch) > CRITICAL_ANGLE_THRESHOLD || abs(roll) > CRITICAL_ANGLE_THRESHOLD) {
        currentState = FAILSAFE_CRITICAL;
        initiateEmergencyLanding();
    }
}

static void initiateEmergencyLanding(void) {
    // Gradually reduce throttle and level the drone
    setEmergencyLandingMode();
}

void initFailsafeTask(void) {
    xTaskCreate(failsafeTask,
                "Failsafe",
                configMINIMAL_STACK_SIZE,
                NULL,
                tskIDLE_PRIORITY + 3,
                &failsafeTaskHandle);
}
