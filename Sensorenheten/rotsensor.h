/*
 * rotsensor.h
 *
 * Created: 4/13/2013 7:12:49 PM
 *  Author: johka546
 */ 


#ifndef ROTSENSOR_H_
#define ROTSENSOR_H_

#include <avr/interrupt.h>

#define NUMROTSAMPLES 25 //TEST

uint8_t rightOverflow;
uint8_t leftOverflow;

volatile uint8_t CurrentRightSensor;
volatile uint8_t CurrentLeftSensor;

volatile uint16_t rightSensor[NUMROTSAMPLES];
volatile uint16_t leftSensor[NUMROTSAMPLES];

volatile uint8_t rightSensorOverFlow[NUMROTSAMPLES];
volatile uint8_t leftSensorOverFlow[NUMROTSAMPLES];


void Init_rotsensor(void);
uint8_t calcVelocityRight(void);
uint8_t calcVelocityLeft(void);

#endif /* ROTSENSOR_H_ */