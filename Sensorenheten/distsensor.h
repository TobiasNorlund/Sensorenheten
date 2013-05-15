/**
* TSEA27 Elektronikprojekt
*
* IDENTIFIERING
*
* Modul:Sensorenheten
* Filnamn: distsensor.h
* Skriven av: Johan Källström
* Datum: 2013-05-15
* Version: 1.0
*
* BESKRIVNING
*
* Används för att att mäta avståndsensorerna samt slå upp ADC värden i tabeller.
*/	


#ifndef DISTSENSOR_H_
#define DISTSENSOR_H_

#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#define NUMSAMPLES 5

volatile uint16_t distSensor[8][NUMSAMPLES];

/*
* Initerar AD-omvandlingen.
*/
void Init_distsensor(void);

/*
* Slå upp ADC -> cm. 0 om för nära 255 om för långt bort.
*/
uint8_t longDistSensor(uint16_t sample);

/*
* Slå upp ADC -> cm/2. 0 om för nära 255 om för långt bort.
*/
uint8_t shortDistSensor(uint16_t sample);

#endif /* DISTSENSOR_H_ */
