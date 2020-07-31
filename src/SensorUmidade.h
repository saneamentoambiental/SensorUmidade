#ifndef _SensorUmidade_H_
#define _SensorUmidade_H_

#include <Arduino.h>
#include <Constantes.h>

typedef enum {
    DEMO  = -1,   // for compatibility with WiFi Shield library
    ERROR = 0,
    Lido  = 1
} SensorUmidade_status_t;

typedef struct {
	float value;
	SensorUmidade_status_t status;	
	String msgError;
} SensorUmidadeResult;

int nroSensores = 0;
int digitalPins[4];
uint8_t analogPin;

#endif //_SensorUmidade_H_