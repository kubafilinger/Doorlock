#include "../header/door_lock.h"

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

int goodCode(char *forwardCode)
{
	int i;
	
	for(i = 0; i < CODE_LENGTH; i++)
		if(forwardCode[i] != writeCode[i])
			return 0;
	
	return 1;
}

void clearCode()
{
	int i;
	
	for(i = 0; i < CODE_LENGTH; i++)
	writeCode[i] = 0;
	
	witchNumber = 0;
}

void openDoor()
{
	my_servo.servo_time = SERVO_MIN; // w us
	moveServo();
	my_servo.servo_position = OPEN;
}

void closeDoor()
{
	my_servo.servo_time = SERVO_MAX; // w us
	moveServo();
	my_servo.servo_position = CLOSE;
	my_codeMode = putCode;
	
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
			_delay_us(1400);
		
		PORTB &= ~(1 << SERVO_PIN);
		
		if(my_servo.servo_time == SERVO_MIN)
			_delay_us(18000 - 1200);
		else
			_delay_us(18000 - 3000);
	}
}

void setOpenCodeMode(enum codeMode *mode)
{
	*mode = open;
	LCD_Clear();
	LCD_Home();
	LCD_WriteText("WELCOME!");
	off_led(LED_RED);
	on_led(LED_GREEN);
}

void setPutCodeCodeMode(enum codeMode *mode)
{
	*mode = putCode;
	LCD_Clear();
	LCD_Home();
	LCD_WriteText("PIN: ");
	on_led(LED_RED);
	off_led(LED_GREEN);
}