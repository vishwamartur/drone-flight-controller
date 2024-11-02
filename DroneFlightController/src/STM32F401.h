//
//  STM32F401.h
//  DroneFlightController
//
//  Created by Vishwanath Martur on 11/1/24.
//
#ifndef STM32F401_H
#define STM32F401_H

#include <stdint.h>

// Base addresses of peripherals
#define PERIPH_BASE           (0x40000000U)
#define APB1PERIPH_BASE       (PERIPH_BASE + 0x00000000U)
#define APB2PERIPH_BASE       (PERIPH_BASE + 0x00010000U)
#define AHB1PERIPH_BASE       (PERIPH_BASE + 0x00020000U)
#define AHB2PERIPH_BASE       (PERIPH_BASE + 0x00060000U)

// Peripheral memory map
#define GPIOA_BASE            (AHB1PERIPH_BASE + 0x0000)
#define GPIOB_BASE            (AHB1PERIPH_BASE + 0x0400)
#define GPIOC_BASE            (AHB1PERIPH_BASE + 0x0800)
#define GPIOD_BASE            (AHB1PERIPH_BASE + 0x0C00)
#define GPIOE_BASE            (AHB1PERIPH_BASE + 0x1000)
#define GPIOF_BASE            (AHB1PERIPH_BASE + 0x1400)
#define GPIOG_BASE            (AHB1PERIPH_BASE + 0x1800)
#define GPIOH_BASE            (AHB1PERIPH_BASE + 0x1C00)

#define RCC_BASE              (AHB1PERIPH_BASE + 0x3800)
#define UART1_BASE            (APB2PERIPH_BASE + 0x1000)
#define UART2_BASE            (APB1PERIPH_BASE + 0x4400)
#define I2C1_BASE             (APB1PERIPH_BASE + 0x5400)
#define SPI1_BASE             (APB2PERIPH_BASE + 0x3000)

// GPIO Registers
typedef struct {
    volatile uint32_t MODER;    // Mode Register
    volatile uint32_t OTYPER;   // Output Type Register
    volatile uint32_t OSPEEDR;  // Output Speed Register
    volatile uint32_t PUPDR;    // Pull-Up/Pull-Down Register
    volatile uint32_t IDR;      // Input Data Register
    volatile uint32_t ODR;      // Output Data Register
    volatile uint32_t BSRR;     // Bit Set/Reset Register
    volatile uint32_t LCKR;     // Configuration Lock Register
    volatile uint32_t AFR[2];   // Alternate Function Registers
} GPIO_TypeDef;

// RCC Registers
typedef struct {
    volatile uint32_t CR;       // Clock Control Register
    volatile uint32_t PLLCFGR;  // PLL Configuration Register
    volatile uint32_t CFGR;     // Clock Configuration Register
    volatile uint32_t CIR;      // Clock Interrupt Register
    volatile uint32_t AHB1RSTR; // AHB1 Peripheral Reset Register
    volatile uint32_t AHB2RSTR; // AHB2 Peripheral Reset Register
    volatile uint32_t AHB3RSTR; // AHB3 Peripheral Reset Register
    uint32_t RESERVED0;         // Reserved
    volatile uint32_t APB1RSTR; // APB1 Peripheral Reset Register
    volatile uint32_t APB2RSTR; // APB2 Peripheral Reset Register
    uint32_t RESERVED1[2];      // Reserved
    volatile uint32_t AHB1ENR;  // AHB1 Peripheral Clock Enable Register
    volatile uint32_t AHB2ENR;  // AHB2 Peripheral Clock Enable Register
    volatile uint32_t AHB3ENR;  // AHB3 Peripheral Clock Enable Register
    uint32_t RESERVED2;         // Reserved
    volatile uint32_t APB1ENR;  // APB1 Peripheral Clock Enable Register
    volatile uint32_t APB2ENR;  // APB2 Peripheral Clock Enable Register
    uint32_t RESERVED3[2];      // Reserved
    volatile uint32_t AHB1LPENR; // AHB1 Peripheral Clock Enable in Low Power Mode Register
    volatile uint32_t AHB2LPENR; // AHB2 Peripheral Clock Enable in Low Power Mode Register
    volatile uint32_t AHB3LPENR; // AHB3 Peripheral Clock Enable in Low Power Mode Register
    uint32_t RESERVED4;         // Reserved
    volatile uint32_t APB1LPENR; // APB1 Peripheral Clock Enable in Low Power Mode Register
    volatile uint32_t APB2LPENR; // APB2 Peripheral Clock Enable in Low Power Mode Register
    uint32_t RESERVED5[2];      // Reserved
} RCC_TypeDef;

// UART Registers
typedef struct {
    volatile uint32_t SR;       // Status Register
    volatile uint32_t DR;       // Data Register
    volatile uint32_t BRR;      // Baud Rate Register
    volatile uint32_t CR1;      // Control Register 1
    volatile uint32_t CR2;      // Control Register 2
    volatile uint32_t CR3;      // Control Register 3
    volatile uint32_t GTPR;     // Guard Time and Prescaler Register
} UART_TypeDef;

// Peripheral Instances
#define GPIOA               ((GPIO_TypeDef *) GPIOA_BASE)
#define GPIOB               ((GPIO_TypeDef *) GPIOB_BASE)
#define GPIOC               ((GPIO_TypeDef *) GPIOC_BASE)
#define GPIOD               ((GPIO_TypeDef *) GPIOD_BASE)
#define GPIOE               ((GPIO_TypeDef *) GPIOE_BASE)
#define GPIOF               ((GPIO_TypeDef *) GPIOF_BASE)
#define GPIOG               ((GPIO_TypeDef *) GPIOG_BASE)
#define GPIOH               ((GPIO_TypeDef *) GPIOH_BASE)
#define RCC                 ((RCC_TypeDef *) RCC_BASE)
#define UART1               ((UART_TypeDef *) UART1_BASE)
#define UART2               ((UART_TypeDef *) UART2_BASE)
#define I2C1                ((I2C_TypeDef *) I2C1_BASE)
#define SPI1                ((SPI_TypeDef *) SPI1_BASE)

// Macros for enabling GPIO clock
#define __HAL_RCC_GPIOA_CLK_ENABLE() (RCC->AHB1ENR |= (1 << 0))
#define __HAL_RCC_GPIOB_CLK_ENABLE() (RCC->AHB1ENR |= (1 << 1))
#define __HAL_RCC_GPIOC_CLK_ENABLE() (RCC->AHB1ENR |= (1 << 2))
#define __HAL_RCC_GPIOD_CLK_ENABLE() (RCC->AHB1ENR |= (1 << 3))
#define __HAL_RCC_GPIOE_CLK_ENABLE() (RCC->AHB1ENR |= (1 << 4))
#define __HAL_RCC_GPIOF_CLK_ENABLE() (RCC->AHB1ENR |= (1 << 5))
#define __HAL_RCC_GPIOG_CLK_ENABLE() (RCC->AHB1ENR |= (1 << 6))
#define __HAL_RCC_GPIOH_CLK_ENABLE() (RCC->AHB1ENR |= (1 << 7))

#endif // STM32F401_H
