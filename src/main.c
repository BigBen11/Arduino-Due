// Eingebettete Systeme  / Embedded Systems
// Embedded Distance Assistent (EDA)
// SEMESTER: SS24
// AUTHORS: 
// FILE: matrix.c
// CONTENTS: main loop 

#include <serial.h>
#include <menu.h>
#include <clock.h>
#include <led.h>
#include <twi.h>
#include <matrix.h>
#include <sonic.h>
#include <drive.h>
#include <motion.h>


void init(void)
{
    serial_init();
    menu_init();
    clock_init();
    led_init();

    //TWI-INIT
    Twi *const twi = TWI1;
    const uint32_t i2c_frequency = 1000; // 1 kHz
    twi_init(twi, i2c_frequency, false);

    matrix_init();
    sonic_init();
    drive_init();
    motion_init();
}

void loop(void)
{
    menu_loop();
    clock_loop();
    led_loop();
    matrix_loop();
    sonic_loop();
    drive_loop();
    motion_loop();
}

int main(void)
{
    init();
    while (1) {
        loop();
    }
}
