// Eingebettete Systeme  / Embedded Systems
// Embedded Distance Assistent (EDA)
// SEMESTER: SS24
// AUTHORS: David Heiss, Manfred Pester, Jens-Peter Akelbein
// FILE: motion.c
// CONTENTS: Implementation of all functions of motion module

#include <motion.h>
#include <sample.h>
#include <state.h>
#include <sam3x8e.h>
#include <drive.h>

#define DUTY_CYCLE_FREQUENCY 100

// Proportional Term Coefficient
#define PROPORTIONAL_GAIN 0.0017 

// Derivative Term Coefficient
#define DERIVATIVE_GAIN 0.007

#define DISTURBANCE_OFFSET 0.0

// Last speed in loop (in mm/sec)
static float current_speed = 0.0;

#define STATIC_BACKWARDS_DUTYCYCLE 0.4

int frequency = 100;

// Initialization steps for motion specific
void motion_init(void) {
    // Any necessary initialization code for the motion module
}

// Loop for regular tasks related to motion functions
void motion_loop(void) {
    // Handle state changes
    switch (state.motion) {
        case MOTION_ON: // Enable the PWM output on the ENA pin (PIOC25)
            PIOC->PIO_PDR = PIO_PC25;
            break;
        case MOTION_OFF: // Disable PIO control for peripheral use
            PIOC->PIO_PER = PIO_PC25;
            break;
        case MOTION_NOLOOP:
        default:
            break;
    }

    // Get the current and last samples
    struct sample_t current_sample = sample_series_get(distance_raw, 0);
    struct sample_t last_sample = sample_series_get(distance_raw, 1);

    // Calculate the change in distance and time
    float delta_distance = (current_sample.value - last_sample.value) / 1000.0; // convert mm to meters
    float delta_time = (current_sample.usec - last_sample.usec) / 1000.0; // convert usec to msec

    // Calculate the current speed
    if (delta_time > 0) {
        current_speed = delta_distance / delta_time;
    }

    // Calculate duty using the PD controller
    float duty_cycle = PD_controller(gap_target, current_sample.value / 1000.0); // convert mm to meters

    if(gap_target > current_sample.value / 1000.0){
        direction = false;
    }else{
        direction = true; 
    }

    if(direction == false){
        duty_cycle = STATIC_BACKWARDS_DUTYCYCLE;
        frequency = 140;
    }

    drive_duty(frequency, duty_cycle);

}

// PD controller function
float PD_controller(float target_distance, float current_distance) {
    float error = target_distance - current_distance;

    // Ensure error is positive
    if (error < 0) {
        error = -error;
    }

    // Proportional part
    float proportional_term = PROPORTIONAL_GAIN * error;

    // Derivative part
    float derivative_term = DERIVATIVE_GAIN * current_speed;

    // PD combined result
    float pd_total = proportional_term - derivative_term;

    // Add disturbances
    float pd_with_disturbance = pd_total + DISTURBANCE_OFFSET;

    return pd_with_disturbance;
}
