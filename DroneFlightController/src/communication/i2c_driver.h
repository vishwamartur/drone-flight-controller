//
//  i2c_driver.h
//  DroneFlightController
//
//  Created by Vishwanath Martur on 11/1/24.
//

#ifndef i2c_driver_h
#define i2c_driver_h

#include <stdint.h>

// I2C bus configuration
#define I2C_CLOCK_SPEED      400000  // 400 kHz
#define I2C_TIMEOUT_MS       1000    // 1 second timeout

// I2C status codes
typedef enum {
    I2C_SUCCESS = 0,
    I2C_ERROR_BUS_BUSY,
    I2C_ERROR_NACK,
    I2C_ERROR_TIMEOUT,
    I2C_ERROR_INVALID_PARAMS
} i2c_status_t;

// I2C initialization
i2c_status_t i2c_init(void);

// I2C write operations
i2c_status_t i2c_write(uint8_t device_addr, uint8_t reg_addr, uint8_t *data, uint16_t len);
i2c_status_t i2c_write_byte(uint8_t device_addr, uint8_t reg_addr, uint8_t data);

// I2C read operations
i2c_status_t i2c_read(uint8_t device_addr, uint8_t reg_addr, uint8_t *data, uint16_t len);
i2c_status_t i2c_read_byte(uint8_t device_addr, uint8_t reg_addr, uint8_t *data);

// I2C bus control
void i2c_deinit(void);
i2c_status_t i2c_reset(void);

#endif /* i2c_driver_h */
