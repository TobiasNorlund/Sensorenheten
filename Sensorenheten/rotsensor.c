/*
 * rotsensor.c
 *
 * Created: 4/13/2013 7:12:09 PM
 *  Author: johka546
 */ 
#include "rotsensor.h"

#define WHEELDIAM 6.3f // cm
#define PI 3.14f 
#define NUMBEROFSTRIPES 16
/*
	+1 var 1024 klockcykel
	8Mhz = 8000000 Hz
	8000000/1024=7812.5
	1/7812.5 = 0.000128 s per +1
*/
#define TICKTIME 0.0000512f // TODO on clock update 1024/8000000=0.000128

void Init_rotsensor(void)
{
	rightOverflow=0;
	leftOverflow=0;

	CurrentRightSensor=0;
	CurrentLeftSensor=0;
	
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

uint8_t calcVelocityRight(void)
{
	if(rightSensorOverFlow[CurrentRightSensor]>0)//TODO tune
	{
		return 0;// om overflow står vi nog stilla
	}
	else if(rightSensor[CurrentLeftSensor]==0)
	{
		return 255;
	}
	else
	{
		return WHEELDIAM*PI/(rightSensor[CurrentRightSensor]*NUMBEROFSTRIPES*TICKTIME);//TODO kolla så den inte avrunda decimaltalen
	}
}

uint8_t calcVelocityLeft(void)
{
	if(leftSensorOverFlow[CurrentLeftSensor]>0)//TODO tune
	{
		return 0;// om overflow står vi nog stilla
	}
	else if(leftSensor[CurrentLeftSensor]==0)
	{
		return 255;
	}
	else
	{
		return WHEELDIAM*PI/(leftSensor[CurrentLeftSensor]*NUMBEROFSTRIPES*TICKTIME);//TODO kolla så den inte avrunda decimaltalen
	}
}


ISR(PCINT3_vect)
{
	CurrentRightSensor++;//uppdatera precis innan så den alltid pekar på senaste värdet
	if(NUMROTSAMPLES<=CurrentRightSensor)
	{
		CurrentRightSensor=0;
	}
	rightSensor[CurrentRightSensor]=TCNT1;
	rightSensorOverFlow[CurrentRightSensor]=rightOverflow;
	TCNT1=0;//reset
	rightOverflow=0;
}

ISR(PCINT2_vect)
{
	CurrentLeftSensor++;//uppdatera precis innan så den alltid pekar på senaste värdet
	if(NUMROTSAMPLES<=CurrentLeftSensor)
	{
		CurrentLeftSensor=0;
	}
	leftSensor[CurrentLeftSensor]=TCNT3;
	leftSensorOverFlow[CurrentLeftSensor]=leftOverflow;
	TCNT3=0;//reset
	leftOverflow=0;
}

ISR(TIMER1_OVF_vect) {
	rightOverflow++;
}

ISR(TIMER3_OVF_vect) {
	leftOverflow++;
}
