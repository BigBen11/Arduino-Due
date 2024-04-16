// Eingebettete Systeme  / Embedded Systems
// Embedded Distance Assistent (EDA)
// SEMESTER: SS24
// AUTHORS: 
// FILE: matrix.c
// CONTENTS: main loop 

#include <serial.h>
#include <menu.h>
#include <clock.h>

void init(void)
{
    serial_init();
    menu_init();
    clock_init();
}

void loop(void)
{
    serial_loop();
    menu_loop();
    clock_loop();
}

int main(void)
{
    init();
    while (1) {
        loop();
    }
}
