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

// Set initial PID values for pitch, roll, and yaw
void set_initial_pid_values(float pitch_p, float pitch_i, float pitch_d,
                            float roll_p, float roll_i, float roll_d,
                            float yaw_p, float yaw_i, float yaw_d);

// Adjust and tune PID parameters as needed
void adjust_pid_parameters(float new_p_gain, float new_i_gain, float new_d_gain);

#endif /* PID_CONTROLLER_H */
