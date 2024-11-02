//
//  spi_driver.h
//  DroneFlightController
//
//  Created by Vishwanath Martur on 11/1/24.
//

#ifndef spi_driver_h
#define spi_driver_h

#include <stdint.h>

// SPI bus configuration
#define SPI_CLOCK_SPEED      1000000  // 1 MHz
#define SPI_TIMEOUT_MS       1000     // 1 second timeout

// SPI status codes
typedef enum {
    SPI_SUCCESS = 0,
    SPI_ERROR_TRANSFER,
    SPI_ERROR_TIMEOUT,
    SPI_ERROR_INVALID_PARAMS
} spi_status_t;

// SPI initialization
spi_status_t spi_init(void);

// SPI transfer operations
spi_status_t spi_write(uint8_t *data, uint16_t len);
spi_status_t spi_read(uint8_t *data, uint16_t len);
spi_status_t spi_transfer(uint8_t *tx_data, uint8_t *rx_data, uint16_t len);

// SPI bus control
void spi_deinit(void);
spi_status_t spi_reset(void);

#endif /* spi_driver_h */
