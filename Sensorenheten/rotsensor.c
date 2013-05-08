﻿/*
 * rotsensor.c
 *
 * Created: 4/13/2013 7:12:09 PM
 *  Author: johka546
 */ 
#include "rotsensor.h"

#define WHEELDIAM 6.3f // cm
#define PI 3.14f 
#define NUMBEROFSTRIPES 12
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
	
	runningAverageIndex = 0;
	runningAverageLeftSum = 0;
	averageLeft = 0;
	leftBufferFilled = 0;
	
	uint8_t pinChangeCounterLeft = 0;
	uint8_t pinChangeCounterRight = 0;
	uint8_t pinStateLastLeft = 0;
	uint8_t pinStateLastRight = 0;
	uint8_t pinCountLastLeft = 0;
	uint8_t pinCountLastRight = 0;
	
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
		return 0;// om overflow står vi nog stilla, >8cm/s == stilla
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

uint8_t filterSamples(volatile uint16_t  *samples, uint8_t numOfSamples, uint8_t threshold)
{
	//räkna ut medelvärde
	//ta bort för långa värden som uppkommer pga avbrott
	//sluta läsa av arrayen om roboten står still
}

uint8_t runningAverageLeft(uint8_t newSample) //tar med värden när den står still, inte bra
{
	if(leftBufferFilled == 1)
	{
		//Ta bort gammalt värde från summan och lägg till nytt
		runningAverageLeftSum -= leftSensor[CurrentLeftSensor];
		leftSensor[CurrentLeftSensor] = newSample;
		runningAverageLeftSum += leftSensor[CurrentLeftSensor];
		averageLeft = runningAverageLeftSum/NUMROTSAMPLES;
		return;		
	}
	else if(CurrentLeftSensor == NUMROTSAMPLES)
	{
		leftBufferFilled = 1;
		runningAverageLeftSum -= leftSensor[CurrentLeftSensor];
		leftSensor[CurrentLeftSensor] = newSample;
		runningAverageLeftSum += leftSensor[CurrentLeftSensor];
		averageLeft = runningAverageLeftSum/NUMROTSAMPLES;
		return;		
	}
	else
	{
		leftSensor[CurrentLeftSensor] = newSample;
		runningAverageLeftSum += leftSensor[CurrentLeftSensor];
		averageLeft = runningAverageLeftSum/(CurrentLeftSensor+1);
		return;
	}
}

void updatePinToggleCounter(void)
{
	if(pinChangeCounterLeft == 255) //undviker overflow
	{
		pinChangeCounterLeft = pinChangeCounterLeft-pinCountLastLeft;
		pinCountLastLeft = 0;
	}
	if(pinChangeCounterRight == 255) //underviker overflow
	{
		pinChangeCounterRight = pinChangeCounterRight-pinCountLastRight;
		pinCountLastRight = 0;
	}
	if(pinStateLastRight == 0)
	{
		if(PINA & 1<<PINA0)
		{
			pinChangeCounterRight++;
			pinStateLastRight = 1;
		}
	}	
	if(pinStateLastRight == 1)
	{
		if(PINA & 0<<PINA0)
		{
			pinChangeCounterRight++;
			pinStateLastRight = 0;
		}
	}
	if(pinStateLastLeft == 0)
	{
		if(PINA & 1<<PINA1)
		{
			pinChangeCounterLeft++;
			pinStateLastLeft = 1;
		}
	}
	if(pinChangeCounterLeft == 1)
	{
		if(PINA & 0<<PINA1)
		{
			pinChangeCounterLeft++;
			pinStateLastLeft = 0;
		}
	}
}

ISR(PCINT3_vect)
{
	CurrentRightSensor++;//uppdatera precis innan så den alltid pekar på senaste värdet
	if(NUMROTSAMPLES<=CurrentRightSensor)
	{
		CurrentRightSensor=0;
	}
	if(pinChangeCounterRight-pinCountLastRight <= 1)
	{
		rightSensor[CurrentRightSensor]=TCNT1;
	}
	else
	{
		rightSensor[CurrentRightSensor]=TCNT1/3;
	}
	pinCountLastRight = pinChangeCounterRight;
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
	if(pinChangeCounterLeft-pinCountLastLeft <= 1)
	{
		leftSensor[CurrentLeftSensor]=TCNT3;		
	}
	else
	{
		leftSensor[CurrentLeftSensor]=TCNT3/3;
	}
	pinCountLastLeft = pinChangeCounterLeft;
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
