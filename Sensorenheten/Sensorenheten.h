/*
 * Sensorenheten.h
 *
 * Created: 4/10/2013 2:39:25 PM
 *  Author: tobno568
 */ 

void constructSensorMessage(uint8_t *msg, uint8_t *len);

#define NUMGYROSAMPLES 10
volatile uint8_t currentGyroCell;
volatile uint16_t gyroData[NUMGYROSAMPLES];
