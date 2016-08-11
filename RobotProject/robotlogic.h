/*
 * robotlogic.h
 *
 * Created: 2016-08-09 오전 2:15:13
 *  Author: Sokwhan
 */ 

#ifndef ROBOTLOGIC_H_
#define ROBOTLOGIC_H_

#include "hardware.h"

typedef enum e_robot_action { NONE, CRUISE, TURN_LEFT, TURN_RIGHT, SHARP_LEFT, SHARP_RIGHT, TURN_AROUND_LEFT, TURN_AROUND_RIGHT, DEFAULT_RIGHT, DEFAULT_LEFT, DEFUSE_MINE } RobotAction;

typedef struct robotLogic{
	RobotAction robotAction;
	float leftSensorVal;
	float centerSensorVal;
	float rightSensorVal;
	float mineSensorVal;
} RobotLogic;

void resetRobotState(void);
RobotLogic runMaze(unsigned int loopDelay);

#endif /* ROBOTLOGIC_H_ */