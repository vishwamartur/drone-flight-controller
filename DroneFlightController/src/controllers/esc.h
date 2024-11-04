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
esc_status_t esc_init(const esc_config_t *config) {
    // Initialize SPI communication
    if (spi_init() != SPI_SUCCESS) {
        return ESC_ERROR_INIT_FAILED;
    }

    // Configure ESC channels
    for (uint8_t i = 0; i < 4; i++) {
        // Set initial throttle to minimum
        if (esc_set_throttle(i + 1, config->min_throttle) != ESC_SUCCESS) {
            return ESC_ERROR_INIT_FAILED;
        }
    }

    return ESC_SUCCESS;
}

// Set ESC throttle value (1000-2000)
esc_status_t esc_set_throttle(uint8_t channel, uint16_t throttle) {
    if (channel < 1 || channel > 4 || throttle < 1000 || throttle > 2000) {
        return ESC_ERROR_INVALID_PARAMS;
    }

    // Send throttle command via SPI
    uint8_t data[2] = { (uint8_t)(throttle >> 8), (uint8_t)(throttle & 0xFF) };
    if (spi_write(data, 2) != SPI_SUCCESS) {
        return ESC_ERROR_COMMUNICATION;
    }

    return ESC_SUCCESS;
}

// Arm the ESC
esc_status_t esc_arm(uint8_t channel) {
    if (channel < 1 || channel > 4) {
        return ESC_ERROR_INVALID_PARAMS;
    }

    // Send arming command via SPI
    uint8_t data[2] = { 0xFF, 0xFF }; // Example arming command
    if (spi_write(data, 2) != SPI_SUCCESS) {
        return ESC_ERROR_COMMUNICATION;
    }

    return ESC_SUCCESS;
}

// Disarm the ESC
esc_status_t esc_disarm(uint8_t channel) {
    if (channel < 1 || channel > 4) {
        return ESC_ERROR_INVALID_PARAMS;
    }

    // Send disarming command via SPI
    uint8_t data[2] = { 0x00, 0x00 }; // Example disarming command
    if (spi_write(data, 2) != SPI_SUCCESS) {
        return ESC_ERROR_COMMUNICATION;
    }

    return ESC_SUCCESS;
}

// Get current ESC status
esc_status_t esc_get_status(uint8_t channel, uint16_t *current_throttle) {
    if (channel < 1 || channel > 4 || current_throttle == NULL) {
        return ESC_ERROR_INVALID_PARAMS;
    }

    // Read throttle value via SPI
    uint8_t data[2] = { 0x00, 0x00 }; // Example read command
    if (spi_transfer(data, data, 2) != SPI_SUCCESS) {
        return ESC_ERROR_COMMUNICATION;
    }

    *current_throttle = (data[0] << 8) | data[1];
    return ESC_SUCCESS;
}

#endif /* ESC_H */
