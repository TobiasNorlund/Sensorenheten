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
#include "../../TSEA27-include/message.h"
#include "../../TSEA27-include/SPI/mspi.h"
#include "../../TSEA27-include/SPI/spi_slave.h"

int main(void)
{	
	Init_rotsensor();
	Init_distsensor();
	SPI_SLAVE_init();
	sei();//enable interupts
	MSPI_init_master();

	// används för att skicka
	uint8_t msgR[16];
	uint8_t lenR;	
	uint8_t typeR;
	// används för att taemot
	uint8_t msg[16];
	uint8_t len;
	uint8_t type;
	while(1)
	{
		if(!SPI_SLAVE_read(msg, &type, &len))
		{
			switch (type)
			{
				case TYPE_REQUEST_SENSOR_DATA:
					readGyro();//TODO move to timed interupt
					constructSensorMessage(msgR, &lenR);
					SPI_SLAVE_write(msgR, TYPE_REPLY_SENSOR_DATA, lenR);
					break;
			}			
		}
	}
	return 0;
}

void readGyro(void)
{
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
}

void constructSensorMessage(uint8_t *msg, uint8_t *len)
{
	//constuct sensor message, move to somewhere else
	msg[0]= longDistSensor(filterSampleArray(distSensor0, 10, 5));
	msg[1] = longDistSensor(filterSampleArray(distSensor1, 10, 5));
	msg[2] = longDistSensor(filterSampleArray(distSensor2, 10, 5));
	msg[3] = longDistSensor(filterSampleArray(distSensor3, 10, 5));
	msg[4] = shortDistSensor(filterSampleArray(distSensor4, 10, 5));
	msg[5] = shortDistSensor(filterSampleArray(distSensor5, 10, 5));
	msg[6] = shortDistSensor(filterSampleArray(distSensor6, 10, 5));
	msg[7] = shortDistSensor(filterSampleArray(distSensor7, 10, 5));
	msg[8] = receivedData1;//GYRO
	msg[9] = receivedData2;//GYRO
	msg[10] = calcVelocityRight();//rot höger	cm/sek
	msg[11] = calcVelocityLeft();//rot vänster cm/sek
}