#ifndef imu_sensor_h
#define imu_sensor_h

#include <stdint.h>

class IMUSensor {
public:
    // Constructor/Destructor
    IMUSensor();
    ~IMUSensor();
    
    // Initialize the IMU sensor
    bool initialize();
    
    // Read sensor data
    bool readAccelerometer(float& x, float& y, float& z);
    bool readGyroscope(float& x, float& y, float& z);
    bool readMagnetometer(float& x, float& y, float& z);
    
    // Get processed data
    bool getOrientation(float& roll, float& pitch, float& yaw);
    bool getAngularVelocity(float& x, float& y, float& z);
    bool getLinearAcceleration(float& x, float& y, float& z);
    
    // Calibration
    bool calibrate();
    void resetCalibration();
    
    // Status checks
    bool isInitialized() const;
    bool isSelfTestPassed() const;
    
    // Sensor register configuration
    bool configureSensorRegisters();

private:
    // Internal state
    bool initialized;
    bool calibrated;
    
    // Calibration data
    struct CalibrationData {
        float accelBias[3];
        float gyroBias[3];
        float magBias[3];
    } calibrationData;
    
    // Raw sensor readings
    struct SensorData {
        float accel[3];
        float gyro[3];
        float mag[3];
    } sensorData;
    
    // Private helper functions
    bool performSelfTest();
    void updateSensorData();
    void applyCalibration();
};

#endif /* imu_sensor_h */
