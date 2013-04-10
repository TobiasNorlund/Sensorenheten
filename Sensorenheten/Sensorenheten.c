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

ISR(ADC_vect)
{
	//case multplexer = ??
	PORTA = ADCH & 0b11101111;
	//PORTB = ADCH & 0b00010000;

	Vout = 20.99*ADCH + 0.19; //Tillfällig linjär ekv.
	//ADEN=0
	//if(ADMUX=7)
	//ADMUX=0
	//else ADMUX=ADMUX+1
	ADCSRA |= (1 << ADSC); 
}