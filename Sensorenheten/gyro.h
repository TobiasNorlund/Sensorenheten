/**
* TSEA27 Elektronikprojekt
*
* IDENTIFIERING
*
* Modul:Sensorenheten
* Filnamn: gyro.h
* Skriven av: Johan Källström
* Datum: 2013-05-15
* Version: 1.0
*
* BESKRIVNING
*
* Används för att sampla gyrot samt slå upp ADC värden i tabeller.
*/	


#ifndef GYRO_H_
#define GYRO_H_

#include "globals.h"

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

/*
* Initiera gyrot.
*/
void Init_gyro(void);
/*
* Räknar ut ett nytt nollställe. Differansen från tidigare kalibrering får ej vara större än maxCalibration.
*/
int8_t calibrateGyro(int8_t maxCalibration);

/*
* Slå upp ADC till grader/sek
*/
int16_t gyroLookUp(uint16_t sample);

/*
* Gör en gyroavläsning och spara ner den i buffern.
*/
void updateGyroData(void);


#endif /* GYRO_H_ */
