/**
* TSEA27 Elektronikprojekt
*
* IDENTIFIERING
*
* Modul:Sensorenheten
* Filnamn: Sensorenheten.h
* Skriven av: Johan Källström
* Datum: 2013-05-15
* Version: 1.0
*
* BESKRIVNING
*
* Mainfilen för Sensorenheten
*/	

/*
* Skapar ett sensormedelande med filtrerat mätdata utifrån de globala samplingsarrayerna.
*/
void constructSensorMessage(uint8_t *msg, uint8_t *len);

