/*
 * gyro.h
 *
 * Created: 4/16/2013 9:48:28 AM
 *  Author: johka546
 */ 


#ifndef GYRO_H_
#define GYRO_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

#include "../../TSEA27-include/SPI/mspi.h"
#include "../../TSEA27-include/utils.h"

#define NUMGYROSAMPLES 5
volatile uint8_t currentGyroCell;
volatile uint16_t gyroData[NUMGYROSAMPLES];

int8_t gyroCaibration;

void Init_gyro(void);
int8_t calibrateGyro(int8_t maxCalibration);

uint16_t gyroLookUp(uint16_t sample);


#endif /* GYRO_H_ */
