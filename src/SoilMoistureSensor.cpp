#include <SoilMoistureSensor.h>

SoilMoistureSensor::SoilMoistureSensor(){
	this->status = SoilMoistureSensorStatus::SMSS_UNPLUGED;
}

void SoilMoistureSensor::setup(int powerPin, unsigned int readQtd,unsigned int analogPin){
	if ( readQtd < 1 ){
		readQtd = 1;
	}
	this->powerPin = powerPin;
	this->analogPin = analogPin;
	this->readQtd = readQtd;
	this->powerOff();
}


int SoilMoistureSensor::getMoistureRawValue()
{
	powerOn();
	int value = analogRead(this->analogPin);
	powerOff();
	return value;	
}

float SoilMoistureSensor::getMoistureValue(){
	powerOn();
	float media = 0;
	for( unsigned int i = 0; i < this->readQtd; i++ ){
		media += analogRead(this->analogPin);
		delayMicroseconds(50);
	}
	powerOff();
	return media/this->readQtd;
	
}
SoilMoistureSensorStatus SoilMoistureSensor::getStatus(){
	return this->status;
}

