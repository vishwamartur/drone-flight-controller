#include "remote_control.h"
#include "esc.h"
#include "failsafe.h"
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/irq.h"

// Remote control channel mapping
#define THROTTLE_CHANNEL 0
#define PITCH_CHANNEL    1
#define ROLL_CHANNEL     2
#define YAW_CHANNEL      3

// PWM configuration
#define PWM_WRAP 20000
#define PWM_DIVIDER 64

// Failsafe configuration
#define SIGNAL_LOSS_TIMEOUT_MS 1000

// Channel values
static uint16_t channel_values[4] = {0};

// Last signal time
static uint32_t last_signal_time = 0;

// Function prototypes
static void pwm_irq_handler(void);
static void update_channel_values(uint slice_num, uint16_t value);
static void check_failsafe(void);

void remote_control_init(void) {
    // Initialize PWM
    for (int i = 0; i < 4; i++) {
        gpio_set_function(i, GPIO_FUNC_PWM);
        uint slice_num = pwm_gpio_to_slice_num(i);
        pwm_set_wrap(slice_num, PWM_WRAP);
        pwm_set_clkdiv(slice_num, PWM_DIVIDER);
        pwm_set_enabled(slice_num, true);
    }

    // Set up PWM interrupt
    irq_set_exclusive_handler(PWM_IRQ_WRAP, pwm_irq_handler);
    irq_set_enabled(PWM_IRQ_WRAP, true);
    pwm_clear_irq(pwm_gpio_to_slice_num(0));
    pwm_set_irq_enabled(pwm_gpio_to_slice_num(0), true);
}

void remote_control_update(void) {
    // Update failsafe
    check_failsafe();

    // Map channels to control functions
    uint16_t throttle = channel_values[THROTTLE_CHANNEL];
    uint16_t pitch = channel_values[PITCH_CHANNEL];
    uint16_t roll = channel_values[ROLL_CHANNEL];
    uint16_t yaw = channel_values[YAW_CHANNEL];

    // Update ESCs
    esc_set_throttle(1, throttle);
    esc_set_throttle(2, pitch);
    esc_set_throttle(3, roll);
    esc_set_throttle(4, yaw);
}

static void pwm_irq_handler(void) {
    // Read PWM values
    for (int i = 0; i < 4; i++) {
        uint slice_num = pwm_gpio_to_slice_num(i);
        uint16_t value = pwm_get_counter(slice_num);
        update_channel_values(slice_num, value);
    }

    // Clear interrupt
    pwm_clear_irq(pwm_gpio_to_slice_num(0));
}

static void update_channel_values(uint slice_num, uint16_t value) {
    if (slice_num < 4) {
        channel_values[slice_num] = value;
        last_signal_time = to_ms_since_boot(get_absolute_time());
    }
}

static void check_failsafe(void) {
    uint32_t current_time = to_ms_since_boot(get_absolute_time());
    if (current_time - last_signal_time > SIGNAL_LOSS_TIMEOUT_MS) {
        // Disarm motors
        for (int i = 1; i <= 4; i++) {
            esc_disarm(i);
        }
    }
}
