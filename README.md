# Drone Flight Controller

A sophisticated flight control system for quadcopter drones, implementing PID control algorithms, sensor fusion, and fail-safe mechanisms for stable flight operations.

## Table of Contents
- [Overview](#overview)
- [Features](#features)
- [Hardware Requirements](#hardware-requirements)
- [Software Requirements](#software-requirements)
- [Setup Instructions](#setup-instructions)
- [Usage](#usage)
- [License](#license)

## Overview
This project provides a robust flight controller for quadcopters using an ARM Cortex-M4 microcontroller. The firmware controls motors via Electronic Speed Controllers (ESCs), integrates sensor data for flight stability, and supports real-time PID tuning and safety features.

## Features
- **PID Control**: Stable flight using customizable PID gains for pitch, roll, and yaw.
- **Sensor Fusion**: Integrates data from gyroscope, accelerometer, and magnetometer for precise orientation.
- **Fail-safe Mechanisms**: Includes emergency stop and battery voltage monitoring.
- **Real-time Communication**: Interfaces for telemetry data and control inputs.
- **Modular Design**: Easy to expand and modify for additional features.

## Hardware Requirements
- **Microcontroller**: STM32F401 (ARM Cortex-M4)
- **Sensors**: 
  - IMU (e.g., MPU6050)
  - Barometer (optional, for altitude hold)
- **ESCs**: Electronic Speed Controllers for motor control
- **Motors**: Quad setup
- **Battery**: Li-Po battery (e.g., 3S or 4S for sufficient power)
- **Additional Components**: Connectors, wiring, and power distribution board.

## Software Requirements
- **Toolchain**: ARM GCC Compiler
- **IDE**: STM32CubeIDE or Xcode (instructions below for Xcode setup)
- **Dependencies**: STM32 HAL Library, FreeRTOS (optional)
- **Communication Protocols**: I2C, SPI for sensor interfaces

## Setup Instructions

### 1. Hardware Setup
- Follow the [hardware pinout diagram](hardware/pinout_diagram.png) for wiring components.
- Refer to the [components list](hardware/components_list.md) for necessary parts.
- Ensure proper power distribution and voltage levels for each component.

### 2. Cloning the Repository
```bash
git clone https://github.com/yourusername/drone-flight-controller.git
cd drone-flight-controller
