﻿/*
 * gyro.c
 *
 * Created: 4/16/2013 9:48:15 AM
 *  Author: johka546
 */
#include "gyro.h"
int8_t lookUpGyro[1024] = {
500
};
 
void Init_gyro(void)
{
	//timed interupt init
	TIMSK0 = (1<<OCIE0A);// Enable Interrupt TimerCounter0 Compare Match A (SIG_OUTPUT_COMPARE0A)
	TCCR0A = (1<<WGM01); // Mode = CTC, clear on compare, dvs reseta räknaren
	TCCR0B = (1<<CS02)|(0<<CS01)|(1<<CS00);// Clock/1024, 0.000128 seconds per tick
	OCR0A = 0.02f/0.000128f; // 0.2f/0.000128f ger 50 gånger i sekunden 1/50= 0.02
	currentGyroCell = 0;
	//end timed interupt init																											
}

uint16_t gyroLookUp(uint16_t sample)
{
	return lookUpGyro[sample];
}

//timed interup
ISR(TIMER0_COMPA_vect)
{
	//gyro data
	uint8_t receivedData1;
	uint8_t receivedData2;

	MSPI_SS_LOW;
	MSPI_exchange(0b10010100);
	receivedData1=MSPI_exchange(0b00000000);
	receivedData2=MSPI_exchange(0b00000000);
	
	MSPI_exchange(0b10010100);
	receivedData1=MSPI_exchange(0b00000000);
	receivedData2=MSPI_exchange(0b00000000);
	_delay_us(130);//Instead of the polling of the EOC bit one can use a simple delay that should be bigger than a maximal conversion time (>115 µs, see table 4). 
	MSPI_exchange(0b10000000);
	receivedData1=MSPI_exchange(0b00000000);
	receivedData2=MSPI_exchange(0b00000000);
	MSPI_SS_HIGH;
	if(receivedData1&0b00100000)//om (EOC) is zero, the conversion is still in progress and the result of the conversion (bits AD10…AD0) is not valid.
	{
		receivedData1=0b00001111&receivedData1;
			
		currentGyroCell++;
		if(NUMGYROSAMPLES<=currentGyroCell)
		{
			currentGyroCell=0;
		}
		gyroData[currentGyroCell]=((((uint16_t)receivedData1)<<8)|(uint16_t)receivedData2)>>2;
	}
}