// Eingebettete Systeme  / Embedded Systems
// Embedded Distance Assistent (EDA)
// SEMESTER: SS24
// AUTHORS: David Heiss, Manfred Pester, Jens-Peter Akelbein
// FILE: motion.c
// CONTENTS: implementation for all functions for PWM motor drive

#include <sam3x8e.h>
#include <drive.h>
#include <state.h>
#include <math.h>

// initialization steps for PWM motor drive
void drive_init(void){


    // Configure the timer for waveform mode
    TC2->TC_CHANNEL[0].TC_CMR = TC_CMR_WAVE |       // Enable wave mode
                                TC_CMR_TCCLKS_TIMER_CLOCK1 | // Use MCK/2 as the clock source
                                TC_CMR_WAVSEL_UP_RC | // Count up with automatic trigger on RC compare
                                TC_CMR_ACPA_SET |     // Set TIOA on RA compare match
                                TC_CMR_ACPC_CLEAR;    // Clear TIOA on RC compare match

    // Set the frequency and duty cycle
    uint32_t rc = SystemCoreClock / 40000; //  100 Hz frequency
    uint32_t ra = rc * duty_cycle; //  duty cycle

    TC2->TC_CHANNEL[0].TC_RA = ra;
    TC2->TC_CHANNEL[0].TC_RC = rc;

    // Enable the counter and start the timer
    TC2->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;

    // Enable the peripheral clock for the timer/counter (TC0)
    PMC->PMC_PCER0 |= PMC_PCER0_PID27; // Timer Counter 0 (TC0)





    // Enable the peripheral clock for and PIOC
    PMC->PMC_PCER1 |= PMC_PCER1_PID34; // PIOC

    // Configure IN1 (PA0) and IN2 (PA1) as outputs
    PIOC->PIO_PER = PIO_PC23 | PIO_PC24 | PIO_PC25; // Enable PIO control
    PIOC->PIO_OER = PIO_PC23 | PIO_PC24 | PIO_PC25; // Set as output
    PIOC->PIO_CODR = PIO_PC23 | PIO_PC24; 

    // Configure PIOC25 for PWM output
    //PIOC->PIO_PDR = PIO_PC25;   // Disable PIO control for peripheral use
    PIOC->PIO_ABSR |= PIO_PC25; // Select Peripheral B function 


    //Pwm->PWML6 
    //TODO: PWML6 in High PWML7 in Low machen für Forward fahren!!!!!!!!!!!

        // FORWARD
    PIOC->PIO_SODR = PIO_PC23;
    PIOC->PIO_CODR = PIO_PC24;


    // Enable the PWM output on the ENA pin (PIOC25)
    PIOC->PIO_SODR = PIO_PC25; // Disable PIO control for peripheral use

        // BACKWARD
    PIOC->PIO_SODR = PIO_PC24;
    PIOC->PIO_CODR = PIO_PC23;


    PIOC->PIO_CODR = PIO_PC25; // Disable PIO control for peripheral use

    
}

// loop for regular tasks related to PWM motor drive
void drive_loop(void)
{
    // Update the PWM duty cycle based on the global variable duty_cycle
    
    uint32_t rc = TC2->TC_CHANNEL[0].TC_RC;
    uint32_t ra = (uint32_t)((float)rc * duty_cycle);

    TC2->TC_CHANNEL[0].TC_RA = ra;

    if (direction)
    {
        // FORWARD
        PIOC->PIO_SODR = PIO_PC23;
        PIOC->PIO_CODR = PIO_PC24;
    }
    else
    {
        // BACKWARD
        PIOC->PIO_CODR = PIO_PC23;
        PIOC->PIO_SODR = PIO_PC24;
    }
    
}

void drive_duty(const uint32_t freq, const float duty)
{
}
