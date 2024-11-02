//
//  hardware_config.h
//  DroneFlightController
//
//  Created by Vishwanath Martur on 11/1/24.
//

#ifndef HARDWARE_CONFIG_H
#define HARDWARE_CONFIG_H

/* SPI Configuration */
#define SPI_CLOCK_SPEED 1000000  // 1MHz SPI clock speed
#define SPI_INSTANCE spi0
#define SPI_SCK_PIN  2  // GPIO2
#define SPI_MOSI_PIN 3  // GPIO3  
#define SPI_MISO_PIN 4  // GPIO4

/* System Clock Configuration */
#define SYSTEM_CLOCK_FREQ 133000000  // 133MHz system clock

/* GPIO Configuration */
// LED pins
#define STATUS_LED_PIN 25  // Built-in LED on Pico

// I2C pins (for future use)
#define I2C_SDA_PIN 16
#define I2C_SCL_PIN 17

/* Timer Configuration */
#define TIMER_INTERVAL_MS 10  // 10ms timer interval for periodic tasks

/* Debug UART Configuration */
#define UART_TX_PIN 0
#define UART_RX_PIN 1
#define UART_BAUD_RATE 115200

#endif /* HARDWARE_CONFIG_H */
