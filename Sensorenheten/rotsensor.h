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

volatile uint8_t rightOverflow;
volatile uint8_t leftOverflow;

uint8_t CurrentRightSensor;
uint8_t CurrentLeftSensor;

uint16_t rightSensor[NUMROTSAMPLES];
uint16_t leftSensor[NUMROTSAMPLES];

volatile uint8_t rightSensorOverFlow[NUMROTSAMPLES];
volatile uint8_t leftSensorOverFlow[NUMROTSAMPLES];

uint8_t pinChangeCounterLeft;
uint8_t pinChangeCounterRight;
uint8_t pinStateLastLeft;
uint8_t pinStateLastRight;

/*
* Initiera minnet samt timers för de olika hjulen.
*/
void Init_rotsensor(void);

/*
* Beräkna hastigheten ut i från pulstid.
*/
uint8_t calcVelocityRight(void);

/*
* Beräkna hastigheten ut i från pulstid.
*/
uint8_t calcVelocityLeft(void);

/*
* Kollar om någon av sensorerna har bytt state.
*/
void updatePinToggleCounter(void);

#endif /* ROTSENSOR_H_ */
