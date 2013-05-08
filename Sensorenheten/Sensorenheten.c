/*
 * Sensorenheten.c
 *
 * Created: 4/10/2013 1:47:53 PM
 *  Author: tobno568
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

#include "Sensorenheten.h"
#include "rotsensor.h"
#include "gyro.h"
#include "distsensor.h"
#include "../../TSEA27-include/utils.h"
#include "../../TSEA27-include/message.h"
#include "../../TSEA27-include/SPI/mspi.h"
#include "../../TSEA27-include/SPI/spi_slave.h"

//#define F_CPU 20000000UL // 20mhz
#include <util/delay.h>

int16_t test,test2,test3;

volatile uint8_t timer2_Overflow;
#define GET_TIMESTAMP (((uint16_t)timer2_Overflow << 8)|(uint16_t)TCNT0)

int main(void)
{	

	Init_rotsensor();
	Init_distsensor();
	SPI_SLAVE_init();
	MSPI_init_master();
	Init_gyro();
	// init 8 sek /16 bit timestamp
	TCCR2B=(1<<CS10)|(0<<CS11)|(1<<CS12);//clk/1024 (From prescaler)
	TIMSK2=(1<<TOIE2);//overflow interupt
	TCNT2=0;//init value for counter 0
	timer2_Overflow=0;
	// end init 8 sek /16 bit timestamp
	uint16_t autoCalibrateGyroTimestamp=0;
	sei();//enable interupts
	for (uint8_t i = 0; i < 10; i++)
	{
		updateGyroData();
	}
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
		updateGyroData();
		// auto calibrate Gyro
		// Om Höger och vänster hjul snurrar lika fort
		// 0.000128 * 5 = 0.00064 sek
		// och mer än 2 sek sedan senast.
		if((absThreshold(rightSensor[CurrentRightSensor], leftSensor[CurrentLeftSensor]) < 5)&&(rightSensorOverFlow[CurrentRightSensor]==leftSensorOverFlow[CurrentLeftSensor])&&(2/0.0000512f < absThreshold(autoCalibrateGyroTimestamp,GET_TIMESTAMP)))
		{
			calibrateGyro(10);//max 10 i komp per gång vi kalibrerar
			autoCalibrateGyroTimestamp=GET_TIMESTAMP;
		}
		updatePinToggleCounter();
	}
	return 0;
}


//turn off optimization
#pragma GCC push_options
#pragma GCC optimize ("O0")
void constructSensorMessage(uint8_t *msg, uint8_t *len)
{
	
	//constuct sensor message
	msg[0] = LONGFRONT;
	msg[1] = longDistSensor(median(distSensor[0], NUMSAMPLES));
	msg[2] = LONGRIGHT;
	msg[3] = longDistSensor(median(distSensor[1], NUMSAMPLES));
	msg[4] = LONGREAR;
	msg[5] = longDistSensor(median(distSensor[2], NUMSAMPLES));
	msg[6] = LONGLEFT;
	msg[7] = longDistSensor(median(distSensor[3], NUMSAMPLES));
	msg[8] = SHORTFRONTRIGHT;
	msg[9] = shortDistSensor(median(distSensor[4], NUMSAMPLES));
	msg[10] = SHORTFRONTLEFT;
	msg[11] = shortDistSensor(median(distSensor[5], NUMSAMPLES));
	msg[12] = SHORTREARRIGHT; 
	msg[13] = shortDistSensor(median(distSensor[6], NUMSAMPLES));
	msg[14] = SHORTREARLEFT;
	msg[15] = shortDistSensor(median(distSensor[7], NUMSAMPLES));
	msg[16] = IDGYROSENSOR;
	uint16_t tGyro;
	tGyro = filterSampleArrayMeanPlusPlus(gyroData, NUMGYROSAMPLES,5);
	int16_t gyroMsg; 
	gyroMsg = gyroLookUp(tGyro);
	msg[17] = (gyroMsg>>8);//GYRO 
	msg[18] = gyroMsg&0x00FF;//GYRO
	msg[19] = IDSPEEDRIGHT;
	msg[20] = calcVelocityRight();//rot höger cm/sek
	msg[21] = IDSPEEDLEFT;
	msg[22] = calcVelocityLeft();//rot vänster cm/sek
	// sätt längd
	*len = 23;
	
}
#pragma GCC pop_options

ISR(TIMER2_OVF_vect) {
	timer2_Overflow++;
}