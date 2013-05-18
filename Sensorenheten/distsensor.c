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
#include "distsensor.h"

#define MAXIMUMVALUELONG 553
#define MINIMUMVALUELONG 82
#define MAXIMUMVALUESHORT 492
#define MINIMUMVALUESHORT 80
#define LONGTABLEOFFSET 6
#define SHORTTABLEOFFSET 1

volatile uint8_t currentDistSensor=0;
volatile uint8_t currentSample=0;

const uint8_t lookUpShortSensor[413] PROGMEM = {//MULTFAC = 2
160, 160, 160, 158, 156, 153, 151, 149, 147, 145, 143, 142, 140, 138, 136, 135, 133, 131, 130, 128, 127, 125, 124, 122, 121, 120, 118, 117, 116, 114, 113, 112, 111, 110, 109, 107, 106, 105, 104, 103, 102, 101, 100, 99, 98, 97, 96, 96, 95, 94, 93, 92, 91, 91, 90, 89, 88, 87, 87, 86, 85, 85, 84, 83, 82, 82, 81, 80, 80, 79, 79, 78, 77, 77, 76, 76, 75, 74, 74, 73, 73, 72, 72, 71, 71, 70, 70, 69, 69, 68, 68, 67, 67, 66, 66, 66, 65, 65, 64, 64, 63, 63, 63, 62, 62, 61, 61, 61, 60, 60, 59, 59, 59, 58, 58, 58, 57, 57, 57, 56, 56, 56, 55, 55, 55, 54, 54, 54, 53, 53, 53, 53, 52, 52, 52, 51, 51, 51, 51, 50, 50, 50, 50, 49, 49, 49, 48, 48, 48, 48, 47, 47, 47, 47, 47, 46, 46, 46, 46, 45, 45, 45, 45, 45, 44, 44, 44, 44, 43, 43, 43, 43, 43, 42, 42, 42, 42, 42, 41, 41, 41, 41, 41, 41, 40, 40, 40, 40, 40, 39, 39, 39, 39, 39, 39, 38, 38, 38, 38, 38, 38, 38, 37, 37, 37, 37, 37, 37, 36, 36, 36, 36, 36, 36, 36, 35, 35, 35, 35, 35, 35, 35, 34, 34, 34, 34, 34, 34, 34, 34, 33, 33, 33, 33, 33, 33, 33, 33, 32, 32, 32, 32, 32, 32, 32, 32, 31, 31, 31, 31, 31, 31, 31, 31, 31, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 19, 19, 19,
};	
const uint8_t lookUpLongSensor[472] PROGMEM = {
150,149,148,146,145,144,142,141,139,138,137,136,134,133,132,131,129,128,127,126,125,124,123,121,120,119,118,117,116,115,114,113,112,111,111,110,109,108,107,106,105,104,104,103,102,101,100,100,99,98,97,97,96,95,95,94,93,92,92,91,91,90,89,89,88,87,87,86,86,85,85,84,83,83,82,82,81,81,80,80,79,79,78,78,77,77,76,76,76,75,75,74,74,73,73,73,72,72,71,71,71,70,70,69,69,69,68,68,68,67,67,67,66,66,66,65,65,65,64,64,64,63,63,63,62,62,62,62,61,61,61,60,60,60,60,59,59,59,59,58,58,58,57,57,57,57,56,56,56,56,56,55,55,55,55,54,54,54,54,54,53,53,53,53,52,52,52,52,52,51,51,51,51,51,50,50,50,50,50,50,49,49,49,49,49,48,48,48,48,48,48,47,47,47,47,47,47,46,46,46,46,46,46,45,45,45,45,45,45,44,44,44,44,44,44,44,43,43,43,43,43,43,43,42,42,42,42,42,42,42,41,41,41,41,41,41,41,41,40,40,40,40,40,40,40,40,39,39,39,39,39,39,39,39,39,38,38,38,38,38,38,38,38,38,37,37,37,37,37,37,37,37,37,37,36,36,36,36,36,36,36,36,36,36,35,35,35,35,35,35,35,35,35,35,34,34,34,34,34,34,34,34,34,34,34,33,33,33,33,33,33,33,33,33,33,33,32,32,32,32,32,32,32,32,32,32,32,32,31,31,31,31,31,31,31,31,31,31,31,30,30,30,30,30,30,30,30,30,30,30,30,29,29,29,29,29,29,29,29,29,29,29,28,28,28,28,28,28,28,28,28,28,28,27,27,27,27,27,27,27,27,27,27,26,26,26,26,26,26,26,26,26,26,25,25,25,25,25,25,25,25,25,25,24,24,24,24,24,24,24,24,24,23,23,23,23,23,23,23,23,23,23,22,22,22,22,22,22,22,22,22,21,21,21,21,21,21,21,21,21,21,20,20,20,20,20,20,20,20,20,20,19,19,19,19,19,19,19,19,19,19,19,19,18,18,18,18,18,18,18,18,18,
};
void Init_distsensor(void)
{
	ADMUX=(1<<REFS0);	// AVcc with external capacitor at AREF
	ADCSRA=(1<<ADEN)|(1<<ADIE)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);//AD ENABLED, INTERUPT, 128 div factor ADC Prescaler Selections, p. 257
	ADCSRA |= (1<<ADSC); //börja ny omvandling
	
}

//turn off optimization 
#pragma GCC push_options
#pragma GCC optimize ("O0")
uint8_t longDistSensor(uint16_t sample, int8_t sensorOffset)
{
	// ska hantera om sample är utanför look up tables intervall
	if(MAXIMUMVALUELONG<(sample+LONGTABLEOFFSET))
	{
		//look up MAXIMUMVALUELONG in look up table
		return 0;
	}
	else if((sample+LONGTABLEOFFSET)<MINIMUMVALUELONG)
	{
		//look up MINIMUMVALUELONG in look up table
		return 255;
	}
	else
	{
		return  pgm_read_byte(&(lookUpLongSensor[(sample+LONGTABLEOFFSET)-MINIMUMVALUELONG]))+sensorOffset;
	}
}

uint8_t shortDistSensor(uint16_t sample, int8_t sensorOffset)
{
	// ska hantera om sample är utanför look up tables intervall
	if(MAXIMUMVALUESHORT<(sample+SHORTTABLEOFFSET))
	{
		//look up MAXIMUMVALUESHORT in look up table
		return 0;
	}
	else if((sample+SHORTTABLEOFFSET)<MINIMUMVALUESHORT)
	{
		//look up MINIMUMVALUESHORT in look up table
		return 255;
	}
	else
	{
		return  pgm_read_byte(&(lookUpShortSensor[(sample+SHORTTABLEOFFSET)-MINIMUMVALUESHORT]))+sensorOffset;
	}
}
#pragma GCC pop_options
//end turn off optimization 


ISR(ADC_vect)
{
	uint8_t nextDistSensor = currentDistSensor;

	currentSample++;//uppdatera precis innan så den alltid pekar på senaste värdet
	if(NUMSAMPLES==currentSample)
	{
		currentSample=0;
		nextDistSensor = currentDistSensor+1;//next sensor
		ADMUX=(0b11111000&ADMUX)|(0b00000111&nextDistSensor);//update ad mux
	}
	distSensor[currentDistSensor][currentSample]=ADC;

	if(7<nextDistSensor)
	{
		nextDistSensor=0;
		ADMUX=(0b11111000&ADMUX)|(0b00000111&nextDistSensor);//update ad mux
	}

	currentDistSensor=nextDistSensor;
	ADCSRA |= (1<<ADSC); //börja ny omvandling
}
