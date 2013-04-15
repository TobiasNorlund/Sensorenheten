/*
 * Sensorenheten.h
 *
 * Created: 4/10/2013 2:39:25 PM
 *  Author: tobno568
 */ 
void readGyro(void);
void constructSensorMessage(uint8_t *msg, uint8_t *len);

//gyro data
uint8_t receivedData1;
uint8_t receivedData2;
