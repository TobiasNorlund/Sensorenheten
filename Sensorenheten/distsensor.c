/*
 * distsensor.c
 *
 * Created: 4/13/2013 7:12:09 PM
 *  Author: johka546
 */ 
#include "distsensor.h"

#define MAXIMUMVALUELONG 266 //TODO UPDATE
#define MINIMUMVALUELONG 43
#define MAXIMUMVALUESHORT 246
#define MINIMUMVALUESHORT 41

volatile uint8_t currentDistSensor=0;
volatile uint8_t currentSample=0;

//41-246 proper values
uint8_t lookUpShortSensor[205] = {
	80,
	78,
	76,
	74,
	72,
	70,
	68,
	66,
	65,
	63,
	62,
	60,
	59,
	58,
	57,
	55,
	54,
	53,
	52,
	51,
	50,
	49,
	48,
	47,
	46,
	46,
	45,
	44,
	43,
	43,
	42,
	41,
	41,
	40,
	39,
	39,
	38,
	37,
	37,
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
	27,
	27,
	27,
	26,
	26,
	26,
	26,
	25,
	25,
	25,
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
	20,
	19,
	19,
	19,
	19,
	19,
	19,
	18,
	18,
	18,
	18,
	18,
	18,
	18,
	17,
	17,
	17,
	17,
	17,
	17,
	17,
	17,
	16,
	16,
	16,
	16,
	16,
	16,
	16,
	16,
	16,
	15,
	15,
	15,
	15,
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
	14,
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
	13,
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
	11,
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
	10,
	10,
	10,
	10,
	10,
	10,
	10
};	
uint8_t lookUpLongSensor[223] = {
	150,
	147,
	144,
	142,
	139,
	137,
	134,
	132,
	129,
	127,
	125,
	123,
	121,
	119,
	117,
	115,
	113,
	111,
	109,
	107,
	105,
	104,
	102,
	101,
	99,
	97,
	96,
	95,
	93,
	92,
	90,
	89,
	88,
	87,
	85,
	84,
	83,
	82,
	81,
	80,
	79,
	78,
	77,
	76,
	75,
	74,
	73,
	72,
	71,
	71,
	70,
	69,
	68,
	67,
	67,
	66,
	65,
	64,
	64,
	63,
	62,
	62,
	61,
	60,
	60,
	59,
	59,
	58,
	57,
	57,
	56,
	56,
	55,
	55,
	54,
	54,
	53,
	53,
	52,
	52,
	51,
	51,
	50,
	50,
	50,
	49,
	49,
	48,
	48,
	48,
	47,
	47,
	46,
	46,
	46,
	45,
	45,
	45,
	44,
	44,
	44,
	43,
	43,
	43,
	42,
	42,
	42,
	41,
	41,
	41,
	40,
	40,
	40,
	40,
	39,
	39,
	39,
	39,
	38,
	38,
	38,
	38,
	37,
	37,
	37,
	37,
	36,
	36,
	36,
	36,
	35,
	35,
	35,
	35,
	35,
	34,
	34,
	34,
	34,
	34,
	33,
	33,
	33,
	33,
	33,
	32,
	32,
	32,
	32,
	32,
	31,
	31,
	31,
	31,
	31,
	31,
	30,
	30,
	30,
	30,
	30,
	30,
	29,
	29,
	29,
	29,
	29,
	29,
	29,
	28,
	28,
	28,
	28,
	28,
	28,
	28,
	27,
	27,
	27,
	27,
	27,
	27,
	27,
	27,
	26,
	26,
	26,
	26,
	26,
	26,
	26,
	26,
	25,
	25,
	25,
	25,
	25,
	25,
	25,
	25,
	25,
	24,
	24,
	24,
	24,
	24,
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
	23,
	23,
	23,
	23,
	23,
	22,
	22,
	22,
};
void Init_distsensor(void)
{
	ADMUX=(1<<REFS0)|(1 << ADLAR);	// AVcc with external capacitor at AREF, ADLAR left adjust res, ADCL innehåller två minsta bitarna, ADCH de andra 8a
	ADCSRA=(1<<ADEN)|(1<<ADIE)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);//AD ENABLED, INTERUPT, 128 div factor ADC Prescaler Selections, p. 257
	ADCSRA |= (1<<ADSC); //börja ny omvandling
	
}

uint8_t filterSampleArray(volatile uint16_t *samples, uint8_t numOfSamples)
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
		return 255;
	}
	else if(sample<MINIMUMVALUELONG)
	{
		//look up MINIMUMVALUELONG in look up table
		return 0;
	}
	else
	{
		return lookUpLongSensor[sample-43];
	}
	return sample;//TODO
}

uint8_t shortDistSensor(uint8_t sample)
{
	
	// ska hantera om sample är utanför look up tables intervall
	if(MAXIMUMVALUESHORT<sample)
	{
		//look up MAXIMUMVALUESHORT in look up table
		return 255;
	}
	else if(sample<MINIMUMVALUESHORT)
	{
		//look up MINIMUMVALUESHORT in look up table
		return 0;
	}
	else
	{
		return lookUpShortSensor[sample-41];
	}
	return sample;//TODO
}

ISR(ADC_vect)
{
	uint8_t nextDistSensor = currentDistSensor;
	volatile uint8_t test;
	currentSample++;//uppdatera precis innan så den alltid pekar på senaste värdet
	if(NUMSAMPLES<currentSample)
	{
		test++;
		currentSample=0;
		nextDistSensor = currentDistSensor+1;//next sensor
		ADMUX=(0b11111000&ADMUX)|(0b00000111&nextDistSensor);//update ad mux
	}
	switch (currentDistSensor)
	{
		case 0:
			distSensor0[currentSample]=ADCL;
			distSensor0[currentSample] << 1;
			distSensor0[currentSample]=distSensor0[currentSample]|ADCH;
			break;
		case 1:
			distSensor1[currentSample]=ADCL;
			distSensor1[currentSample] << 1;
			distSensor1[currentSample]=distSensor1[currentSample]|ADCH;
			break;
		case 2:
			distSensor2[currentSample]=ADCL;
			distSensor2[currentSample] << 1;
			distSensor2[currentSample]=distSensor2[currentSample]|ADCH;
			break;
		case 3:
			distSensor3[currentSample]=ADCL;
			distSensor3[currentSample] << 1;
			distSensor3[currentSample]=distSensor3[currentSample]|ADCH;
			break;
		case 4:
			distSensor4[currentSample]=ADCL;
			distSensor4[currentSample] << 1;
			distSensor4[currentSample]=distSensor4[currentSample]|ADCH;
			break;
		case 5:
			distSensor5[currentSample]=ADCL;
			distSensor5[currentSample] << 1;
			distSensor5[currentSample]=distSensor5[currentSample]|ADCH;
			break;
		case 6:
			distSensor6[currentSample]=ADCL;
			distSensor6[currentSample] << 1;
			distSensor6[currentSample]=distSensor6[currentSample]|ADCH;
			break;
		case 7:
			distSensor7[currentSample]=ADCL;
			distSensor7[currentSample] << 1;
			distSensor7[currentSample]=distSensor7[currentSample]|ADCH;
			break;
	}
	if(7<nextDistSensor)
	{
		nextDistSensor=0;
		ADMUX=(0b11111000&ADMUX)|(0b00000111&nextDistSensor);//update ad mux
	}

	currentDistSensor=nextDistSensor;
	ADCSRA |= (1<<ADSC); //börja ny omvandling
}
