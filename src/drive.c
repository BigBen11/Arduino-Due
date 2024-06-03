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

void drive_clock_init(void){

    // Enable the peripheral clock for the timer/counter (TC6 => TC2 with Channel 0)  
    PMC->PMC_PCER0 |= PMC_PCER0_PID27;

    // Configure the timer for waveform mode
    TC2->TC_CHANNEL[0].TC_CMR = TC_CMR_WAVE |       // Enable wave mode
                                TC_CMR_TCCLKS_TIMER_CLOCK1 | // Use MCK/2 as the clock source
                                TC_CMR_WAVSEL_UP_RC | // Count up with automatic trigger on RC compare
                                TC_CMR_ACPA_SET |     // Set TIOA on RA compare match
                                TC_CMR_ACPC_CLEAR;    // Clear TIOA on RC compare match

    // Set the frequency and duty cycle
    uint32_t rc = SystemCoreClock / 80000; //  Hz frequency         //TODO: HIER IST FEHLER
    uint32_t ra = rc * duty_cycle; //  duty cycle

    TC2->TC_CHANNEL[0].TC_RA = ra;
    TC2->TC_CHANNEL[0].TC_RC = rc;

    // Enable the counter and start the timer
    TC2->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;
}

// initialization steps for PWM motor drive
void drive_init(void){

    drive_clock_init();

    // Enable the peripheral clock for PIOC
    // PMC->PMC_PCER1 |= PMC_PCER1_PID34; // PIOC

    // Configure IN1 (PA0) and IN2 (PA1) as outputs
    PIOC->PIO_PER = PIO_PC23 | PIO_PC24 | PIO_PC25; // Enables the PIO to control the corresponding pin
    PIOC->PIO_OER = PIO_PC23 | PIO_PC24 | PIO_PC25; // Enables the output on the I/O line
    PIOC->PIO_CODR = PIO_PC23 | PIO_PC24;           // Clears the data to be driven on the I/O line

    // Configure PIOC25 for PWM output
    PIOC->PIO_ABSR |= PIO_PC25; // Assigns the I/O line to the Peripheral B function 

    //TODO: Was macht B function

}

// loop for regular tasks related to PWM motor drive
void drive_loop(void)
{
    // Update the PWM duty cycle based on the global variable duty_cycle
    uint32_t rc = TC2->TC_CHANNEL[0].TC_RC;             //TODO: HIER IST FEHLER
    uint32_t ra = (uint32_t)((float)rc * duty_cycle);   //TODO: HIER IST FEHLER
    TC2->TC_CHANNEL[0].TC_RA = ra;                      //TODO: HIER IST FEHLER

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
