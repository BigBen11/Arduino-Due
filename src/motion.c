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
#include <drive.h>

#define FREQENCY_FOR_DUTY_CYCLE 100
#define KP 0.00035
#define TV 0.001
#define DISTURBANCE 0.001

// last speed in loop in mm/sec
float speed = 0;

// last acceleration (duty) in loop in mm/sec²
float current_duty = 0;

// initialization steps for motion specific
void motion_init(void)
{
}

// loop for regular tasks related to motion functions
void motion_loop(void)
{
  switch (state.motion)
   {
   case MOTION_ON:     // Enable the PWM output on the ENA pin (PIOC25)
        PIOC->PIO_PDR = PIO_PC25;
        break;
   case MOTION_OFF:
        PIOC->PIO_PER = PIO_PC25;     // Disable PIO control for peripheral use        
        break;
   case MOTION_NOLOOP:
       break;
   default:
       break;
   }

    struct sample_t current_sample = sample_series_get(distance_raw, 0);

    //if (current_sample.value / 1000. > gap_target) {
    //     drive_duty(100, 0.25f);
    //} else {
    //     drive_duty(100, 0.01f);
    //}

     // duty ~ acceleration
     float duty = PD_regler(gap_target, current_sample.value / 1000.);
     drive_duty(FREQENCY_FOR_DUTY_CYCLE, duty);

     //Speed upgrade
     if(speed < 0.5){
          speed = speed + duty;
     }else{
          speed = 1;
     }

}

float PD_regler(int32_t soll, int32_t ist){

     float regelabweichung = soll - ist;

     if(regelabweichung < 0){
          regelabweichung = -regelabweichung;
     }

     float p_anteil = KP * regelabweichung;

     float d_anteil = TV * speed;

     float pd_insgesamt = p_anteil + d_anteil;

     float pd_mit_stoerungen = pd_insgesamt - DISTURBANCE;

     return pd_mit_stoerungen;
}
