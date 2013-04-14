/*
 * Sensorenheten.c
 *
 * Created: 4/10/2013 1:47:53 PM
 *  Author: tobno568
 */ 

#define F_CPU 8000000UL // 8mhz 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "Sensorenheten.h"
#include "rotsensor.h"
#include "distsensor.h"
#include "../../TSEA27-include/SPI/mspi.h"

int main(void)
{	
	Init_rotsensor();
	Init_distsensor();
	sei();//enable interupts
	MSPI_init_master();
	uint8_t receivedData1;
	uint8_t receivedData2;

	

	uint8_t sensorDataMsg[12];
	sensorDataMsg[0]=0x00|12;
	
	while(1)
	{
		_delay_ms(100);

		MSPI_SS_LOW;
		MSPI_exchange(0b10010100);
		receivedData1=MSPI_exchange(0b00000000);
		receivedData2=MSPI_exchange(0b00000000);
			
		MSPI_exchange(0b10010100);
		receivedData1=MSPI_exchange(0b00000000);
		receivedData2=MSPI_exchange(0b00000000);
			
			
		MSPI_exchange(0b10000000);
		receivedData1=MSPI_exchange(0b00000000);
		receivedData2=MSPI_exchange(0b00000000);
		MSPI_SS_HIGH;

		//constuct sensor message, move to somewhere else
		sensorDataMsg[1]= longDistSensor(filterSampleArray(distSensor0, 10, 5));
		sensorDataMsg[2] = longDistSensor(filterSampleArray(distSensor1, 10, 5));
		sensorDataMsg[3] = longDistSensor(filterSampleArray(distSensor2, 10, 5));
		sensorDataMsg[4] = longDistSensor(filterSampleArray(distSensor3, 10, 5));
		sensorDataMsg[5] = shortDistSensor(filterSampleArray(distSensor4, 10, 5));
		sensorDataMsg[6] = shortDistSensor(filterSampleArray(distSensor5, 10, 5));
		sensorDataMsg[7] = shortDistSensor(filterSampleArray(distSensor6, 10, 5));
		sensorDataMsg[8] = shortDistSensor(filterSampleArray(distSensor7, 10, 5));
		sensorDataMsg[9] = receivedData1;//GYRO
		sensorDataMsg[10] = receivedData2;//GYRO
		sensorDataMsg[11] = calcVelocityRight();//rot höger	mm/sek
		sensorDataMsg[12] = calcVelocityLeft();//rot vänster mm/sek

	}
	return 0;
}
