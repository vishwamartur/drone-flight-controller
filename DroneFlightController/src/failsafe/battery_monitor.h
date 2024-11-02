//
//  battery_monitor.h
//  DroneFlightController
//
//  Created by Vishwanath Martur on 11/1/24.
//

#ifndef BATTERY_MONITOR_H
#define BATTERY_MONITOR_H

#include <stdint.h>

// Battery status codes
typedef enum {
    BATTERY_STATUS_OK = 0,
    BATTERY_STATUS_LOW,
    BATTERY_STATUS_CRITICAL,
    BATTERY_STATUS_ERROR
} battery_status_t;

// Battery configuration structure
typedef struct {
    float voltage_low;      // Low battery voltage threshold (V)
    float voltage_critical; // Critical battery voltage threshold (V)
    uint8_t adc_channel;    // ADC channel for battery voltage measurement
} battery_config_t;

// Initialize battery monitor with configuration
battery_status_t battery_monitor_init(const battery_config_t *config);

// Get current battery voltage
float battery_get_voltage(void);

// Get current battery status
battery_status_t battery_get_status(void);

// Enable/disable battery monitoring
void battery_monitor_enable(bool enable);

// Register callback for battery status changes
typedef void (*battery_status_callback_t)(battery_status_t status);
void battery_register_status_callback(battery_status_callback_t callback);

#endif /* BATTERY_MONITOR_H */
