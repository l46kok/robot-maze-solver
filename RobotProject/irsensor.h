/*
 * irsensor.h
 *
 * Created: 2016-08-09 오전 1:47:42
 *  Author: Sokwhan
 */ 


#ifndef IRSENSOR_H_
#define IRSENSOR_H_

#include "hardware.h"

typedef enum e_sensor {	SENSOR_LEFT, SENSOR_CENTER, SENSOR_RIGHT, SENSOR_MINE } SensorType;

void initADC(void);
float getSensorValue(SensorType sensor);


#endif /* IRSENSOR_H_ */