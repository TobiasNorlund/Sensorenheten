/*
 * distsensor.c
 *
 * Created: 4/13/2013 7:12:09 PM
 *  Author: johka546
 */ 
#include "distsensor.h"

#define NUMSAMPLES 10

#define MAXIMUMVALUELONG 255 //TODO UPDATE
#define MINIMUMVALUELONG 0
#define MAXIMUMVALUESHORT 255
#define MINIMUMVALUESHORT 0

uint8_t currentDistSensor=0;
uint8_t currentSample=0;

volatile uint8_t distSensor0[NUMSAMPLES];
volatile uint8_t distSensor1[NUMSAMPLES];
volatile uint8_t distSensor2[NUMSAMPLES];
volatile uint8_t distSensor3[NUMSAMPLES];
volatile uint8_t distSensor4[NUMSAMPLES];
volatile uint8_t distSensor5[NUMSAMPLES];
volatile uint8_t distSensor6[NUMSAMPLES];
volatile uint8_t distSensor7[NUMSAMPLES];


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

uint8_t filterSampleArray(uint8_t *samples, uint8_t numOfSamples)
{
	for(uint8_t i = 0; i <=numOfSamples; i++)
	{
		// do some
		//samples[i]
	}
	return samples[0];//TODO //return result
}

uint8_t longDistSensor(uint8_t sample)
{
	// ska hantera om sample är utanför look up tables intervall
	if(MAXIMUMVALUELONG<sample)
	{
		//look up MAXIMUMVALUELONG in look up table
		//return MAXIMUMVALUELONG;
	}
	else if(sample<MINIMUMVALUELONG)
	{
		//look up MINIMUMVALUELONG in look up table
		//return MINIMUMVALUELONG;
	}
	else
	{
		//Look up as usual
	}
	return sample;//TODO
}

uint8_t shortDistSensor(uint8_t sample)
{
	// ska hantera om sample är utanför look up tables intervall
	if(MAXIMUMVALUESHORT<sample)
	{
		//look up MAXIMUMVALUESHORT in look up table
		//return MAXIMUMVALUESHORT;
	}
	else if(sample<MINIMUMVALUESHORT)
	{
		//look up MINIMUMVALUESHORT in look up table
		//return MINIMUMVALUESHORT;
	}
	else
	{
		//Look up as usual
	}
	return sample;//TODO
}

ISR(ADC_vect)
{
	switch (currentDistSensor)
	{
		case 0:
			distSensor0[currentSample]=ADCH;
			break;
		case 1:
			distSensor1[currentSample]=ADCH;
			break;
		case 2:
			distSensor2[currentSample]=ADCH;
			break;
		case 3:
			distSensor3[currentSample]=ADCH;
			break;
		case 4:
			distSensor4[currentSample]=ADCH;
			break;
		case 5:
			distSensor5[currentSample]=ADCH;
			break;
		case 6:
			distSensor6[currentSample]=ADCH;
			break;
		case 7:
			distSensor7[currentSample]=ADCH;
			break;
		default:
			currentDistSensor=0;//reset
			currentSample=0;
			changeDistSensor(currentDistSensor);//update ad mux
			//detta buggar dock bort currentSample=0;
	}
	currentSample++;
	if(NUMSAMPLES<currentSample)
	{
		currentSample=0;
		currentDistSensor++;//next sensor
		changeDistSensor(currentDistSensor);//update ad mux
	}
	if(7<currentDistSensor)
	{
		currentDistSensor=0;
		changeDistSensor(currentDistSensor);//update ad mux
	}
	ADCSRA |= (1<<ADSC); //börja ny omvandling
}
