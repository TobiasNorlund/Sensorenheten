/*
 * Sensorenheten.c
 *
 * Created: 4/10/2013 1:47:53 PM
 *  Author: tobno568
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
#include "Sensorenheten.h"


int main(void)
{
	DDRA = 0b11101111;
	DDRB = 0b10000000;
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); 
	ADMUX |= (1 << REFS0); 
	ADMUX |= (1 << MUX2) | (0 << MUX1) | (0 << MUX0); //ADC på PA4 för test 	
	ADMUX |= (1 << ADLAR); 
	ADCSRA |= (1 << ADEN); 
	ADCSRA |= (1 << ADIE);
	sei();
	ADCSRA |= (1 << ADSC); 
    while(1)
    {
        //TODO för MSPI
    }
}

ISR(ADC_vect) //läs av 10 värden och byt sedan sensor
{
	int distance = lookUp[ADCH]; //konvertera till avstånd
	if(avgVal == 0)
	{
		avgVal = distance;
	}
	else if(distance == 0)
	{
	}
	else
	{
		avgVal = (avgVal + distance)/2;
	}		
	if(sampleCounter== desiredSamples)
	{
		avgValues[ADMUX && 0b00000111] = avgVal; // stoppa in medelvärdet på korrekt position i array med medelvärden
		ADCSRA |= (0 << ADEN); //stäng av ADC
		if((ADMUX && 0b0000111) ==  0b00000111) //välj nästa ADC
		{
			ADMUX =  ADMUX && 0b11111000;
		}	
		else 
		{
			ADMUX=ADMUX+1;
		}
		sampleCounter = 0;	
		ADCSRA |= (1 << ADEN); //starta ADC
		ADCSRA |= (1 << ADSC); //börja ny omvandling
	}
	else
	{
		sampleCounter = sampleCounter+1;
		ADCSRA |= (1 << ADSC); //börja ny omvandling
	}
}		


