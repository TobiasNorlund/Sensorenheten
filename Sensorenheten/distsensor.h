/*
 * distsensor.h
 *
 * Created: 4/13/2013 7:12:49 PM
 *  Author: johka546
 */ 


#ifndef DISTSENSOR_H_
#define DISTSENSOR_H_

#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#define NUMSAMPLES 5

volatile uint16_t distSensor[8][NUMSAMPLES];

void Init_distsensor(void);
void changeDistSensor(uint8_t ch);
uint16_t filterSampleArray(volatile uint16_t *samples, uint8_t numOfSamples);
uint8_t longDistSensor(uint16_t sample);
uint8_t shortDistSensor(uint16_t sample);
uint16_t absDist(uint16_t a1, uint16_t a2);

#endif /* DISTSENSOR_H_ */
