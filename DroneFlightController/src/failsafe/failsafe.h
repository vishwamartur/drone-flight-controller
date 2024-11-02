//
//  failsafe.h
//  DroneFlightController
//
//  Created by Vishwanath Martur on 11/1/24.
//

#ifndef failsafe_h
#define failsafe_h

#include <stdbool.h>

// Failsafe states
typedef enum {
    FAILSAFE_IDLE = 0,       // No failsafe active
    FAILSAFE_RX_LOSS,        // Radio signal lost
    FAILSAFE_BATTERY_LOW,    // Battery voltage critical
    FAILSAFE_CRASH_DETECTED  // Crash detected
} failsafeState_t;

// Failsafe configuration
typedef struct {
    uint16_t rxLossTimeout;      // Time in ms before triggering RX loss failsafe
    float batteryVoltageThreshold; // Minimum battery voltage before failsafe
    bool enableCrashDetection;   // Enable crash detection failsafe
} failsafeConfig_t;

// Function declarations
void failsafeInit(failsafeConfig_t *config);
void failsafeUpdate(void);
failsafeState_t getFailsafeState(void);
void resetFailsafe(void);
bool isFailsafeActive(void);

#endif /* failsafe_h */
