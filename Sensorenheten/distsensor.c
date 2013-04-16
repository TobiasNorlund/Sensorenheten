/*
 * distsensor.c
 *
 * Created: 4/13/2013 7:12:09 PM
 *  Author: johka546
 */ 
#include "distsensor.h"

#define MAXIMUMVALUELONG 255 //TODO UPDATE
#define MINIMUMVALUELONG 0
#define MAXIMUMVALUESHORT 255
#define MINIMUMVALUESHORT 0

uint8_t currentDistSensor=0;
uint8_t currentSample=0;

uint8_t lookUpShortSensor[255] = {
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	77,
	73,
	70,
	66,
	63,
	60,
	58,
	55,
	53,
	51,
	49,
	47,
	46,
	44,
	42,
	41,
	40,
	39,
	37,
	36,
	35,
	34,
	33,
	32,
	32,
	31,
	30,
	29,
	29,
	28,
	27,
	27,
	26,
	25,
	25,
	24,
	24,
	23,
	23,
	23,
	22,
	22,
	21,
	21,
	20,
	20,
	20,
	19,
	19,
	19,
	18,
	18,
	18,
	18,
	17,
	17,
	17,
	17,
	16,
	16,
	16,
	16,
	15,
	15,
	15,
	15,
	15,
	14,
	14,
	14,
	14,
	14,
	13,
	13,
	13,
	13,
	13,
	13,
	12,
	12,
	12,
	12,
	12,
	12,
	12,
	12,
	11,
	11,
	11,
	11,
	11,
	11,
	11,
	11,
	10,
	10,
	10,
	10,
	10,
	10,
	10,
	10,
	10,
	10,
	9,
	9,
	9,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
};

uint8_t lookUpLongSensor[255] = {
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	152,
	147,
	142,
	136,
	132,
	127,
	123,
	118,
	114,
	111,
	107,
	104,
	100,
	97,
	94,
	92,
	89,
	87,
	84,
	82,
	80,
	78,
	76,
	74,
	72,
	70,
	69,
	67,
	66,
	64,
	63,
	62,
	60,
	59,
	58,
	57,
	56,
	55,
	54,
	53,
	52,
	51,
	50,
	49,
	48,
	47,
	47,
	46,
	45,
	44,
	44,
	43,
	42,
	42,
	41,
	41,
	40,
	40,
	39,
	38,
	38,
	37,
	37,
	36,
	36,
	36,
	35,
	35,
	34,
	34,
	33,
	33,
	33,
	32,
	32,
	32,
	31,
	31,
	31,
	30,
	30,
	30,
	29,
	29,
	29,
	28,
	28,
	28,
	28,
	27,
	27,
	27,
	26,
	26,
	26,
	26,
	26,
	25,
	25,
	25,
	25,
	24,
	24,
	24,
	24,
	24,
	23,
	23,
	23,
	23,
	23,
	22,
	22,
	22,
	22,
	22,
	22,
	21,
	21,
	21,
	21,
	21,
	21,
	20,
	20,
	20,
	20,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
};

void Init_distsensor(void)
{
	ADMUX=(1<<REFS0)|(1 << ADLAR);	// AVcc with external capacitor at AREF, ADLAR left adjust res, ADCL innehåller två minsta bitarna, ADCH de andra 8a
	ADCSRA=(1<<ADEN)|(1<<ADIE)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);//AD ENABLED, INTERUPT, 128 div factor ADC Prescaler Selections, p. 257
	ADCSRA |= (1<<ADSC); //börja ny omvandling
}

void changeDistSensor(uint8_t ch)
{
	ch= ch & 0b00000111;// channel must be b/w 0 to 7
	ADMUX |= ch;// selecting channel
}

uint8_t filterSampleArray(volatile uint8_t *samples, uint8_t numOfSamples)
{
	uint8_t globalMin = 255;
	for(uint8_t i = 0; i < numOfSamples; i++)
	{
		if(globalMin > samples[i])
		{
			globalMin = samples[i];
		}
	}
	return globalMin;	
}

uint8_t longDistSensor(uint8_t sample)
{
	// ska hantera om sample är utanför look up tables intervall
	if(MAXIMUMVALUELONG<sample)
	{
		//look up MAXIMUMVALUELONG in look up table
		return lookUpLongSensor[255];
	}
	else if(sample<MINIMUMVALUELONG)
	{
		//look up MINIMUMVALUELONG in look up table
		return lookUpShortSensor[0];
	}
	else
	{
		return lookUpLongSensor[sample];
	}
	return sample;//TODO
}

uint8_t shortDistSensor(uint8_t sample)
{
	// ska hantera om sample är utanför look up tables intervall
	if(MAXIMUMVALUESHORT<sample)
	{
		//look up MAXIMUMVALUESHORT in look up table
		return lookUpShortSensor[255];
	}
	else if(sample<MINIMUMVALUESHORT)
	{
		//look up MINIMUMVALUESHORT in look up table
		return lookUpShortSensor[0];
	}
	else
	{
		return lookUpShortSensor[sample];
	}
	return sample;//TODO
}

ISR(ADC_vect)
{
	uint8_t nextDistSensor;
	currentSample++;//uppdatera precis innan så den alltid pekar på senaste värdet
	if(NUMSAMPLES<currentSample)
	{
		currentSample=0;
		nextDistSensor = currentDistSensor+1;//next sensor
		changeDistSensor(nextDistSensor);//update ad mux
	}
	switch (currentDistSensor)
	{
		case 0:
			distSensor0[currentSample]=ADC>>2;
			break;
		case 1:
			distSensor1[currentSample]=ADC>>2;
			break;
		case 2:
			distSensor2[currentSample]=ADC>>2;
			break;
		case 3:
			distSensor3[currentSample]=ADC>>2;
			break;
		case 4:
			distSensor4[currentSample]=ADC>>2;
			break;
		case 5:
			distSensor5[currentSample]=ADC>>2;
			break;
		case 6:
			distSensor6[currentSample]=ADC>>2;
			break;
		case 7:
			distSensor7[currentSample]=ADC>>2;
			break;
		default:
			currentDistSensor=0;//reset
			currentSample=0;
			changeDistSensor(currentDistSensor);//update ad mux
			//detta buggar dock bort currentSample=0;
	}
	if(7<nextDistSensor)
	{
		nextDistSensor=0;
		changeDistSensor(nextDistSensor);//update ad mux
	}
	currentDistSensor=nextDistSensor;
	ADCSRA |= (1<<ADSC); //börja ny omvandling
}
