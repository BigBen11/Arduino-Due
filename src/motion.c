// Eingebettete Systeme  / Embedded Systems
// Embedded Distance Assistent (EDA)
// SEMESTER: SS24
// AUTHORS: David Heiss, Manfred Pester, Jens-Peter Akelbein
// FILE: motion.c
// CONTENTS: implementation for all functions of motion module

#include <motion.h>
#include <sample.h>
#include <state.h>
#include <sam3x8e.h>


// current movement is zero mm/sec
int32_t motion_current = 0;

// initialization steps for motion specific
void motion_init(void)
{
}

// loop for regular tasks related to motion functions
void motion_loop(void)
{
  switch (state.motion)
   {
   case MOTION_ON:
        PIOC->PIO_SODR = PIO_PC25;     // Enable the PWM output on the ENA pin (PIOC25)
        break;
   case MOTION_OFF:
        PIOC->PIO_CODR = PIO_PC25; // Disable PIO control for peripheral use
        break;
   case MOTION_NOLOOP:
       break;
   default:
       break;
   }

    // super simple control logic
    struct sample_t current_sample = sample_series_get(distance_raw, 0);
    if (current_sample.value / 1000. > gap_target) {
        duty_cycle = 0.5f;
    } else {
        duty_cycle = 0.f;
    }
}
