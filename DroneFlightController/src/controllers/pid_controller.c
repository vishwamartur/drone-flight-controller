#include <stdio.h>
#include <stdlib.h>
#include "pid_controller.h"

// PID controller gains
static float Kp = 0.0f;  // Proportional gain
static float Ki = 0.0f;  // Integral gain 
static float Kd = 0.0f;  // Derivative gain

// Error terms
static float prev_error = 0.0f;
static float integral = 0.0f;

// Initial PID values for pitch, roll, and yaw
static float initial_pitch_p = 0.0f;
static float initial_pitch_i = 0.0f;
static float initial_pitch_d = 0.0f;

static float initial_roll_p = 0.0f;
static float initial_roll_i = 0.0f;
static float initial_roll_d = 0.0f;

static float initial_yaw_p = 0.0f;
static float initial_yaw_i = 0.0f;
static float initial_yaw_d = 0.0f;

void pid_init(float p_gain, float i_gain, float d_gain) {
    Kp = p_gain;
    Ki = i_gain;
    Kd = d_gain;
    prev_error = 0.0f;
    integral = 0.0f;
}

float pid_compute(float setpoint, float measured_value, float dt) {
    // Calculate error
    float error = setpoint - measured_value;
    
    // Proportional term
    float p_term = Kp * error;
    
    // Integral term
    integral += error * dt;
    float i_term = Ki * integral;
    
    // Derivative term
    float derivative = (error - prev_error) / dt;
    float d_term = Kd * derivative;
    
    // Save error for next iteration
    prev_error = error;
    
    // Calculate total output
    float output = p_term + i_term + d_term;
    
    return output;
}

void pid_reset(void) {
    prev_error = 0.0f;
    integral = 0.0f;
}

void set_initial_pid_values(float pitch_p, float pitch_i, float pitch_d,
                            float roll_p, float roll_i, float roll_d,
                            float yaw_p, float yaw_i, float yaw_d) {
    initial_pitch_p = pitch_p;
    initial_pitch_i = pitch_i;
    initial_pitch_d = pitch_d;

    initial_roll_p = roll_p;
    initial_roll_i = roll_i;
    initial_roll_d = roll_d;

    initial_yaw_p = yaw_p;
    initial_yaw_i = yaw_i;
    initial_yaw_d = yaw_d;
}

void adjust_pid_parameters(float new_p_gain, float new_i_gain, float new_d_gain) {
    Kp = new_p_gain;
    Ki = new_i_gain;
    Kd = new_d_gain;
}
