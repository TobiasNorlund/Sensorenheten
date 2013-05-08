/*
 * rotsensor.h
 *
 * Created: 4/13/2013 7:12:49 PM
 *  Author: johka546
 */ 


#ifndef ROTSENSOR_H_
#define ROTSENSOR_H_

#include <avr/interrupt.h>

#define NUMROTSAMPLES 16 //TEST

uint8_t rightOverflow;
uint8_t leftOverflow;

volatile uint8_t CurrentRightSensor;
volatile uint8_t CurrentLeftSensor;

volatile uint16_t rightSensor[NUMROTSAMPLES];
volatile uint16_t leftSensor[NUMROTSAMPLES];

volatile uint8_t rightSensorOverFlow[NUMROTSAMPLES];
volatile uint8_t leftSensorOverFlow[NUMROTSAMPLES];

uint8_t runningAverageIndex;
uint8_t runningAverageLeftSum;
uint8_t averageLeft;
uint8_t leftBufferFilled;

volatile uint8_t pinChangeCounterLeft;
volatile uint8_t pinChangeCounterRight;
volatile uint8_t pinStateLastLeft;
volatile uint8_t pinStateLastRight;
volatile uint8_t pinCountLastLeft;
volatile uint8_t pinCountLastRight;


void Init_rotsensor(void);
uint8_t calcVelocityRight(void);
uint8_t calcVelocityLeft(void);
uint8_t runningAverageLeft(uint8_t newSample);
uint16_t filterRotSamples(volatile uint16_t  *samples, uint8_t numOfSamples, uint8_t threshold, uint8_t start)
void updatePinToggleCounter(void);

#endif /* ROTSENSOR_H_ */
