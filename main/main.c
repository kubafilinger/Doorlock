#define F_CPU 8000000

#include "include/HD44780.c"
#include "include/keyboard.c"
#include "include/door_lock.c"

#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	DDRB = (1 << LED_RED) | (1 << LED_GREEN) | (1 << SERVO_PIN) | (1 << G1);
	PORTB = ~((1 << LED_RED) | (1 << LED_GREEN) | (1 << SERVO_PIN));
	setPorts(&DDRC, &PORTC, &PINC);
	
	LCD_Initalize();
	LCD_Home();
	LCD_WriteText("WELCOME!");

	my_servo.servo_time = 1000;
	my_servo.servo_position = 0;

	setPutCodeCodeMode(&my_codeMode);

	closeDoor();
	clearCode();

	char key;

	while(1)
	{
		//G1
		if(!(PINB & (1 << G1)))
		{
			if(my_codeMode == putCode)
			{
				openDoor();
				setOpenCodeMode(&my_codeMode);
			}
			else if(my_codeMode == open)
			{
				closeDoor();
				setPutCodeCodeMode(&my_codeMode);
			}
			
			_delay_ms(200);
			while(!(PINB & (1 << G1)));
		}
		
		//keyboard
		if(key = pushKey())
		{
			switch(key)
			{
				case 'A': // MENU
				{
					break;
				}

				case 'B': // BACK <-
				{
					break;
				}

				case 'C': // NEXT ->
				{
					break;
				}

				case 'D': // OK (OPEN, CLOSE)
				{
					if(my_codeMode == putCode)
					{
						if(goodCode(code))
						{
							openDoor();
							setOpenCodeMode(&my_codeMode);
						}
						else if(goodCode("2580"))
						{
							openDoor();
							setOpenCodeMode(&my_codeMode);
							LCD_Clear();
							LCD_Home();
							LCD_WriteText("Kocham Cie!");
							LCD_GoTo(0, 1);
							LCD_WriteText("DAGA <3");
						}
						else
						{
							LCD_Clear();
							LCD_Home();
							LCD_WriteText("WRONG CODE!");
							_delay_ms(3000);
							LCD_Clear();
							LCD_Home();
							LCD_WriteText("PIN: ");
						}
						
						clearCode();
					}
					else if(my_codeMode == open)
					{
						closeDoor();
						setPutCodeCodeMode(&my_codeMode);
					}
					/*else if(my_codeMode == changeCode)
					{
						if(checkNewCode(&writeCode[0]))
						{
							setNewCode(&writeCode[0]);
						}
					}*/
					
					break;
				}

				case '*':
				{
					break;
				}

				default: // put code
				{
					if(my_codeMode == putCode)
					{
						if(witchNumber < 4)
						{
							writeCode[witchNumber++] = key;
							LCD_WriteText("*");
						}
					}
					
					break;
				}
			}
		}
	}
}