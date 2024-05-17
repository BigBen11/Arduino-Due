// Eingebettete Systeme  / Embedded Systems
// Embedded Distance Assistent (EDA)
// SEMESTER: SS24
// AUTHORS: David Heiss, Manfred Pester, Jens-Peter Akelbein
// FILE: led.c
// CONTENTS: Source for Due Onboard LED control


#include <clock.h>
#include <constants.h>
#include <led.h>
#include <sam3x8e.h>
#include <state.h>


// BUILTIN-LED on Arduino Due Board on PIO Port B Bit 27
#define DUE_LED_BUILTIN PIO_PB27


// return state of LED
bool led_get(void) {
   return (PIOB->PIO_ODSR & PIO_ODSR_P27) != 0;
  
  
  
  
}


// turn on/off LED
void led_set(bool led_on) {
   if (led_on) {
       // Set PIOB27 output to HIGH to turn on the LED
       PIOB->PIO_SODR |= PIO_SODR_P27;
   } else {
       // Set PIOB27 output to LOW to turn off the LED
       PIOB->PIO_CODR |= PIO_CODR_P27;
   }
}


// toogle LED
static void led_toggle(void) {
	if(led_get() == false){
		led_set(true);
	}else{
		// Set PIOB27 output to HIGH to turn on the LED
       led_set(false);
	}
}


// LED is toggling after a number of calls
static void led_blinking(void)
{
   static uint32_t update_tick = 0;
   const uint32_t interval = CLOCK_TICKS_PER_SECOND / 2;


   // if last update is back more than an interval
   if (update_tick + interval < clock_get_loop_tick())
   {
       led_toggle();
       update_tick = clock_get_loop_tick();
   }
}


// initial config for using LED
void led_init(void) {
   //Für Input
   PIOB->PIO_PER |= PIO_PER_P27;
   //Für Output
   PIOB->PIO_OER |= PIO_OER_P27;


	// Schalten der verwendeten PIO Controller im PMC ein
   PMC->PMC_PCER0 |=  1 << ID_PIOD; 

   // Initialisieren der verwendeten Tasten als Eingängell
   PIOB->PIO_IER |= PIO_OER_P0 | PIO_OER_P1 | PIO_OER_P2;
   
}


// loop for LED control - blinking when EDA code is running
void led_loop(void)
{
    if (state.led == LED_NOLOOP)
       return;



    // Lesen des Zustands der Eingänge
    uint32_t input_state = ~PIOD->PIO_PDSR;  

    

    // Überprüfen der Zustände der Eingänge und Aktualisierung des LED-Zustands
    if (input_state & PIO_PDSR_P0) {
        state.led = LED_ON;
    } else if (input_state & PIO_PDSR_P1) {
        state.led = LED_OFF;
    } else if (input_state & PIO_PDSR_P2) {
        state.led = LED_BLINKING;
    }


   switch (state.led)
   {
   case LED_BLINKING:
       led_blinking();
       break;
   case LED_ON:
       led_set(true);
       break;
   case LED_OFF:
       led_set(false);
       break;
   case LED_NOLOOP:
   default:
       break;
   }
}