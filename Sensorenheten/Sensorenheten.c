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
	MSPI_init_master();

	//timed interupt init	
	TIMSK0 = (1<<OCIE0A);// Enable Interrupt TimerCounter0 Compare Match A (SIG_OUTPUT_COMPARE0A)
	TCCR0A = (1<<WGM01); // Mode = CTC, clear on compare, dvs reseta räknaren
	TCCR0B = (1<<CS02)|(0<<CS01)|(1<<CS00);// Clock/1024, 0.000128 seconds per tick
	OCR0A = 0.02f/0.000128f; // 0.2f/0.000128f ger 50 gånger i sekunden 1/50= 0.02
	currentGyroCell = 0;
	//end timed interupt init
	
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

//timed interup
ISR(TIM0_COMPA_vect)
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
	
	
	MSPI_exchange(0b10000000);
	receivedData1=MSPI_exchange(0b00000000);
	receivedData2=MSPI_exchange(0b00000000);
	MSPI_SS_HIGH;
	
	currentGyroCell++;
	if(NUMGYROSAMPLES<=currentGyroCell)
	{
		currentGyroCell=0;
	}
	gyroData[currentGyroCell]=(((uint16_t)receivedData1)<<8)|(uint16_t)receivedData2;
	
}

void constructSensorMessage(uint8_t *msg, uint8_t *len)
{
	//constuct sensor message
	msg[0] = IDSENSOR1;
	msg[1] = longDistSensor(filterSampleArray(distSensor0, 10));
	msg[2] = IDSENSOR2;
	msg[3] = longDistSensor(filterSampleArray(distSensor1, 10));
	msg[4] = IDSENSOR3;
	msg[5] = longDistSensor(filterSampleArray(distSensor2, 10));
	msg[6] = IDSENSOR4;
	msg[7] = longDistSensor(filterSampleArray(distSensor3, 10));
	msg[8] = IDSENSOR5;
	msg[9] = shortDistSensor(filterSampleArray(distSensor4, 10));
	msg[10] = IDSENSOR6;
	msg[11] = shortDistSensor(filterSampleArray(distSensor5, 10));
	msg[12] = IDSENSOR7;
	msg[13] = shortDistSensor(filterSampleArray(distSensor6, 10));
	msg[14] = IDSENSOR8;
	msg[15] = shortDistSensor(filterSampleArray(distSensor7, 10));
	msg[16] = IDGYROSENSOR;
	msg[17] = gyroData[currentGyroCell]&0b1111111100000000;//GYRO TODO fixa medelvärdesfilter
	msg[18] = gyroData[currentGyroCell]&0b0000000011111111;//GYRO
	msg[19] = IDSPEEDRIGHT;
	msg[20] = calcVelocityRight();//rot höger	cm/sek
	msg[21] = IDSPEEDLEFT;
	msg[22] = calcVelocityLeft();//rot vänster cm/sek
}