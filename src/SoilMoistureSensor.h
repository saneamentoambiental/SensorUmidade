#ifndef SoilMoistureSensor_H
#define SoilMoistureSensor_H

#include <Arduino.h>


enum SoilMoistureSensorStatus {
    SMSS_DEMO  = -1,   // for compatibility with WiFi Shield library
    SMSS_ERROR = 0,
	SMSS_UNPLUGED = 1,
    SMSS_ON = 2,
	SMSS_OFF = 3
};

class SoilMoistureSensor {
	private:
		int powerPin;
		unsigned int readQtd = 3;
		unsigned int analogPin = A0;
		unsigned int delayTime = 150;
		
		SoilMoistureSensorStatus status = SoilMoistureSensorStatus::SMSS_UNPLUGED;
		void powerOn(){
			digitalWrite(this->powerPin, HIGH);
			this->status = SoilMoistureSensorStatus::SMSS_ON;
			delay(this->delayTime);
		}
		void powerOff(){
			digitalWrite(this->powerPin, LOW);
			this->status = SoilMoistureSensorStatus::SMSS_OFF;
			delay(this->delayTime);
		}

	public:
		SoilMoistureSensor();
		void setup(int powerPin, unsigned int readQtd = 3U, unsigned int analogPin = A0);
		float getMoistureValue(const unsigned int in_min = 0, const unsigned int in_max = 1024);
		float getMoistureRawValue();
		SoilMoistureSensorStatus getStatus();
		int getPowerPin(){
			return this->powerPin;
		}


};
#endif