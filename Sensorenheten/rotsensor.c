/*
 * rotsensor.c
 *
 * Created: 4/13/2013 7:12:09 PM
 *  Author: johka546
 */ 
#include "rotsensor.h"

#define NUMSAMPLES 10

#define RIGHTSENSOR PIND&(1<<5)//pd5
#define LEFTSENSOR PIND&(1<<6)//pd6

uint8_t rightSensorPrevState=0;
uint8_t leftSensorPrevState=0;

uint8_t rightOverflow=0;
uint8_t leftOverflow=0;

uint8_t test=0;

volatile uint16_t rightSensor[NUMSAMPLES];
volatile uint16_t leftSensor[NUMSAMPLES];

volatile uint8_t CurrentRightSensor=0;
volatile uint8_t CurrentLeftSensor=0;

void Init_rotsensor(void)
{
	//setup pin change interupts
	PCICR = (1<<PCIE2)|(1<<PCIE3);//enable pin change interupt 2 och 3
	PCMSK3 = (1<<PCINT30);// enable on interupt pin 30
	PCMSK2 = (1<<PCINT16);// enable on interupt pin 16
	
	//setup timers 1 och 3 16bit timers
	//start clock and set clock devider.
	TCCR1B=(1<<CS10)|(0<<CS11)|(1<<CS12);//clk/1024 (From prescaler)
	TCCR3B=(1<<CS10)|(0<<CS11)|(1<<CS12);// p. 134 i manualen

	//enable overflow interupt
	TIMSK1=(1<<TOIE1);//overflow interupt
	TIMSK3=(1<<TOIE3);//overflow interupt
	
	TCNT1=0;//init value for counter 1
	TCNT3=0;//init value for counter 3
}

ISR(PCINT3_vect)
{
	if(NUMSAMPLES<=CurrentRightSensor)
	{
		CurrentRightSensor=0;
	}
	rightSensor[CurrentRightSensor]=TCNT1+65536*rightOverflow;//blir ändå overflow i lagringen, kolla på det. 65536 = 2^16
	CurrentRightSensor++;
	TCNT1=0;//reset
	rightOverflow=0;
	rightSensorPrevState=RIGHTSENSOR;
}
ISR(PCINT2_vect)
{
	if(NUMSAMPLES<=CurrentLeftSensor)
	{
		CurrentLeftSensor=0;
	}
	rightSensor[CurrentLeftSensor]=TCNT3+65536*leftOverflow;//blir ändå overflow i lagringen, kolla på det. 65536 = 2^16
	CurrentLeftSensor++;
	TCNT3=0;//reset
	leftOverflow=0;
	leftSensorPrevState=LEFTSENSOR;
}
ISR(TIMER1_OVF_vect) {
	rightOverflow++;
}

ISR(TIMER3_OVF_vect) {
	leftOverflow++;
}
