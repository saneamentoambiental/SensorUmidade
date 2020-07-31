
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
	SensorUmidade_status_t status;	
	char* msgError;
} SensorUmidadeResult;

int nroSensores = 0;
int digitalPinsSensorUmidade[4];
uint8_t analogPin;



bool SensorUmidadeSetup(int qtdSensoresConectados, uint8_t analogPin = A0, bool EhDemo = false)
{	
	Serial.print("Configurando senores de umidade... ");
	if(!between(qtdSensoresConectados, 1, 4) || EhDemo){
		nroSensores = 0;
	} else {
		nroSensores = qtdSensoresConectados;	
	}
	Serial.printf(" %d configurados [Modo demonstração].",nroSensores, EhDemo);
	return true;
}

// Write the state of a group of pins
void setOff(int pins[], int len) {
  for (int i = 0; i < len; i++)
  {
    digitalWrite(pins[i], LOW);
  }
  delay(10);
}

/**
 * Recupera o valor do sensor informado. 
 * 
 */
void obterValorUmidade(int sensorNro, SensorUmidadeResult *resultado, int in_min = 1, int in_max=1024)
{
	if( nroSensores == 0 ){	
		resultado->status = SensorUmidade_status_t::DEMO;
		resultado->value = millis();
	} else {
		if ( ! between(sensorNro, 1, nroSensores)){
			resultado->status = SensorUmidade_status_t::ERROR;
			resultado->msgError = "O sensor informado não está dentro das configurações estabelecidas.";
			resultado->value = -1;
		}	
		sensorNro--;
		//We will use the map function to map the values read on the analog input range from 0 to 1023, and will be remapped to another range from 0 to 100%.
		setOff(digitalPinsSensorUmidade, nroSensores);
		
		digitalWrite(digitalPinsSensorUmidade[sensorNro], HIGH);
		float amostra_umid = analogRead(analogPin);
		setOff(digitalPinsSensorUmidade, nroSensores);

		resultado->value = map ( amostra_umid, in_min, in_max, 100, 0 );
		resultado->status = SensorUmidade_status_t::SUCESSO;		
	}	
}
#endif //_SensorUmidade_H_