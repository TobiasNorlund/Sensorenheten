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
#include "gyro.h"
#include "distsensor.h"
#include "../../TSEA27-include/message.h"
#include "../../TSEA27-include/SPI/mspi.h"
#include "../../TSEA27-include/SPI/spi_slave.h"

uint8_t test[32];

int main(void)
{	
	Init_rotsensor();
	Init_distsensor();
	SPI_SLAVE_init();
	MSPI_init_master();
	Init_gyro();
	
	sei();//enable interupts

	// används för att skicka
	uint8_t msgR[32];
	uint8_t lenR;	
	uint8_t typeR;
	// används för att taemot
	uint8_t msg[16];
	uint8_t len;
	uint8_t type;
	
	while(1)
	{
		constructSensorMessage(test,&lenR);
		
		if(SPI_SLAVE_read(msg, &type, &len))//om det finns ett helt medelande
		{
			switch (type)
			{
				case TYPE_REQUEST_SENSOR_DATA:
					constructSensorMessage(msgR, &lenR);
					SPI_SLAVE_write(msgR, TYPE_REPLY_SENSOR_DATA, lenR);
					break;
			}			
		}
	}
	return 0;
}



void constructSensorMessage(uint8_t *msg, uint8_t *len)
{
	
	//constuct sensor message
	msg[0] = IDSENSOR1;
	msg[1] = longDistSensor(filterSampleArray(distSensor0, NUMSAMPLES));
	msg[2] = IDSENSOR2;
	msg[3] = longDistSensor(filterSampleArray(distSensor1, NUMSAMPLES));
	msg[4] = IDSENSOR3;
	msg[5] = longDistSensor(filterSampleArray(distSensor2, NUMSAMPLES));
	msg[6] = IDSENSOR4;
	msg[7] = longDistSensor(filterSampleArray(distSensor3, NUMSAMPLES));
	msg[8] = IDSENSOR5;
	msg[9] = shortDistSensor(filterSampleArray(distSensor4, NUMSAMPLES));
	msg[10] = IDSENSOR6;
	msg[11] = shortDistSensor(filterSampleArray(distSensor5, NUMSAMPLES));
	msg[12] = IDSENSOR7;
	msg[13] = shortDistSensor(filterSampleArray(distSensor6, NUMSAMPLES));
	msg[14] = IDSENSOR8;
	msg[15] = shortDistSensor(filterSampleArray(distSensor7, NUMSAMPLES));
	msg[16] = IDGYROSENSOR;
	uint16_t gyroMsg = gyroLookUp(gyroData[currentGyroCell]); //TODO fixa medelvärdesfilter
	msg[17] = (gyroMsg&0xFF00)>>8;//GYRO 
	msg[18] = gyroMsg&0x00FF;//GYRO
	msg[19] = IDSPEEDRIGHT;
	msg[20] = calcVelocityRight();//rot höger cm/sek
	msg[21] = IDSPEEDLEFT;
	msg[22] = calcVelocityLeft();//rot vänster cm/sek
}