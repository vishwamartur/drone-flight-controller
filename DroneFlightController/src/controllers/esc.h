//
//  esc.h
//  DroneFlightController
//
//  Created by Vishwanath Martur on 11/1/24.
//

#ifndef ESC_H
#define ESC_H

#include <stdint.h>
#include "communication/spi_driver.h"

// ESC status codes
typedef enum {
    ESC_SUCCESS = 0,
    ESC_ERROR_INIT_FAILED,
    ESC_ERROR_INVALID_PARAMS,
    ESC_ERROR_COMMUNICATION
} esc_status_t;

// ESC configuration structure
typedef struct {
    uint8_t channel;           // ESC channel number (1-4)
    uint16_t min_throttle;     // Minimum throttle value (typically 1000)
    uint16_t max_throttle;     // Maximum throttle value (typically 2000)
    uint16_t arm_throttle;     // Arming throttle value
} esc_config_t;

// Initialize ESC driver
esc_status_t esc_init(const esc_config_t *config);

// Set ESC throttle value (1000-2000)
esc_status_t esc_set_throttle(uint8_t channel, uint16_t throttle);

// Arm the ESC
esc_status_t esc_arm(uint8_t channel);

// Disarm the ESC
esc_status_t esc_disarm(uint8_t channel);

// Get current ESC status
esc_status_t esc_get_status(uint8_t channel, uint16_t *current_throttle);

#endif /* ESC_H */
