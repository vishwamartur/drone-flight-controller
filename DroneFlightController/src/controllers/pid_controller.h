//
//  pid_controller.h
//  DroneFlightController
//
//  Created by Vishwanath Martur on 11/1/24.
//

#ifndef PID_CONTROLLER_H
#define PID_CONTROLLER_H

#include <stdint.h>

// Initialize PID controller with gains
void pid_init(float p_gain, float i_gain, float d_gain);

// Compute PID output based on setpoint and measured value
// dt is the time delta since last computation in seconds
float pid_compute(float setpoint, float measured_value, float dt);

// Reset PID controller state (integral term and previous error)
void pid_reset(void);

#endif /* PID_CONTROLLER_H */
