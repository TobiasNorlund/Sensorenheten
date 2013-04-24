/*
 * gyro.c
 *
 * Created: 4/16/2013 9:48:15 AM
 *  Author: johka546
 */
#define MAXIMUMVALUEGYRO 923	
#define MINIMUMVALUEGYRO 101
#define CENTERVALUE 	 409 //409 är 0 i lookuptable
#include "gyro.h"

const int16_t lookUpGyro[821] PROGMEM = {
-300, -299, -298, -297, -297, -296, -295, -294, -294, -293, -292, -292, -291, -290, -289, -289, -288, -287, -286, -286, -285, -284, -283, -283, -282, -281, -281, -280, -279, -278, -278, -277, -276, -275, -275, -274, -273, -272, -272, -271, -270, -270, -269, -268, -267, -267, -266, -265, -264, -264, -263, -262, -261, -261, -260, -259, -259, -258, -257, -256, -256, -255, -254, -253, -253, -252, -251, -250, -250, -249, -248, -248, -247, -246, -245, -245, -244, -243, -242, -242, -241, -240, -240, -239, -238, -237, -237, -236, -235, -234, -234, -233, -232, -231, -231, -230, -229, -229, -228, -227, -226, -226, -225, -224, -223, -223, -222, -221, -220, -220, -219, -218, -218, -217, -216, -215, -215, -214, -213, -212, -212, -211, -210, -209, -209, -208, -207, -207, -206, -205, -204, -204, -203, -202, -201, -201, -200, -199, -198, -198, -197, -196, -196, -195, -194, -193, -193, -192, -191, -190, -190, -189, -188, -188, -187, -186, -185, -185, -184, -183, -182, -182, -181, -180, -179, -179, -178, -177, -177, -176, -175, -174, -174, -173, -172, -171, -171, -170, -169, -168, -168, -167, -166, -166, -165, -164, -163, -163, -162, -161, -160, -160, -159, -158, -157, -157, -156, -155, -155, -154, -153, -152, -152, -151, -150, -149, -149, -148, -147, -146, -146, -145, -144, -144, -143, -142, -141, -141, -140, -139, -138, -138, -137, -136, -135, -135, -134, -133, -133, -132, -131, -130, -130, -129, -128, -127, -127, -126, -125, -125, -124, -123, -122, -122, -121, -120, -119, -119, -118, -117, -116, -116, -115, -114, -114, -113, -112, -111, -111, -110, -109, -108, -108, -107, -106, -105, -105, -104, -103, -103, -102, -101, -100, -100, -99, -98, -97, -97, -96, -95, -94, -94, -93, -92, -92, -91, -90, -89, -89, -88, -87, -86, -86, -85, -84, -83, -83, -82, -81, -81, -80, -79, -78, -78, -77, -76, -75, -75, -74, -73, -73, -72, -71, -70, -70, -69, -68, -67, -67, -66, -65, -64, -64, -63, -62, -62, -61, -60, -59, -59, -58, -57, -56, -56, -55, -54, -53, -53, -52, -51, -51, -50, -49, -48, -48, -47, -46, -45, -45, -44, -43, -42, -42, -41, -40, -40, -39, -38, -37, -37, -36, -35, -34, -34, -33, -32, -31, -31, -30, -29, -29, -28, -27, -26, -26, -25, -24, -23, -23, -22, -21, -21, -20, -19, -18, -18, -17, -16, -15, -15, -14, -13, -12, -12, -11, -10, -10, -9, -8, -7, -7, -6, -5, -4, -4, -3, -2, -1, -1, 0, 1, 1, 2, 3, 4, 4, 5, 6, 7, 7, 8, 9, 10, 10, 11, 12, 12, 13, 14, 15, 15, 16, 17, 18, 18, 19, 20, 21, 21, 22, 23, 23, 24, 25, 26, 26, 27, 28, 29, 29, 30, 31, 31, 32, 33, 34, 34, 35, 36, 37, 37, 38, 39, 40, 40, 41, 42, 42, 43, 44, 45, 45, 46, 47, 48, 48, 49, 50, 51, 51, 52, 53, 53, 54, 55, 56, 56, 57, 58, 59, 59, 60, 61, 62, 62, 63, 64, 64, 65, 66, 67, 67, 68, 69, 70, 70, 71, 72, 73, 73, 74, 75, 75, 76, 77, 78, 78, 79, 80, 81, 81, 82, 83, 83, 84, 85, 86, 86, 87, 88, 89, 89, 90, 91, 92, 92, 93, 94, 94, 95, 96, 97, 97, 98, 99, 100, 100, 101, 102, 103, 103, 104, 105, 105, 106, 107, 108, 108, 109, 110, 111, 111, 112, 113, 114, 114, 115, 116, 116, 117, 118, 119, 119, 120, 121, 122, 122, 123, 124, 125, 125, 126, 127, 127, 128, 129, 130, 130, 131, 132, 133, 133, 134, 135, 135, 136, 137, 138, 138, 139, 140, 141, 141, 142, 143, 144, 144, 145, 146, 146, 147, 148, 149, 149, 150, 151, 152, 152, 153, 154, 155, 155, 156, 157, 157, 158, 159, 160, 160, 161, 162, 163, 163, 164, 165, 166, 166, 167, 168, 168, 169, 170, 171, 171, 172, 173, 174, 174, 175, 176, 177, 177, 178, 179, 179, 180, 181, 182, 182, 183, 184, 185, 185, 186, 187, 188, 188, 189, 190, 190, 191, 192, 193, 193, 194, 195, 196, 196, 197, 198, 198, 199, 200, 201, 201, 202, 203, 204, 204, 205, 206, 207, 207, 208, 209, 209, 210, 211, 212, 212, 213, 214, 215, 215, 216, 217, 218, 218, 219, 220, 220, 221, 222, 223, 223, 224, 225, 226, 226, 227, 228, 229, 229, 230, 231, 231, 232, 233, 234, 234, 235, 236, 237, 237, 238, 239, 240, 240, 241, 242, 242, 243, 244, 245, 245, 246, 247, 248, 248, 249, 250, 250, 251, 252, 253, 253, 254, 255, 256, 256, 257, 258, 259, 259, 260, 261, 261, 262, 263, 264, 264, 265, 266, 267, 267, 268, 269, 270, 270, 271, 272, 272, 273, 274, 275, 275, 276, 277, 278, 278, 279, 280, 281, 281, 282, 283, 283, 284, 285, 286, 286, 287, 288, 289, 289, 290, 291, 292, 292, 293, 294, 294, 295, 296, 297, 297, 298, 299, 300, 300, 301,
};
 
void Init_gyro(void)
{
	//timed interupt init
	//TIMSK0 = (1<<OCIE0A);// Enable Interrupt TimerCounter0 Compare Match A (SIG_OUTPUT_COMPARE0A)
	TCCR0A = (1<<WGM01); // Mode = CTC, clear on compare, dvs reseta räknaren
	TCCR0B = (1<<CS02)|(0<<CS01)|(1<<CS00);// Clock/1024, 0.000128 seconds per tick
	OCR0A = 255; // 0.2f/0.000128f ger 50 gånger i sekunden 1/50= 0.02
	currentGyroCell = 0;
	//end timed interupt init
	gyroCaibration=0;
}

int8_t calibrateGyro(int8_t maxCalibration)
{
	volatile uint16_t newZero = filterSampleArrayMeanPlusPlus(gyroData, NUMGYROSAMPLES, 5);
	newZero=newZero-CENTERVALUE-MINIMUMVALUEGYRO;
	int8_t tempCaibration = (int8_t)newZero;

	// Spara bara om det är mindre maxCalibration i skillnad från föregående kalibrering
	if((tempCaibration < -1*maxCalibration+gyroCaibration) || (tempCaibration > maxCalibration+gyroCaibration))
	{
		return 127;//error
	}
	else
	{
		gyroCaibration=tempCaibration;
		return gyroCaibration;
	}

}


//turn off optimization 
#pragma GCC push_options
#pragma GCC optimize ("O0")
int16_t gyroLookUp(uint16_t sample)
{
	sample=sample-gyroCaibration;
	// ska hantera om sample är utanför look up tables intervall
	if(MAXIMUMVALUEGYRO<sample)
	{
		//look up MAXIMUMVALUELONG in look up table
		return 32767;
	}
	else if(sample<MINIMUMVALUEGYRO)
	{
		//look up MINIMUMVALUELONG in look up table
		return -32678;
	}
	else
	{
		sample-=MINIMUMVALUEGYRO;
		return  pgm_read_byte(&(lookUpGyro[sample]));
	}
}
#pragma GCC pop_options
//end turn off optimization 

//timed interup
ISR(TIMER0_COMPA_vect)
{
	updateGyroData();
}

void updateGyroData(void)
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

	uint8_t i = 0;
	while((!(receivedData1&0b00100000))&&(i < 10))
	{
		MSPI_exchange(0b10000000);
		receivedData1=MSPI_exchange(0b00000000);
		receivedData2=MSPI_exchange(0b00000000);
		i++;
	}
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