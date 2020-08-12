#ifndef SoilMoistureManager_H
#define SoilMoistureManager_H

#include <Arduino.h>
#include <SoilMoistureSensor.h>

#ifndef SoilMoistureManager_MAX_SENSORS
	#define SoilMoistureManager_MAX_SENSORS 8
#endif

//#define getSizeArray(x)  (sizeof(x) / sizeof((x)[0]))


class SoilMoistureManager {
	private:	
		std::vector<SoilMoistureSensor> sensoresSolo;
		unsigned int qtdSensores = 0;
	public:
		SoilMoistureManager(){

		}
		SoilMoistureManager(int *digitalPins, unsigned int qtdSensores)
		{
			Serial.printf("Setup %d sensors...\n", qtdSensores);
			
			//Serial.println(qtdSensores);
			this->qtdSensores = qtdSensores;
			//sensoresSolo = new SoilMoistureSensor[qtdSensores];
			Serial.println("Array - OK"); 
			delay(2000);
			Serial.println("For 1");

			for(unsigned int i = 0; i< qtdSensores; i++){
				SoilMoistureSensor s = SoilMoistureSensor();
				s.setup(digitalPins[i],3);
				Serial.printf("Sensor %d => port %d\n", i+1, digitalPins[i]);
				this->sensoresSolo.push_back(s);
			}
		}
		std::vector<SoilMoistureSensor> getSensores(){
			return this->sensoresSolo;
		}
		unsigned int getQtdSensors(){
			return this->qtdSensores;
		}

		void printStatus(){
			Serial.printf(":::: Soil Moisture Manager ::::\n");
			Serial.printf(":: Size: %d\n", this->getQtdSensors());
			for( unsigned i = 0; i < this->getQtdSensors(); i++){
				SoilMoistureSensor s =  this->sensoresSolo[i];
				Serial.printf(":: \t - Sensor pin nro %d has on? %s\n", s.getPowerPin(), s.getStatus());
			}
			Serial.printf("::::::::::::::::::::::::::::::\n");

		}
		
};


#endif
