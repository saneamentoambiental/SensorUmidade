#include <SoilMoistureSensor.h>

SoilMoistureSensor::SoilMoistureSensor(){
	this->status = SoilMoistureSensorStatus::SMSS_OFF;
	pinMode(analogPin, INPUT);

}

void SoilMoistureSensor::setup(int powerPin, unsigned int readQtd, unsigned int analogPin){
	if ( readQtd < 1 ){
		readQtd = 1;
	}
	this->powerPin = powerPin;
	this->analogPin = analogPin;
	this->readQtd = readQtd;
	this->powerOff();
	pinMode(this->powerPin, OUTPUT);
	SoilMoistureSensor();
}


float SoilMoistureSensor::getMoistureRawValue()
{
	powerOn();
	float media = 0;
	for( unsigned int i = 0; i < this->readQtd; i++ ){
		media += analogRead(this->analogPin);
		delayMicroseconds(150);
	}
	powerOff();
	return media/this->readQtd;
}

float SoilMoistureSensor::getMoistureValue(const unsigned int in_min, const unsigned int in_max ){
	return map(this->getMoistureRawValue(), in_min, in_max, 100, 0);
	
}
SoilMoistureSensorStatus SoilMoistureSensor::getStatus(){
	return this->status;
}

