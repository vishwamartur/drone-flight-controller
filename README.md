# Drone Flight Controller

## Overview

The **Drone Flight Controller** is a sophisticated flight control system for quadcopter drones designed to provide stable flight operations using PID control algorithms, sensor fusion, and fail-safe mechanisms. This project targets ARM Cortex-M4 microcontrollers and utilizes an RTOS for task management.

### Features
- **PID Control**: Stable and precise control using customizable PID parameters.
- **Sensor Fusion**: Combines data from multiple sensors for accurate orientation.
- **Communication Protocols**: I2C and SPI drivers for sensor and peripheral communication.
- **Fail-Safe Mechanisms**: Monitors for critical conditions and initiates safe actions.
- **Real-Time Operating System (RTOS)**: Task management using FreeRTOS for efficient multitasking.

## Table of Contents
- [Project Structure](#project-structure)
- [Getting Started](#getting-started)
- [Hardware Requirements](#hardware-requirements)
- [Installation](#installation)
- [Configuration](#configuration)
- [Usage](#usage)
- [PID Tuning Guide](#pid-tuning-guide)
- [Contributing](#contributing)
- [License](#license)

## Project Structure

```plaintext
drone-flight-controller/
├── README.md                       # Project description and setup instructions
├── docs/                           # Documentation
├── src/                            # Main source code
│   ├── main.c                      # Entry point for the application
│   ├── config/                     # Configuration files
│   ├── controllers/                # Control algorithms and PID
│   ├── sensors/                    # Sensor drivers and fusion logic
│   ├── communication/              # Communication protocols (I2C, SPI)
│   ├── failsafe/                   # Fail-safe mechanisms
│   ├── rtos/                       # RTOS-related setup and tasks
│   └── utils/                      # Utility functions
├── hardware/                       # Hardware details and schematics
└── tests/                          # Test files for each module
```

## Getting Started

### Prerequisites
- **ARM Cortex-M4 Development Board** (e.g., STM32 or similar)
- **FreeRTOS** for task scheduling
- **ARM GCC Toolchain** for building the code
- **GDB or JTAG Debugger** for testing

### Hardware Requirements
- **IMU Sensor** (e.g., MPU6050 or similar) for orientation and acceleration data
- **Voltage Sensor** for battery monitoring
- **Motor Drivers** compatible with PWM control
- Wiring and connectors for I2C/SPI communication

## Installation

1. **Clone the Repository**
   ```bash
   git clone [https://github.com/yourusername/drone-flight-controller.git](https://github.com/vishwamartur/drone-flight-controller.git)
   cd drone-flight-controller
   ```

2. **Set Up the Development Environment**
   - Ensure your ARM toolchain is set up and accessible via the command line.
   - Install FreeRTOS by following the setup guide in `docs/setup.md`.

3. **Configure the Project**
   - Adjust the configuration files in `src/config/` as needed for your hardware setup.
   - Modify `FreeRTOSConfig.h` to match the ARM Cortex-M4 specifications (e.g., interrupt priority and tick frequency).

## Configuration

Configure specific parameters such as PID gains, sensor calibration data, and communication settings in the following files:
- `src/config/hardware_config.h`: Hardware pin configurations and initialization settings.
- `src/controllers/pid_controller.h`: Set PID tuning parameters.
- `src/communication/i2c_driver.h` & `src/communication/spi_driver.h`: Configure I2C and SPI settings as per the sensor requirements.

## Usage

1. **Build and Flash the Code**
   - Use the ARM GCC toolchain to build the project:
     ```bash
     make
     ```
   - Flash the compiled code to your microcontroller using a JTAG or USB debugger.

2. **Run the Flight Controller**
   - Power up the drone and monitor initial logs (if a serial or debug interface is available).
   - The system will initialize sensors, controllers, and fail-safe mechanisms and then enter active flight control mode.

## PID Tuning Guide

Tuning the PID controller is essential for stable flight. Adjust the following parameters in `src/controllers/pid_controller.h`:
- **Proportional (P)**: Determines responsiveness. Increase for faster reaction but watch for instability.
- **Integral (I)**: Compensates for drift. Too high values may cause oscillation.
- **Derivative (D)**: Smooths out response. Avoid high values as it may slow response time.

Refer to `docs/tuning.md` for more detailed tuning instructions and best practices.

## Contributing

Contributions are welcome! Please follow these steps:
1. Fork the repository.
2. Create a new branch: `git checkout -b feature-name`.
3. Commit your changes: `git commit -m 'Add some feature'`.
4. Push to the branch: `git push origin feature-name`.
5. Submit a pull request.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
```
