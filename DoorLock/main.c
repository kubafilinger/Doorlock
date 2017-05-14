#define F_CPU 8000000

#include <avr/io.h>
#include <util/delay.h>
#include "include/HD44780.c"
#include "include/keyboard.c"

#define SERVO_MIN 200
#define SERVO_MAX 1000
#define CODE_LENGTH 4
#define LED_RED PINB2
#define LED_GREEN PINB1
#define SERVO_PIN PINB0

volatile int servo_time = 1500;
volatile uint8_t servo_stop = 1; // 0 - ruch, 1 - stop
char *code = "1234";
char writeCode[CODE_LENGTH];
uint8_t witchNumber = 0;

enum codeMode
{
	putCode,
	changeCode
} my_codeMode;

struct servo
{
	int servo_time;
	uint8_t servo_position; // 0 - close, 1 - open
} my_servo;

void openDoor();
void closeDoor();
void moveServo();
int goodCode();
void clearCode();
void setNewCode(char *);
int checkNewCode(char *);
void on_led(uint8_t);
void off_led(uint8_t);

int main(void)
{
	DDRB = (1 << LED_RED) | (1 << LED_GREEN) | (1 << SERVO_PIN); // PD6 is OUTPUT other in
	PORTB = ~((1 << LED_RED) | (1 << LED_GREEN) | (1 << SERVO_PIN)); // do IN podciagniete rezystory a OUT = low

	setPorts(&DDRD, &PORTD, &PIND);
	
	LCD_Initalize();   //inicjalizacja LCD
	LCD_GoTo(0, 0);      //Ustawienie kursora w pozycji (0,0)
	LCD_WriteText("Kutass!!!");

	my_servo.servo_time = 1000;
	my_servo.servo_position = 1;

	my_codeMode = putCode;

	// przy starcie programu zawsze closeDoor();
	closeDoor();
	clearCode();

	while(1)
	{
		char key = pushKey();

		if(key)
		{
			switch(key)
			{
				case 'A':
				{
					my_codeMode = changeCode;
					
					break;
				}

				case 'B':
				{
					break;
				}

				case 'C':
				{
					break;
				}

				case 'D':
				{
					break;
				}

				case '*':
				{
					if(my_codeMode == putCode)
					{
						if(goodCode())
						{
							if(my_servo.servo_position == 0)
							openDoor();
							else
							closeDoor();
						}
					}
					else if(my_codeMode == changeCode)
					{
						if(checkNewCode(&writeCode[0]))
						{
							setNewCode(&writeCode[0]);
						}
					}
					
					clearCode();
					break;
				}

				default: // wpisywanie kodu
				{
					if(witchNumber < 4)
					writeCode[witchNumber++] = key;
					
					break;
				}
			}
		}
	}
}

void off_led(uint8_t led)
{
	PORTB &= ~(1 << led);
}

void on_led(uint8_t led)
{
	PORTB |= (1 << led);
}

int checkNewCode(char *new_code)
{
	int i;
	
	for(i = 0; i < CODE_LENGTH; i++)
	if(new_code[i] == 0)
	return 0;
	
	return 1;
}

void setNewCode(char *new_code)
{
	int i;
	
	for(i = 0; i < CODE_LENGTH; i++)
	code[i] = new_code[i];
}

int goodCode()
{
	int i;
	
	for(i = 0; i < CODE_LENGTH; i++)
	if(code[i] != writeCode[i])
	return 0;
	
	return 1;
}

void clearCode()
{
	int i;
	
	for(i = 0; i < CODE_LENGTH; i++)
	writeCode[i] = 0;
	
	witchNumber = 0;
	my_codeMode = putCode;
}

void openDoor()
{
	my_servo.servo_time = SERVO_MAX; // w us
	moveServo();
	my_servo.servo_position = 1;
	
	off_led(LED_RED);
	on_led(LED_GREEN);
}

void closeDoor()
{
	my_servo.servo_time = SERVO_MIN; // w us
	moveServo();
	my_servo.servo_position = 0;
	
	off_led(LED_GREEN);
	on_led(LED_RED);
}

void moveServo()
{
	int i, time;
	
	if(my_servo.servo_time == SERVO_MIN)
	time = 200;
	else
	time = 1000;
	
	for(i = 0; i < 50; i++)
	{
		PORTB |= (1 << SERVO_PIN);
		
		if(my_servo.servo_time == SERVO_MIN)
		_delay_us(200);
		else
		_delay_us(1000);
		
		PORTB &= ~(1 << SERVO_PIN);
		
		if(my_servo.servo_time == SERVO_MIN)
		_delay_us(20000 - 200);
		else
		_delay_us(20000 - 1000);
	}
}