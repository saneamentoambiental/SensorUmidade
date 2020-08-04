#ifndef SoilMoistureManager_H
#define SoilMoistureManager_H

#include <Arduino.h>
#include <SoilMoistureSensor.h>

#ifndef SoilMoistureManager_MAX_SENSORS
	#define SoilMoistureManager_MAX_SENSORS 8
#endif

#define getSizeArray(x)  (sizeof(x) / sizeof((x)[0]))


class SoilMoistureManager {
	private:	
		std::vector<SoilMoistureSensor> sensoresSolo;
		unsigned int qtdSensores = 0;
	public:
		SoilMoistureManager(){

		}
		SoilMoistureManager(int *digitalPins, unsigned int qtdSensores)
		{
			Serial.println("Constructor");
			
			Serial.println(qtdSensores);
			this->qtdSensores = qtdSensores;
			//sensoresSolo = new SoilMoistureSensor[qtdSensores];
			Serial.println("Array - OK"); 
			delay(2000);
			Serial.println("For 1");

			for(unsigned int i = 0; i< qtdSensores; i++){
				SoilMoistureSensor s = SoilMoistureSensor();
				s.setup(digitalPins[i]);
				this->sensoresSolo.push_back(s);
				//sensoresSolo[i] = s;
			}
			// Serial.println("For 2");
			// // delay(2000);
			// // for(unsigned int i = qtdPins; i < SoilMoistureManager_MAX_SENSORS; i++){
			// // 	s = new SoilMoistureSensor();
			// // 	this->sensoresSolo[i] = *s;
			// // }
		}
		std::vector<SoilMoistureSensor> getSensores(){
			return this->sensoresSolo;
		}
		unsigned int getQtdSensors(){
			return this->qtdSensores;
		}
};


#endif
