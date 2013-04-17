/*
 * distsensor.h
 *
 * Created: 4/13/2013 7:12:49 PM
 *  Author: johka546
 */ 


#ifndef DISTSENSOR_H_
#define DISTSENSOR_H_

#include <avr/interrupt.h>

#define NUMSAMPLES 10

volatile uint8_t distSensor0[NUMSAMPLES];
volatile uint8_t distSensor1[NUMSAMPLES];
volatile uint8_t distSensor2[NUMSAMPLES];
volatile uint8_t distSensor3[NUMSAMPLES];
volatile uint8_t distSensor4[NUMSAMPLES];
volatile uint8_t distSensor5[NUMSAMPLES];
volatile uint8_t distSensor6[NUMSAMPLES];
volatile uint8_t distSensor7[NUMSAMPLES];

void Init_distsensor(void);
void changeDistSensor(uint8_t ch);
uint8_t filterSampleArray(volatile uint8_t *samples, uint8_t numOfSamples);
uint8_t longDistSensor(uint8_t sample);
uint8_t shortDistSensor(uint8_t sample);

#endif /* DISTSENSOR_H_ */
