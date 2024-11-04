#ifndef REMOTE_CONTROL_H
#define REMOTE_CONTROL_H

#include <stdint.h>

// Initialize remote control communication
void remote_control_init(void);

// Update remote control inputs
void remote_control_update(void);

// Channel mapping functions
uint16_t get_throttle_channel(void);
uint16_t get_pitch_channel(void);
uint16_t get_roll_channel(void);
uint16_t get_yaw_channel(void);

// Failsafe mechanism functions
void check_failsafe(void);

#endif /* REMOTE_CONTROL_H */
