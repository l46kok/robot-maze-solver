/*
 * robotlogic.c
 *
 * Created: 2016-08-09 오전 2:15:05
 *  Author: Sokwhan
 */ 
#include "robotlogic.h"
#include "irsensor.h"

volatile long remainingTime = 0;
volatile RobotAction currentAction = NONE;

#define WALL_PROXIMITY_THRESHOLD 1.35
#define WALL_PROXIMITY_CLOSE_THRESHOLD 1.80
#define WALL_PROXIMITY_TURN_AROUND_THRESHOLD 1.4
#define TURN_AROUND_DURATION 7.0
#define TURN_GENERAL_DURATION 0.1
#define TURN_SHARP_DURATION 0.025
#define MINE_SENSOR_THRESHOLD 1.15

void resetRobotState(void) {
	remainingTime = 0;
	currentAction = NONE;
}

RobotLogic runMaze(unsigned int loopDelay) {
	RobotLogic logicState;
	logicState.leftSensorVal = getSensorValue(SENSOR_LEFT);
	logicState.centerSensorVal = getSensorValue(SENSOR_CENTER);
	logicState.rightSensorVal = getSensorValue(SENSOR_RIGHT);
	logicState.mineSensorVal = getSensorValue(SENSOR_MINE);
	if (remainingTime > 0) {
		remainingTime -= 1;
		logicState.robotAction = currentAction;
		return logicState;
	}
	if (logicState.mineSensorVal < MINE_SENSOR_THRESHOLD) {
		remainingTime = 0;
		logicState.robotAction = DEFUSE_MINE;
	}
	else if (logicState.leftSensorVal > WALL_PROXIMITY_TURN_AROUND_THRESHOLD && 
		logicState.centerSensorVal > WALL_PROXIMITY_CLOSE_THRESHOLD-0.2 && 
		logicState.rightSensorVal > WALL_PROXIMITY_TURN_AROUND_THRESHOLD) {
		if (logicState.leftSensorVal > logicState.rightSensorVal) {
			logicState.robotAction = TURN_AROUND_RIGHT;	
		}
		else {
			logicState.robotAction = TURN_AROUND_LEFT;
		}
		remainingTime = TURN_AROUND_DURATION / (loopDelay / 1000.);
	}
	else if (logicState.leftSensorVal > WALL_PROXIMITY_CLOSE_THRESHOLD &&
			 logicState.centerSensorVal > WALL_PROXIMITY_THRESHOLD) {
		logicState.robotAction = SHARP_RIGHT;
		remainingTime = TURN_SHARP_DURATION / (loopDelay / 1000.);
	}
	else if (logicState.rightSensorVal > WALL_PROXIMITY_CLOSE_THRESHOLD && 
			 logicState.centerSensorVal > WALL_PROXIMITY_THRESHOLD) {
		logicState.robotAction = SHARP_LEFT;
		remainingTime = TURN_SHARP_DURATION / (loopDelay / 1000.);
	}
	else if (logicState.leftSensorVal > 1.2 && 
			 logicState.rightSensorVal <= 0.9) {
		logicState.robotAction = TURN_RIGHT;
		remainingTime = TURN_GENERAL_DURATION / (loopDelay / 1000.);
	}
	else if (logicState.leftSensorVal <= 0.9 && 
			 logicState.rightSensorVal > 1.2) {
		logicState.robotAction = TURN_LEFT;
		remainingTime = TURN_GENERAL_DURATION / (loopDelay / 1000.);
	}
	else if (logicState.leftSensorVal <= WALL_PROXIMITY_THRESHOLD &&
	 		 logicState.centerSensorVal > WALL_PROXIMITY_CLOSE_THRESHOLD &&
	  		 logicState.rightSensorVal <= WALL_PROXIMITY_THRESHOLD) {
		//TO DO: Come up with better algorithm to determine direction (Map pathing?)
		if (logicState.leftSensorVal > logicState.rightSensorVal) {
			logicState.robotAction = DEFAULT_RIGHT; //Turn right as default for now
		}
		else {
			logicState.robotAction = DEFAULT_LEFT; //Turn right as default for now
		}
		
		remainingTime = TURN_GENERAL_DURATION / (loopDelay / 1000.);
	}
	else if (logicState.centerSensorVal > 1.2) {
		if (logicState.leftSensorVal > logicState.rightSensorVal) {
			logicState.robotAction = TURN_RIGHT;
		}
		else {
			logicState.robotAction = TURN_LEFT;
		}
		remainingTime = TURN_GENERAL_DURATION / (loopDelay / 1000.);
	}
	else {
		logicState.robotAction = CRUISE;
	}
	currentAction = logicState.robotAction;
	return logicState;
}