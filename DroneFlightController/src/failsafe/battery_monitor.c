//
//  battery_monitor.c
//  DroneFlightController
//
//  Created by Vishwanath Martur on 11/1/24.
//

#include "battery_monitor.h"
#include <stdbool.h>
#include "hardware/adc.h"
#include "pico/stdlib.h"

// Private variables
static battery_config_t battery_config;
static bool monitoring_enabled = false;
static battery_status_callback_t status_callback = NULL;
static battery_status_t current_status = BATTERY_STATUS_OK;

// ADC reference voltage and conversion factor
#define ADC_VREF 3.3f
#define ADC_RANGE (1 << 12)  // 12-bit ADC
#define ADC_CONVERSION_FACTOR (ADC_VREF / ADC_RANGE)

// Voltage divider ratio (if used)
#define VOLTAGE_DIVIDER_RATIO 3.0f  // Adjust based on hardware

battery_status_t battery_monitor_init(const battery_config_t *config) {
    if (!config) {
        return BATTERY_STATUS_ERROR;
    }

    // Store configuration
    battery_config = *config;

    // Initialize ADC
    adc_init();
    adc_gpio_init(battery_config.adc_channel + 26); // ADC pins start at GPIO26
    adc_select_input(battery_config.adc_channel);

    monitoring_enabled = true;
    current_status = BATTERY_STATUS_OK;

    return BATTERY_STATUS_OK;
}

float battery_get_voltage(void) {
    if (!monitoring_enabled) {
        return 0.0f;
    }

    // Read ADC value
    adc_select_input(battery_config.adc_channel);
    uint16_t raw = adc_read();
    
    // Convert to voltage
    float voltage = (float)raw * ADC_CONVERSION_FACTOR * VOLTAGE_DIVIDER_RATIO;
    
    return voltage;
}

battery_status_t battery_get_status(void) {
    if (!monitoring_enabled) {
        return BATTERY_STATUS_ERROR;
    }

    float voltage = battery_get_voltage();
    battery_status_t new_status;

    if (voltage <= battery_config.voltage_critical) {
        new_status = BATTERY_STATUS_CRITICAL;
    } else if (voltage <= battery_config.voltage_low) {
        new_status = BATTERY_STATUS_LOW;
    } else {
        new_status = BATTERY_STATUS_OK;
    }

    // Notify if status changed
    if (new_status != current_status && status_callback) {
        status_callback(new_status);
    }

    current_status = new_status;
    return current_status;
}

void battery_monitor_enable(bool enable) {
    monitoring_enabled = enable;
}

void battery_register_status_callback(battery_status_callback_t callback) {
    status_callback = callback;
}
