/*
 * Sensorenheten.h
 *
 * Created: 4/10/2013 2:39:25 PM
 *  Author: tobno568
 */ 

void constructSensorMessage(uint8_t *msg, uint8_t *len);

volatile uint8_t multFactor = 48;

uint8_t pinChangeCounterLeft;
uint8_t pinChangeCounterRight;
uint8_t pinChangeLastLeft;
uint8_t pinChangeLastRight;