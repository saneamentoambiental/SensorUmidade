#include <SensorUmidade.h>


bool SensorUmidadeSetup(int qtdSensoresConectados, uint8_t analogPin = D0)
{	
	if(!between(qtdSensoresConectados, 1, 4)){
		nroSensores = 0;
	}
	nroSensores = qtdSensoresConectados -1;	
	return true;
}

// Write the state of a group of pins
void setOff(int pins[], int len) {
  for (int i = 0; i < len; i++)
  {
    digitalWrite(pins[i], LOW);
  }
}

/**
 * Recupera o valor do sensor informado. 
 * 
 */
void obterValorUmidade(int sensorNro, SensorUmidadeResult *resultado, int in_min = 1, int in_max=1024);
{
	if( ! nroSensores == 0 ){	
		resultado->status = SensorUmidade_status_t::DEMO;
		resultado->value = millis();
	} else {
		if ( ! between(sensorNro, 1, nroSensores)){
			resultado->status = SensorUmidade_status_t::ERROR;
			resultado->msgError = "O sensor informado não está dentro das configurações estabelecidas.";
			resultado->value = -1;
		}	
		//We will use the map function to map the values read on the analog input range from 0 to 1023, and will be remapped to another range from 0 to 100%.
		setOff(digitalPins, nroSensores);
		digitalWrite(digitalPins[sensorNro], HIGH);
		float amostra_umid = analogRead(analogPin);
		setOff(digitalPins, nroSensores);
		resultado->value = map ( amostra_umid, in_min, in_max, 100, 0 );
		resultado->status = SensorUmidade_status_t::Lido;
		delay(10);
	}	
}
