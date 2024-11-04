#include "imu_sensor.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "i2c_driver.h"

// Constructor
IMUSensor::IMUSensor() : initialized(false), calibrated(false) {
    // Initialize calibration data to zero
    for(int i = 0; i < 3; i++) {
        calibrationData.accelBias[i] = 0.0f;
        calibrationData.gyroBias[i] = 0.0f;
        calibrationData.magBias[i] = 0.0f;
        sensorData.accel[i] = 0.0f;
        sensorData.gyro[i] = 0.0f;
        sensorData.mag[i] = 0.0f;
    }
}

// Destructor
IMUSensor::~IMUSensor() {
    // Clean up resources if needed
}

bool IMUSensor::initialize() {
    if(!performSelfTest()) {
        return false;
    }

    // Configure sensor registers
    if(!configureSensorRegisters()) {
        return false;
    }
    
    initialized = true;
    return true;
}

bool IMUSensor::readAccelerometer(float& x, float& y, float& z) {
    if(!initialized) return false;
    
    updateSensorData();
    x = sensorData.accel[0];
    y = sensorData.accel[1];
    z = sensorData.accel[2];
    return true;
}

bool IMUSensor::readGyroscope(float& x, float& y, float& z) {
    if(!initialized) return false;
    
    updateSensorData();
    x = sensorData.gyro[0];
    y = sensorData.gyro[1];
    z = sensorData.gyro[2];
    return true;
}

bool IMUSensor::readMagnetometer(float& x, float& y, float& z) {
    if(!initialized) return false;
    
    updateSensorData();
    x = sensorData.mag[0];
    y = sensorData.mag[1];
    z = sensorData.mag[2];
    return true;
}

bool IMUSensor::getOrientation(float& roll, float& pitch, float& yaw) {
    if(!initialized) return false;
    
    // Calculate orientation from accelerometer and magnetometer data
    // Using basic AHRS algorithm
    float ax = sensorData.accel[0];
    float ay = sensorData.accel[1];
    float az = sensorData.accel[2];
    
    roll = atan2f(ay, az);
    pitch = atan2f(-ax, sqrtf(ay * ay + az * az));
    yaw = atan2f(sensorData.mag[1], sensorData.mag[0]);
    
    return true;
}

bool IMUSensor::getAngularVelocity(float& x, float& y, float& z) {
    return readGyroscope(x, y, z);
}

bool IMUSensor::getLinearAcceleration(float& x, float& y, float& z) {
    return readAccelerometer(x, y, z);
}

bool IMUSensor::calibrate() {
    if(!initialized) return false;
    
    // Collect multiple samples and average them
    const int numSamples = 100;
    float accelSum[3] = {0};
    float gyroSum[3] = {0};
    float magSum[3] = {0};
    
    for(int i = 0; i < numSamples; i++) {
        updateSensorData();
        for(int j = 0; j < 3; j++) {
            accelSum[j] += sensorData.accel[j];
            gyroSum[j] += sensorData.gyro[j];
            magSum[j] += sensorData.mag[j];
        }
        vTaskDelay(pdMS_TO_TICKS(10)); // Wait between samples
    }
    
    // Calculate average bias
    for(int i = 0; i < 3; i++) {
        calibrationData.accelBias[i] = accelSum[i] / numSamples;
        calibrationData.gyroBias[i] = gyroSum[i] / numSamples;
        calibrationData.magBias[i] = magSum[i] / numSamples;
    }
    
    calibrated = true;
    return true;
}

void IMUSensor::resetCalibration() {
    for(int i = 0; i < 3; i++) {
        calibrationData.accelBias[i] = 0.0f;
        calibrationData.gyroBias[i] = 0.0f;
        calibrationData.magBias[i] = 0.0f;
    }
    calibrated = false;
}

bool IMUSensor::isInitialized() const {
    return initialized;
}

bool IMUSensor::isSelfTestPassed() const {
    return initialized;
}

bool IMUSensor::performSelfTest() {
    // Implement self-test routine
    // This would typically involve reading test registers
    // and verifying sensor responses
    return true;
}

bool IMUSensor::configureSensorRegisters() {
    // Example configuration for MPU6050
    uint8_t data = 0;

    // Set sample rate to 1kHz
    data = 0x07;
    if(i2c_write_byte(MPU6050_ADDRESS, MPU6050_SMPLRT_DIV, data) != I2C_SUCCESS) {
        return false;
    }

    // Set accelerometer configuration
    data = 0x00; // +/- 2g
    if(i2c_write_byte(MPU6050_ADDRESS, MPU6050_ACCEL_CONFIG, data) != I2C_SUCCESS) {
        return false;
    }

    // Set gyroscope configuration
    data = 0x00; // +/- 250 degrees/sec
    if(i2c_write_byte(MPU6050_ADDRESS, MPU6050_GYRO_CONFIG, data) != I2C_SUCCESS) {
        return false;
    }

    // Set power management
    data = 0x01; // PLL with X axis gyroscope reference
    if(i2c_write_byte(MPU6050_ADDRESS, MPU6050_PWR_MGMT_1, data) != I2C_SUCCESS) {
        return false;
    }

    return true;
}

void IMUSensor::updateSensorData() {
    // Here you would implement the actual I2C/SPI communication
    // with the IMU hardware to read the latest sensor values
    // For now, we'll just simulate some values
    for(int i = 0; i < 3; i++) {
        sensorData.accel[i] = 0.0f; // Replace with actual sensor reading
        sensorData.gyro[i] = 0.0f;  // Replace with actual sensor reading
        sensorData.mag[i] = 0.0f;   // Replace with actual sensor reading
    }
    
    if(calibrated) {
        applyCalibration();
    }
}

void IMUSensor::applyCalibration() {
    for(int i = 0; i < 3; i++) {
        sensorData.accel[i] -= calibrationData.accelBias[i];
        sensorData.gyro[i] -= calibrationData.gyroBias[i];
        sensorData.mag[i] -= calibrationData.magBias[i];
    }
}
