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
	
	runningAverageIndex = 0;
	runningAverageLeftSum = 0;
	averageLeft = 0;
	leftBufferFilled = 0;
	
	pinChangeCounterLeft = 0;
	pinChangeCounterRight = 0;
	pinStateLastLeft = 0;
	pinStateLastRight = 0;
	pinCountLastLeft = 0;
	pinCountLastRight = 0;
/*	
	//setup pin change interupts
	PCICR = (1<<PCIE2)|(1<<PCIE3);//enable pin change interupt 2 och 3
	PCMSK3 = (1<<PCINT30);// enable on interupt pin 30
	PCMSK2 = (1<<PCINT16);// enable on interupt pin 16
*/
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
	if((currentTime>32300)||(rightOverflow>0))//TODO tune
	{
		return 0;// om overflow står vi nog stilla
	}
	else if(rightSensor[CurrentLeftSensor]==0)
	{
		return 0;
	}
	else
	{
		uint16_t meanVal = filterRotSamples(rightSensor, NUMROTSAMPLES, 40, CurrentRightSensor);//TODO kolla threshold så det fungerar
		uint8_t t = 2*WHEELDIAM*PI/(meanVal*NUMBEROFSTRIPES*TICKTIME);//TODO kolla så den inte avrunda decimaltalen
		return t;
	}
}

uint8_t calcVelocityLeft(void)
{
	volatile uint16_t currentTime = TCNT1;
	if((currentTime>32300)||(leftOverflow>0))//TODO tune
	{
		return 0;// om overflow står vi nog stilla, >8cm/s == stilla
	}
	else if(leftSensor[CurrentLeftSensor]==0)
	{
		return 0;
	}
	else
	{
		uint16_t meanVal = filterRotSamples(leftSensor, NUMROTSAMPLES, 40, CurrentLeftSensor);//TODO kolla threshold så det fungerar
		uint8_t t = 2*WHEELDIAM*PI/(meanVal*NUMBEROFSTRIPES*TICKTIME);//TODO kolla så den inte avrunda decimaltalen
		return t;
	}
}
//turn off optimization 
#pragma GCC push_options
#pragma GCC optimize ("O0")
uint16_t filterRotSamples(volatile uint16_t  *samples, uint8_t numOfSamples, uint8_t threshold, uint8_t start)
{
	//räkna ut medelvärde
	//ta bort för långa värden som uppkommer pga avbrott
	//sluta läsa av arrayen om roboten står still
	uint8_t currentNumInRow=0;
	uint16_t currentSum=0;
	uint8_t i = 0;
	uint8_t n = start;
	while(i < numOfSamples-1)
	{
		if(absThreshold(samples[n], samples[n-1]) < threshold)
		{
			currentNumInRow++;
			currentSum+=samples[n];//summera
		}
		n--;
		i++;
		if(n<0)
		{
			n=numOfSamples-1;
		}
	}
	if(absThreshold(samples[start], samples[start+1]) < threshold)
	{
		currentNumInRow++;
		currentSum+=samples[start+1];//summera
	}
	if(currentNumInRow!=0)
	{
		uint16_t ttt = (uint16_t)(currentSum/currentNumInRow);//return result
		return ttt;//return result
	}
	else
	{
		return samples[start];//om vi har en -x^2 dvs derivatan runt den ser ish ut som en triangel.
	}
}
#pragma GCC pop_options
//end turn off optimization 
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
	if(pinStateLastRight == 0)
	{
		if(PIND &  (1<<PIND6))
		{
			pinChangeCounterRight++;
			pinStateLastRight = 1;
		}
	}	
	if(pinStateLastRight == 1)
	{
		if(!(PIND & (1<<PIND6)))
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
	if(pinStateLastLeft == 0)
	{
		if(PINC & (1<<PINC0))
		{
			pinChangeCounterLeft++;
			pinStateLastLeft = 1;
		}
	}
	if(pinStateLastLeft == 1)
	{
		if(!(PINC & (1<<PINC0)))
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

ISR(PCINT3_vect) //pin20
{
	CurrentRightSensor++;//uppdatera precis innan så den alltid pekar på senaste värdet
	if(NUMROTSAMPLES<=CurrentRightSensor)
	{
		CurrentRightSensor=0;
	}
	if(pinChangeCounterRight<=1)
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
	pinChangeCounterRight=0;
}

ISR(PCINT2_vect) //pin 22
{
	CurrentLeftSensor++;//uppdatera precis innan så den alltid pekar på senaste värdet
	if(NUMROTSAMPLES<=CurrentLeftSensor)
	{
		CurrentLeftSensor=0;
	}
	if(pinChangeCounterLeft<= 1)
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
	pinChangeCounterLeft=0;
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
