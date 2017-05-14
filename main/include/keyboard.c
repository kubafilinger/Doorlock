#include "../header/keyboard.h"

void setPorts(volatile uint8_t * ddr, volatile uint8_t * port, volatile uint8_t * pin) {
	
	KEYBOARD_DDR = ddr;
	KEYBOARD_PORT = port;
	KEYBOARD_PIN = pin;
	
	*KEYBOARD_DDR = 0b00001111;
	*KEYBOARD_PORT = 0b11111111;
}

char pushKey() {
	int i, j;
	
	for(i = 0; i < 4; i++)
	{
		*KEYBOARD_PORT &= ~(1 << i);

		for(j = 0; j < 4; j++)
		{
			if(!(*KEYBOARD_PIN & (1 << (j + 4)))) // jest stan LOW
			{
				char key = keyboard[3 - j][3 - i];
				
				_delay_ms(20);
				while(!(*KEYBOARD_PIN & (1 << (j + 4))));
				
				return key;
			}
		}

		*KEYBOARD_PORT |= (1 << i);
	}
	
	return 0;
}