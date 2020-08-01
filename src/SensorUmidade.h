
#ifndef _SensorUmidade_H_
#define _SensorUmidade_H_

#include <Constantes.h>

typedef enum {
    DEMO  = -1,   // for compatibility with WiFi Shield library
    ERROR = 0,
    SUCESSO  = 1
} SensorUmidade_status_t;

typedef struct {
	float value;
	float rawValue;
	SensorUmidade_status_t status;	
	char* msgError;
} SensorUmidadeResult;

int nroSensores = 0;
int digitalPinsSensorUmidade[4] = {14,12,13,15};
uint8_t analogPin;


// Write the state of a group of pins
void setOff(int pins[], int len) {
  for (int i = 0; i < 4; i++)
  {
	  if ( pins[i] > 0 )
	  {
    	digitalWrite(pins[i], LOW);
		//Serial.printf("SU:: SET PIN[%d] - %d = LOW\n", i, pins[i]);
	  }
  }
  //Serial.printf("SU:: SET %d PINs to LOW\n", (len+1));
  delay(100);
}


void SensorUmidadeSetupPins(int digitalPins[], int qtdValues){
	for(int i=0; i< qtdValues && i < 4; i++){
		digitalPinsSensorUmidade[i] = digitalPins[i];
		
	}
	for(int i = qtdValues; 4-qtdValues && i < 4; i++){
		digitalPinsSensorUmidade[i] = -1;
	}
	for(int i =0; i < 4; i++){
		if(digitalPinsSensorUmidade[i] >0){
			pinMode(digitalPinsSensorUmidade[i], OUTPUT);
		}
	}
	pinMode(analogPin, INPUT);
	setOff(digitalPinsSensorUmidade, 4);	
}

bool SensorUmidadeInit(int qtdSensoresConectados, uint8_t analogPin = A0, bool EhDemo = false)
{	
	Serial.print("SU:: Configurando senores de umidade... ");
	if(!between(qtdSensoresConectados, 1, 4) || EhDemo){
		nroSensores = 0;
	} else {
		nroSensores = qtdSensoresConectados;	
	}
	Serial.printf(" %d configurados [Modo demonstração].",nroSensores, EhDemo);
	SensorUmidadeSetupPins(digitalPinsSensorUmidade, 4);
	return true;
}

/**
 * Recupera o valor do sensor informado. 
 * 
 */
void obterValorUmidade(int sensorNro, SensorUmidadeResult *resultado, int in_min = 250, int in_max=1024)
{
	if( nroSensores == 0 ){	
		resultado->status = SensorUmidade_status_t::DEMO;
		resultado->value = millis();
		resultado->rawValue = resultado->value;
	} else {
		if ( ! between(sensorNro, 1, nroSensores)){
			resultado->status = SensorUmidade_status_t::ERROR;
			resultado->msgError = "O sensor informado não está dentro das configurações estabelecidas.";
			resultado->value = -1;
			resultado->rawValue = -1;
		}	
		sensorNro--;
		//We will use the map function to map the values read on the analog input range from 0 to 1023, and will be remapped to another range from 0 to 100%.
		setOff(digitalPinsSensorUmidade, nroSensores);		
		digitalWrite(digitalPinsSensorUmidade[sensorNro], HIGH);
		delay(500);
		float amostra_umid = analogRead(analogPin);
		resultado->rawValue = amostra_umid;
		delay(500);
		setOff(digitalPinsSensorUmidade, nroSensores);
		Serial.printf("SU:: Sensor %d [porta %d] = %f\n", sensorNro, digitalPinsSensorUmidade[sensorNro], amostra_umid);
		resultado->value = map ( amostra_umid, in_min, in_max, 100, 0 );
		resultado->status = SensorUmidade_status_t::SUCESSO;		
	}	
}
#endif //_SensorUmidade_H_