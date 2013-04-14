/*
 * distsensor.h
 *
 * Created: 4/13/2013 7:12:49 PM
 *  Author: johka546
 */ 


#ifndef DISTSENSOR_H_
#define DISTSENSOR_H_

#include <avr/interrupt.h>

void Init_distsensor(void);
void changeDistSensor(uint8_t ch);
uint8_t filterSampleArray(uint8_t *samples, uint8_t numOfSamples);
uint8_t longDistSensor(uint8_t sample);
uint8_t shortDistSensor(uint8_t sample);

#endif /* DISTSENSOR_H_ */
