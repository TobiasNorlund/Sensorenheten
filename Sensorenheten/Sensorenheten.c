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
#include "../../TSEA27-include/utils.h"
#include "../../TSEA27-include/message.h"
#include "../../TSEA27-include/SPI/mspi.h"
#include "../../TSEA27-include/SPI/spi_slave.h"

uint8_t test[32];

volatile uint8_t timer0_Overflow;
#define GET_TIMESTAMP (((uint16_t)timer0_Overflow << 8)|(uint16_t)TCNT0)

int main(void)
{	
	Init_rotsensor();
	Init_distsensor();
	SPI_SLAVE_init();
	MSPI_init_master();
	Init_gyro();
	// init 8 sek /16 bit timestamp
	TCCR0B=(1<<CS10)|(0<<CS11)|(1<<CS12);//clk/1024 (From prescaler)
	TIMS03=(1<<TOIE0);//overflow interupt
	TCNT0=0;//init value for counter 0
	timer0_Overflow=0;
	// end init 8 sek /16 bit timestamp
	uint16_t autoCalibrateGyroTimestamp=0;
	sei();//enable interupts
	_delay_ms(100);//vänta så vi har lite värden för kalibrering av gyro, samt att vi slipper massa nollor.
	// Autokalibrera antar att vi står still när vi startar.
	calibrateGyro(20);// TODO tune

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
				case TYPE_CALIBRATE_GYRO:
					msgR[0]=calibrateGyro(10);//return calibration
					SPI_SLAVE_write(msgR, TYPE_REPLY_CALIBRATE_GYRO, 1);
					break;
			}			
		}

		// auto calibrate Gyro
		// Om Höger och vänster hjul snurrar lika fort
		// 0.000128 * 5 = 0.00064 sek
		// och mer än 2 sek sedan senast.
		if((absThreshold(rightSensor[CurrentRightSensor], leftSensor[CurrentLeftSensor]) < 5)&&(rightSensorOverFlow[CurrentRightSensor]==leftSensorOverFlow[CurrentLeftSensor])&&(2/0.000128f < absThreshold(autoCalibrateGyroTimestamp,GET_TIMESTAMP)))
		{
			calibrateGyro(10);//max 10 i komp per gång vi kalibrerar
			autoCalibrateGyroTimestamp=GET_TIMESTAMP;
		}

	}
	return 0;
}



void constructSensorMessage(uint8_t *msg, uint8_t *len)
{
	//constuct sensor message
	msg[0] = LONGFRONT;
	msg[1] = longDistSensor(filterSampleArray(distSensor0, NUMSAMPLES));
	msg[2] = LONGRIGHT;
	uint16_t t1 = filterSampleArray(distSensor1, NUMSAMPLES);
	uint8_t t2 = longDistSensor(t1);
	msg[3] = t2;
	msg[4] = LONGREAR;
	msg[5] = longDistSensor(filterSampleArray(distSensor2, NUMSAMPLES));
	msg[6] = LONGLEFT;
	msg[7] = longDistSensor(filterSampleArray(distSensor3, NUMSAMPLES));
	msg[8] = SHORTFRONTRIGHT;
	msg[9] = shortDistSensor(filterSampleArray(distSensor4, NUMSAMPLES));
	msg[10] = SHORTFRONTLEFT;
	msg[11] = shortDistSensor(filterSampleArray(distSensor5, NUMSAMPLES));
	msg[12] = SHORTREARRIGHT;
	msg[13] = shortDistSensor(filterSampleArray(distSensor6, NUMSAMPLES));
	msg[14] = SHORTREARLEFT;
	msg[15] = shortDistSensor(filterSampleArray(distSensor7, NUMSAMPLES));
	msg[16] = IDGYROSENSOR;
	uint16_t gyroMsg = gyroLookUp(filterSampleArrayMeanPlusPlus(gyroData[currentGyroCell], NUMGYROSAMPLES, 5));
	msg[17] = (gyroMsg&0xFF00)>>8;//GYRO 
	msg[18] = gyroMsg&0x00FF;//GYRO
	msg[19] = IDSPEEDRIGHT;
	msg[20] = calcVelocityRight();//rot höger cm/sek
	msg[21] = IDSPEEDLEFT;
	msg[22] = calcVelocityLeft();//rot vänster cm/sek
	// sätt längd
	*len = 23;
}

ISR(TIMER0_OVF_vect) {
	timer0_Overflow++;
}
