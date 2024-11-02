//
//  pid_controller.c
//  DroneFlightController
//
//  Created by Vishwanath Martur on 11/1/24.
//

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
