#include <avr/io.h>
#include <util/delay.h>

#define SERVO_MIN 400
#define SERVO_MAX 800
#define CODE_LENGTH 4
#define G1 PINB3
#define LED_RED PINB1
#define LED_GREEN PINB2
#define SERVO_PIN PINB0
#define CLOSE 1
#define OPEN 0

volatile int servo_time = 1500;
volatile uint8_t servo_stop = 1; // 0 - ruch, 1 - stop
char *code = "1234";
char writeCode[CODE_LENGTH];
uint8_t witchNumber = 0;

enum codeMode
{
	putCode,
	open,
	menu
} my_codeMode;

struct servo
{
	int servo_time;
	uint8_t servo_position;
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

void setOpenCodeMode(enum codeMode *);
void setPutCodeCodeMode(enum codeMode *);