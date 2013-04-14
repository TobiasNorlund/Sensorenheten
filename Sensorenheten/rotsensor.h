/*
 * rotsensor.h
 *
 * Created: 4/13/2013 7:12:49 PM
 *  Author: johka546
 */ 


#ifndef ROTSENSOR_H_
#define ROTSENSOR_H_

#include <avr/interrupt.h>

#define NUMSAMPLES 10

uint8_t rightOverflow;
uint8_t leftOverflow;

volatile uint8_t CurrentRightSensor;
volatile uint8_t CurrentLeftSensor;

volatile uint16_t rightSensor[NUMSAMPLES];
volatile uint16_t leftSensor[NUMSAMPLES];

volatile uint8_t rightSensorOverFlow[NUMSAMPLES];
volatile uint8_t leftSensorOverFlow[NUMSAMPLES];


void Init_rotsensor(void);
uint8_t calcVelocityRight(void);
uint8_t calcVelocityLeft(void);

#endif /* ROTSENSOR_H_ */