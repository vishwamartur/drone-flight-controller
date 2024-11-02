//
//  spi_driver.c
//  DroneFlightController
//
//  Created by Vishwanath Martur on 11/1/24.
//

#include "spi_driver.h"
#include <stdbool.h>
#include "hardware/spi.h"
#include "pico/stdlib.h"

// SPI hardware instance
#define SPI_INSTANCE spi0

// Pin definitions
#define SPI_SCK_PIN  2  // GPIO2
#define SPI_MOSI_PIN 3  // GPIO3  
#define SPI_MISO_PIN 4  // GPIO4

static bool is_initialized = false;

spi_status_t spi_init(void) {
    if (is_initialized) {
        return SPI_SUCCESS;
    }

    // Initialize SPI pins
    gpio_set_function(SPI_SCK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(SPI_MOSI_PIN, GPIO_FUNC_SPI);
    gpio_set_function(SPI_MISO_PIN, GPIO_FUNC_SPI);

    // Initialize SPI peripheral
    spi_init(SPI_INSTANCE);
    spi_set_baudrate(SPI_INSTANCE, SPI_CLOCK_SPEED);
    spi_set_format(SPI_INSTANCE, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);
    is_initialized = true;

    return SPI_SUCCESS;
}

spi_status_t spi_write(uint8_t *data, uint16_t len) {
    if (!is_initialized || !data || len == 0) {
        return SPI_ERROR_INVALID_PARAMS;
    }

    int result = spi_write_blocking(SPI_INSTANCE, data, len);
    if (result == PICO_ERROR_GENERIC) {
        return SPI_ERROR_TRANSFER;
    } else if (result == PICO_ERROR_TIMEOUT) {
        return SPI_ERROR_TIMEOUT;
    }

    return SPI_SUCCESS;
}

spi_status_t spi_read(uint8_t *data, uint16_t len) {
    if (!is_initialized || !data || len == 0) {
        return SPI_ERROR_INVALID_PARAMS;
    }

    int result = spi_read_blocking(SPI_INSTANCE, 0, data, len);
    if (result == PICO_ERROR_GENERIC) {
        return SPI_ERROR_TRANSFER;
    } else if (result == PICO_ERROR_TIMEOUT) {
        return SPI_ERROR_TIMEOUT;
    }

    return SPI_SUCCESS;
}

spi_status_t spi_transfer(uint8_t *tx_data, uint8_t *rx_data, uint16_t len) {
    if (!is_initialized || !tx_data || !rx_data || len == 0) {
        return SPI_ERROR_INVALID_PARAMS;
    }

    int result = spi_write_read_blocking(SPI_INSTANCE, tx_data, rx_data, len);
    if (result == PICO_ERROR_GENERIC) {
        return SPI_ERROR_TRANSFER;
    } else if (result == PICO_ERROR_TIMEOUT) {
        return SPI_ERROR_TIMEOUT;
    }

    return SPI_SUCCESS;
}

void spi_deinit(void) {
    if (is_initialized) {
        spi_deinit(SPI_INSTANCE);
        gpio_set_function(SPI_SCK_PIN, GPIO_FUNC_NULL);
        gpio_set_function(SPI_MOSI_PIN, GPIO_FUNC_NULL);
        gpio_set_function(SPI_MISO_PIN, GPIO_FUNC_NULL);
        is_initialized = false;
    }
}
