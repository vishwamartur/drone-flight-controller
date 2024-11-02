//
//  i2c_driver.c
//  DroneFlightController
//
//  Created by Vishwanath Martur on 11/1/24.
//

#include "i2c_driver.h"
#include <stdbool.h>
#include "hardware/i2c.h"
#include "pico/stdlib.h"

// I2C hardware instance
#define I2C_INSTANCE i2c0

// Pin definitions
#define I2C_SDA_PIN 4  // GPIO4
#define I2C_SCL_PIN 5  // GPIO5

static bool is_initialized = false;

i2c_status_t i2c_init(void) {
    if (is_initialized) {
        return I2C_SUCCESS;
    }

    // Initialize I2C pins
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA_PIN);
    gpio_pull_up(I2C_SCL_PIN);

    // Initialize I2C peripheral
    i2c_init(I2C_INSTANCE, I2C_CLOCK_SPEED);
    is_initialized = true;

    return I2C_SUCCESS;
}

i2c_status_t i2c_write(uint8_t device_addr, uint8_t reg_addr, uint8_t *data, uint16_t len) {
    if (!is_initialized || !data || len == 0) {
        return I2C_ERROR_INVALID_PARAMS;
    }

    uint8_t buffer[len + 1];
    buffer[0] = reg_addr;
    memcpy(&buffer[1], data, len);

    int result = i2c_write_blocking(I2C_INSTANCE, device_addr, buffer, len + 1, false);
    if (result == PICO_ERROR_GENERIC) {
        return I2C_ERROR_NACK;
    } else if (result == PICO_ERROR_TIMEOUT) {
        return I2C_ERROR_TIMEOUT;
    }

    return I2C_SUCCESS;
}

i2c_status_t i2c_write_byte(uint8_t device_addr, uint8_t reg_addr, uint8_t data) {
    return i2c_write(device_addr, reg_addr, &data, 1);
}

i2c_status_t i2c_read(uint8_t device_addr, uint8_t reg_addr, uint8_t *data, uint16_t len) {
    if (!is_initialized || !data || len == 0) {
        return I2C_ERROR_INVALID_PARAMS;
    }

    // Write register address
    int result = i2c_write_blocking(I2C_INSTANCE, device_addr, &reg_addr, 1, true);
    if (result == PICO_ERROR_GENERIC) {
        return I2C_ERROR_NACK;
    } else if (result == PICO_ERROR_TIMEOUT) {
        return I2C_ERROR_TIMEOUT;
    }

    // Read data
    result = i2c_read_blocking(I2C_INSTANCE, device_addr, data, len, false);
    if (result == PICO_ERROR_GENERIC) {
        return I2C_ERROR_NACK;
    } else if (result == PICO_ERROR_TIMEOUT) {
        return I2C_ERROR_TIMEOUT;
    }

    return I2C_SUCCESS;
}

i2c_status_t i2c_read_byte(uint8_t device_addr, uint8_t reg_addr, uint8_t *data) {
    return i2c_read(device_addr, reg_addr, data, 1);
}

void i2c_deinit(void) {
    if (is_initialized) {
        i2c_deinit(I2C_INSTANCE);
        gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_NULL);
        gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_NULL);
        is_initialized = false;
    }
}

i2c_status_t i2c_reset(void) {
    i2c_deinit();
    return i2c_init();
}
