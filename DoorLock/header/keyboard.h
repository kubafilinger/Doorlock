/************************************************************************/
/* SPECIFICATION:

ports 0-3 OUTPUTS and changing in loop
ports 4-7 INPUTS and listen HIGH state

INICIALIZATION:
1. setPorts() - required 8 identical pors
2.                                                                      */
/************************************************************************/

#include <avr/io.h>
#include <util/delay.h>

volatile uint8_t * KEYBOARD_DDR = 0;
volatile uint8_t * KEYBOARD_PORT = 0;
volatile uint8_t * KEYBOARD_PIN = 0;
volatile uint8_t checkPush = 0;
char keyboard[4][4] = {
	{'1', '2', '3', 'A'},
	{'4', '5', '6', 'B'},
	{'7', '8', '9', 'C'},
	{'*', '0', '#', 'D'}
};

void setPorts(volatile uint8_t *, volatile uint8_t *, volatile uint8_t *);
char pushKey();