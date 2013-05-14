/*
 * rotsensor.c
 *
 * Created: 4/13/2013 7:12:09 PM
 *  Author: johka546
 */ 
#include "rotsensor.h"
#include "../../TSEA27-include/utils.h"
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
	
	pinChangeCounterLeft = 0;
	pinChangeCounterRight = 0;
	pinStateLastLeft = 0;
	pinStateLastRight = 0;
	for(uint8_t i = 0; i < NUMROTSAMPLES; i++)
	{
		rightSensor[i]=0;
		leftSensor[i]=0;

		rightSensorOverFlow[i]=0;
		leftSensorOverFlow[i]=0;
	}



	//setup timers 1 och 3 16bit timers
	//start clock and set clock devider.
	TCCR1B=(1<<CS10)|(0<<CS11)|(1<<CS12);//clk/1024 (From prescaler)
	TCCR3B=(1<<CS10)|(0<<CS11)|(1<<CS12);// p. 134 i manualen

	//enable overflow interupt
	TIMSK1=(1<<TOIE1);//overflow interupt rightSensor
	TIMSK3=(1<<TOIE3);//overflow interupt leftSensor
	
	TCNT1=0;//init value for counter 1 rightSensor
	TCNT3=0;//init value for counter 3 leftSensor
}

uint8_t calcVelocityRight(void)
{
	volatile uint16_t currentTime = TCNT1;
	if((currentTime>16000)||(rightOverflow>0))//TODO tune
	{
		return 0;// om overflow står vi nog stilla
	}
	else if(rightSensor[CurrentRightSensor]==0)
	{
		return 0;
	}
	else
	{
		uint16_t meanVal = filterMeanTimeAware(rightSensor, NUMROTSAMPLES, 40, CurrentRightSensor);//TODO kolla threshold så det fungerar
		uint8_t t = 2*WHEELDIAM*PI/(meanVal*NUMBEROFSTRIPES*TICKTIME);//TODO kolla så den inte avrunda decimaltalen
		return t;
	}
}

uint8_t calcVelocityLeft(void)
{
	volatile uint16_t currentTime = TCNT1;
	if((currentTime>16000)||(leftOverflow>0))//TODO tune
	{
		return 0;// om overflow står vi nog stilla, >8cm/s == stilla
	}
	else if(leftSensor[CurrentLeftSensor]==0)
	{
		return 0;
	}
	else
	{
		uint16_t meanVal = filterMeanTimeAware(leftSensor, NUMROTSAMPLES, 40, CurrentLeftSensor);//TODO kolla threshold så det fungerar
		uint8_t t = 2*WHEELDIAM*PI/(meanVal*NUMBEROFSTRIPES*TICKTIME);//TODO kolla så den inte avrunda decimaltalen
		return t;
	}
}
#define RIGHTSENSORPINHIGH (PIND&(1<<PIND6))
#define LEFTSENSORPINHIGH (PINC&(1<<PINC0))
void updatePinToggleCounter(void)
{
	//right toggle checking code
	if(pinStateLastRight == 0)
	{
		if(RIGHTSENSORPINHIGH)
		{
			pinChangeCounterRight++;
			pinStateLastRight = 1;
		}
	}	
	if(pinStateLastRight == 1)
	{
		if(!RIGHTSENSORPINHIGH)
		{
			pinChangeCounterRight++;
			pinStateLastRight = 0;
			CurrentRightSensor++;//uppdatera precis innan så den alltid pekar på senaste värdet
			if(NUMROTSAMPLES<=CurrentRightSensor)
			{
				CurrentRightSensor=0;
			}
			rightSensor[CurrentRightSensor]=TCNT1;
			TCNT1=0;
			rightSensorOverFlow[CurrentRightSensor]=rightOverflow;
			rightOverflow=0;
		}
	}
	//left toggle checking code
	if(pinStateLastLeft == 0)
	{
		if(LEFTSENSORPINHIGH)
		{
			pinChangeCounterLeft++;
			pinStateLastLeft = 1;
		}
	}
	if(pinStateLastLeft == 1)
	{
		if(!LEFTSENSORPINHIGH)
		{
			pinChangeCounterLeft++;
			pinStateLastLeft = 0;
			CurrentLeftSensor++;//uppdatera precis innan så den alltid pekar på senaste värdet
			if(NUMROTSAMPLES<=CurrentLeftSensor)
			{
				CurrentLeftSensor=0;
			}
			leftSensor[CurrentLeftSensor]=TCNT3;
			TCNT3=0;
			leftSensorOverFlow[CurrentLeftSensor]=leftOverflow;
			leftOverflow=0;
		}
	}
}

ISR(TIMER1_OVF_vect)
{
	if(rightOverflow != 255)// så vi inte får en overflow på rightOverflow och tiden börjar om, bättre att veta att den gått långt
	{
		rightOverflow++;
	}
}

ISR(TIMER3_OVF_vect)
{
	if(leftOverflow != 255)// så vi inte får en overflow på leftOverflow och tiden börjar om, bättre att veta att den gått långt
	{
		leftOverflow++;
	}
}
