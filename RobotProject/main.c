/*
 * RobotProject.c
 *
 * Created: 8/8/2016 3:17:46 PM
 * Author : shuh
 */ 

#define ROBOT_STATE_IDLE 0
#define ROBOT_STATE_RUNNING 1
#define LCD_DISPLAY_DELAY 500
#define LOOP_DELAY 5

//#define ROBOT_SPEED_FAST   175
//#define ROBOT_SPEED_NORMAL 70

//#define ROBOT_SPEED_FAST   210
#define ROBOT_SPEED_FAST   250
#define ROBOT_SPEED_NORMAL 60

//#define ROBOT_SPEED_FAST 0
//#define ROBOT_SPEED_NORMAL 0

#include "hardware.h"
#include "lcd.h"
#include "irsensor.h"
#include "robotlogic.h"

void init(void);
void initIO(void);
void initPWM(void);
void displayRobotState(RobotLogic robotLogic);
void driveRobot(RobotLogic robotLogic);
void defuseMine(RobotLogic robotLogic);

int main(void) {
	init();
	unsigned int robotState = ROBOT_STATE_IDLE;
	_delay_ms(1000);
	lcdClearScreen();
	lcdDisplayText("Robot ready");
	unsigned long stateChange = 0;
	unsigned long displayFreq = 0;
    while (1) {
		if (stateChange > 0) {
			stateChange -= 1;
		}

		//Change state of robot upon pressing switch
		if (PINC == 0x00 && stateChange <= 0) {
			robotState = robotState == ROBOT_STATE_IDLE ? ROBOT_STATE_RUNNING : ROBOT_STATE_IDLE;
			lcdClearScreen();
			if (robotState == ROBOT_STATE_IDLE) {
				lcdDisplayText("Robot Idle");
				PORTB = 0x00;
				OCR1A = 0;
				OCR1B = 0;
				stateChange = 800000;
			}
			else {
				stateChange = 500;
				resetRobotState();
			}
		}

		if (robotState == ROBOT_STATE_IDLE) {
			continue;
		}
		
		RobotLogic robotLogic = runMaze(LOOP_DELAY);
		if (robotLogic.robotAction == DEFUSE_MINE) {
			defuseMine(robotLogic);
		}
		else {
			driveRobot(robotLogic);	
			if (displayFreq == LCD_DISPLAY_DELAY) {
				displayRobotState(robotLogic);
				displayFreq = 0;
			}

			displayFreq++;
		}
	
		//_delay_ms(LOOP_DELAY);
    }
}

void defuseMine(RobotLogic robotLogic) {
	float defuseTime = 3.0;
	char result[6];
	lcdBreakLine();
	OCR1A = 0;
	OCR1B = 0;
	while (defuseTime > 0) {
		lcdClearScreen();
		lcdDisplayText("DEFUSING MINE");
		lcdBreakLine();
		PORTB ^= 0x02;	
		dtostrf(robotLogic.mineSensorVal,4,3,result);
		lcdDisplayText(result);
		lcdDisplayChar(0x20);
		dtostrf(defuseTime, 4, 3, result);
		lcdDisplayText(result);
		defuseTime -= 0.072;	
		_delay_ms(50);	
	}
	lcdClearScreen();
}

void driveRobot(RobotLogic robotLogic) {
	switch (robotLogic.robotAction) {
		case NONE:
			OCR1A = 0;
			OCR1B = 0;
			break;
		case CRUISE:
			OCR1A = ROBOT_SPEED_FAST;
			OCR1B = ROBOT_SPEED_FAST;
			break;
		case TURN_LEFT:
		case DEFAULT_LEFT:
			OCR1A = ROBOT_SPEED_FAST;
			OCR1B = ROBOT_SPEED_NORMAL;
			break;
		case TURN_RIGHT:
		case DEFAULT_RIGHT:
			OCR1A = ROBOT_SPEED_NORMAL;
			OCR1B = ROBOT_SPEED_FAST;
			break;
		case SHARP_LEFT:
		case TURN_AROUND_LEFT:
			OCR1A = ROBOT_SPEED_FAST;
			OCR1B = 0;
			break;
		case SHARP_RIGHT:
		case TURN_AROUND_RIGHT:
			OCR1A = 0;
			OCR1B = ROBOT_SPEED_FAST;
			break;
	}
}

void displayRobotState(RobotLogic robotLogic) {
	char result[6];
	lcdClearScreen();
	switch (robotLogic.robotAction) {
		case NONE:
			lcdDisplayText("NONE");
			break;
		case CRUISE:
			lcdDisplayText("CRUISE");
			break;
		case TURN_AROUND_LEFT:
			lcdDisplayText("TURN_AROUND_L");
			break;
		case TURN_AROUND_RIGHT:
			lcdDisplayText("TURN_AROUND_R");
			break;
		case TURN_LEFT:
			lcdDisplayText("TURN_LEFT");
			break;
		case TURN_RIGHT:
			lcdDisplayText("TURN_RIGHT");
			break;
		case SHARP_LEFT:
			lcdDisplayText("SHARP_LEFT");
			break;
		case SHARP_RIGHT:
			lcdDisplayText("SHARP_RIGHT");
			break;
		case DEFAULT_RIGHT:
			lcdDisplayText("DEFAULT_RIGHT");
			break;
		case DEFAULT_LEFT:
			lcdDisplayText("DEFAULT_LEFT");
			break;
		default:
			lcdDisplayText("ERR");
			break;
	}

	lcdBreakLine();

	dtostrf(robotLogic.leftSensorVal, 3, 2, result);
	lcdDisplayText(result);
	lcdDisplayChar(0x20); //Send a space

	dtostrf(robotLogic.centerSensorVal, 3, 2, result);
	lcdDisplayText(result);
	lcdDisplayChar(0x20); //Send a space

	dtostrf(robotLogic.rightSensorVal, 3, 2, result);
	lcdDisplayText(result);
}

void init(void) {
	initIO();
	initUART0();
	initADC();
	initPWM();
	sei(); // Enable global interrupts
}

void initPWM(void) {
	TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1<<WGM10);
	TCCR1B = (1 << CS10) | (1 << CS11);
	OCR1B = 0; 
	OCR1A = 0; 
}

void initIO(void) {
	DDRB = 0x03;
	PORTB = 0x00;
	DDRD = 0xFF; //Motors (PWM Output)
	DDRC = 0xFE; //Tact Switch
	PORTC = 0x01;
}