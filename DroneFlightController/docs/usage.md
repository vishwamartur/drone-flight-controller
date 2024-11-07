# Usage Instructions for Drone Flight Controller

## Introduction

This document provides instructions on how to use the Drone Flight Controller after it has been set up and successfully flashed onto your ARM Cortex-M4 development board. The flight controller is designed to operate a quadcopter drone with various features, including PID control, sensor fusion, and fail-safe mechanisms.

## Table of Contents
- [Starting the Flight Controller](#starting-the-flight-controller)
- [Basic Flight Commands](#basic-flight-commands)
- [Using the Control Interface](#using-the-control-interface)
- [Fail-Safe Mechanisms](#fail-safe-mechanisms)
- [Adjusting PID Parameters](#adjusting-pid-parameters)
- [Configuring and Using the Logging Feature](#configuring-and-using-the-logging-feature)

## Starting the Flight Controller

1. **Power On**: Connect the battery to the drone to power on the flight controller. Ensure that all components (motors, sensors, etc.) are connected properly.
  
2. **Initialization**: Once powered, the flight controller will perform an initialization sequence. This includes self-checks of the IMU sensors and motor drivers. Watch for any error codes displayed in the console output.

3. **Ready for Flight**: After successful initialization, the system will be ready for flight. The console will indicate that the drone is armed and ready.

## Basic Flight Commands

The flight controller supports several basic commands that can be sent via a ground control station (GCS) or a remote control interface.

### Command List

- **Takeoff**: Initiates the takeoff sequence.
  ```plaintext
  command: TAKEOFF
  ```
- **Land**: Initiates the landing sequence.
  ```plaintext
  command: LAND
  ```
- **Move**: Moves the drone in specified directions (forward, backward, left, right).
  ```plaintext
  command: MOVE
  parameters: {direction: "FORWARD", distance: 10}  # Distance in meters
  ```
- **Hover**: Maintains the current position in the air.
  ```plaintext
  command: HOVER
  duration: 30  # Duration in seconds
  ```

## Using the Control Interface

1. **Connecting to the GCS**: 
   - Use a compatible GCS application (e.g., QGroundControl, Mission Planner) to connect to the flight controller via a serial connection (USB or telemetry).
   - Select the appropriate COM port and baud rate (typically 115200).

2. **Sending Commands**:
   - Use the GCS interface to send commands as described in the Basic Flight Commands section.
   - Monitor telemetry data for real-time updates on position, altitude, and battery status.

## Fail-Safe Mechanisms

The flight controller includes several fail-safe mechanisms to ensure safe operation during flight:

- **Battery Fail-Safe**: If the battery voltage drops below a predefined threshold, the flight controller will initiate an automatic landing sequence.
- **Signal Loss Fail-Safe**: If the control signal is lost for more than a specified duration, the flight controller will trigger a hover mode or return-to-home (RTH) sequence, depending on the configuration.
- **Sensor Fail-Safe**: If the IMU sensors detect abnormal readings (e.g., extreme tilt), the flight controller will execute corrective actions to stabilize the drone.

## Adjusting PID Parameters

Fine-tuning the PID parameters is crucial for optimal flight performance. To adjust the parameters:

1. **Access the Configuration File**: Open the configuration file in your project directory (e.g., `config/pid_config.h`).
  
2. **Modify PID Values**:
   ```c
   #define PITCH_P 1.0
   #define PITCH_I 0.1
   #define PITCH_D 0.05
   ```
   - Adjust the values of P (proportional), I (integral), and D (derivative) for each axis (pitch, roll, yaw) based on flight performance.

3. **Recompile the Firmware**: After making changes to the PID values, recompile the firmware and flash it back to the drone.

## Configuring and Using the Logging Feature

The flight controller now supports logging critical data to flash memory or external storage for analysis. Follow these steps to configure and use the logging feature:

1. **Initialize the Logger**:
   - In your main code file (e.g., `main.c`), initialize the logger with the desired logging destination (flash memory or external storage).
   ```c
   logger_init("flight.log", LOG_INFO, LOG_TO_FLASH);
   ```

2. **Log Data**:
   - Use the `logger_log` function to log sensor readings, PID outputs, and motor speeds.
   ```c
   logger_log(LOG_INFO, __FILE__, __LINE__, "Sensor Readings - Roll: %f, Pitch: %f, Yaw: %f", roll, pitch, yaw);
   logger_log(LOG_INFO, __FILE__, __LINE__, "PID Outputs - Roll: %f, Pitch: %f, Yaw: %f", roll_output, pitch_output, yaw_output);
   logger_log(LOG_INFO, __FILE__, __LINE__, "Motor Speeds - Roll: %f, Pitch: %f, Yaw: %f", roll_output, pitch_output, yaw_output);
   ```

3. **Close the Logger**:
   - After the flight session, close the logger to ensure all data is properly saved.
   ```c
   logger_close();
   ```

4. **Analyze the Data**:
   - Retrieve the logged data from the flash memory or external storage and analyze it to improve flight performance and diagnose issues.

## Conclusion

This usage guide should help you operate the Drone Flight Controller effectively. For further assistance or troubleshooting, refer to the project documentation, check the GitHub issues page, or consult the community forums.
