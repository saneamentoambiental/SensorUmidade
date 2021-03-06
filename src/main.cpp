/**
 * IotWebConf01Minimal.ino -- IotWebConf is an ESP8266/ESP32
 *   non blocking WiFi/AP web configuration library for Arduino.
 *   https://github.com/prampec/IotWebConf 
 *
 * Copyright (C) 2018 Balazs Kelemen <prampec+arduino@gmail.com>
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

/**
 * Example: Custom HTML
 * Description:
 *   This example demostrates how to override the default look and feel of the
 *   config portal.
 * 
 *   The first customalization is to add a logo to the config page.
 * 
 *   The second customalization is a special javascript, that detects all
 *   password fields, and adds a small button after each of them with a
 *   lock on it. By pressing the lock the password became visible/hidden.
 */

#include <IotWebConf.h>
#include <main.h>

void atualizarVariaveis(){
	timeUbidots = atoi(timeUbidotsValue);
	qtdSensores = atoi(qtdSensoresValue);
}

void configSaved()
{
	atualizarVariaveis();
	//setupUbidots();
	Serial.println("Configuração atualizada.");	
}

boolean formValidator()
{
	Serial.println("Validating form.");
	boolean valid = true;

	if (ubidots && ubidots->wifiConnect(iotWebConf.getWifiSsidParameter()->getId(), iotWebConf.getWifiPasswordParameter()->getId())){
		iotWebConf.getWifiSsidParameter()->errorMessage = "Não foi possível conectar à rede informada.";
		valid = false;
	}
	Serial.println("WIFI.");

	int l = server.arg(tokenUbidotsParam.getId()).length();
	if (l < 5)
	{
		tokenUbidotsParam.errorMessage = "Token inválido!";
		valid = false;
	}

	Serial.println("TOKEN.");

	if( server.arg(qtdSensoresParam.getId()).length() > 0 ){
		int qtd = atoi(server.arg(qtdSensoresParam.getId()).c_str());
		if (qtd < 1 && qtd > 8)
		{
			qtdSensoresParam.errorMessage = "A quantidade de sensores deve ser entre 1 e 8.";
			valid = false;
		}
	}
	Serial.println("Sensores.");

	if ( server.arg(timeUbidotsParam.getId()).length() > 0 ){
		int time = atoi(server.arg(timeUbidotsParam.getId()).c_str());
		if (time < 30 && time > 86400)
		{
			timeUbidotsParam.errorMessage = "O valor deve ser entre 30 (seg) e 86400 (1 dia)!";
			valid = false;
		} else {
			if (time % 30 > 0)
			{
				timeUbidotsParam.errorMessage = "O valor deve ser múltiplo de 30 seg";
				valid = false;
			}
		}
	}
	Serial.println("Configurações estão validas?");
	Serial.println(valid);
	return valid;
}
void connectWifi(const char* ssid, const char* password);
void setupUbidots();
void setupIotWebConf(){
	
	itoa(timeUbidots, timeUbidotsValue, 10);
	itoa(qtdSensores, qtdSensoresValue, 10);


	//iotWebConf.setStatusPin(LED_BUILTIN);


	iotWebConf.getWifiPasswordParameter()->type = "text";
	iotWebConf.addParameter(&separatorUbidots);
	iotWebConf.addParameter(&tokenUbidotsParam);
	iotWebConf.addParameter(&timeUbidotsParam);

	iotWebConf.addParameter(&separatorConfigGeral);
	iotWebConf.addParameter(&qtdSensoresParam);
	iotWebConf.addParameter(&timeZoneParam);

	iotWebConf.setConfigSavedCallback(&configSaved);
	iotWebConf.setFormValidator(&formValidator);
	iotWebConf.setWifiConnectionHandler(&connectWifi);
	// -- Applying the new HTML format to IotWebConf.
	iotWebConf.setHtmlFormatProvider(&customHtmlFormatProvider);
	
	iotWebConf.init();

	
  
}
void connectWifi(const char* ssid, const char* password){
	WiFi.hostname(iotWebConf.getThingName());
	if( WiFi.begin(ssid, password) && ! ubidots )
	//if( iotWebConf.getState() == IOTWEBCONF_STATE_ONLINE && ! ubidots )
	{
		sincronizarRelogio();
		Serial.printf("Hostname = %s\n", WiFi.hostname().c_str());
		debugMsg("Conectado!");		
		setupUbidots();
	}
}

void printStatusUbidots(){
	debugMsg("::::  Info ubidots ::::: ");
	
	if ( ubidots )
	{		
		debugMsg(":: Wifi OK? "); Serial.println(ubidots->wifiConnected());
		debugMsg(":: Server OK? "); Serial.println(ubidots->serverConnected());
		debugMsg(":: IP "); Serial.println(WiFi.localIP());
		debugMsg(":: MAC "); Serial.println(WiFi.macAddress());

	}
	Serial.println("::::  ubidots (END) ::::: ");	
}
void setupUbidots(){
	if (ubidots){
		debugMsg("Dropando ubidots");
		free(ubidots);
		delete ubidots;
		debugMsg("Dropado ubidots");

	}
	debugMsg("Criando ubidots");
	ubidots = new Ubidots(tokenUbidotsValue);
	ubidots->setDebug(EhDebug);	
	//printStatusUbidots();
}
bool sendUbidots(){
	unsigned success = 0;
	for (unsigned i = 0 ; i < smm->getQtdSensors(); i++){
		char sensor[40], sensorRawValue[40];
		sprintf(sensor,"%s_S_%d", iotWebConf.getThingName(), (i));
		sprintf(sensorRawValue,"%s_S_%d_Raw", iotWebConf.getThingName(),  (i));
		SoilMoistureSensor s = smm->getSensores()[i];
		float value = s.getMoistureValue();
		float rawValue = s.getMoistureRawValue();
		if ( EhParaEnviarAoServidor )
		{
			//ubidots->add(sensor, value);
			ubidots->add(sensorRawValue, rawValue);
		} 
		Serial.printf("%s = %f [%f]\n", &sensor, value, rawValue);
		if ( ubidots->wifiConnected() )
		{
			if ( ! EhParaEnviarAoServidor || ubidots->send(iotWebConf.getThingName()))
			{
				success++;
			}
		}
	}

	// for(int i = 1; i <= qtdSensores; i++ ){
	// 	char sensor[20], sensorRawValue[20];
	// 	sprintf(sensor,"Sensor_%d", (i));
	// 	sprintf(sensorRawValue,"Sensor_%d_Raw", (i));


	// 	SensorUmidadeResult resultado;
	// 	obterValorUmidade(i, &resultado);
	// 	if ( resultado.status == SensorUmidade_status_t::ERROR)
	// 	{			
	// 		Serial.printf("%s = %s [\n%c\n]\n", &sensor, resultado.status, resultado.msgError);
	// 		Serial.println(resultado.msgError);
	// 		continue;
	// 	} else {
	// 		if ( EhParaEnviarAoServidor )
	// 		{
	// 			ubidots->add(sensor, resultado.value);
	// 			ubidots->add(sensorRawValue, resultado.rawValue);
	// 		}
	// 	}
	// 	Serial.printf("%s = %f [%d]\n", &sensor, resultado.value, resultado.status);
	
	// 	if ( ubidots->wifiConnected() )
	// 	{
	// 		if ( ! EhParaEnviarAoServidor || ubidots->send(iotWebConf.getThingName()))
	// 		{
	// 			success++;
	// 		}
	// 	}
	// }
	if( success == smm->getQtdSensors() ){
		Serial.println("Ubidots updated.");
		ultimoEnvio = millis()/1000;
		return true;
	} else{
		Serial.printf("Ubidots error(s): \n- %d dos %d foram enviados.", success, qtdSensores);
		return false;
	}
}


void setupSensores()
{	
	smm = new SoilMoistureManager(digitalPinsSoilMoisture, qtdSensores);
}

void setup() 
{
	Serial.begin(9600);
	Serial.println("Starting up...");
	setupIotWebConf();
	
	// -- Set up required URL handlers on the web server.
	server.on("/", handleRoot);
	server.on("/config", []{ iotWebConf.handleConfig(); });
	server.on("/ubidots", pageSendUbidots);
	server.on("/time", pageSincronizarRelogio);
	server.onNotFound([](){ iotWebConf.handleNotFound(); });

	atualizarVariaveis();
	lastMilles = millis();
	
	if( WiFi.isConnected())
	{
		debugMsg("Conectado!");
		setupDateTime();
		sincronizarRelogio();
	}
	setupSensores();
	
	
	Serial.println("Ready.");
}

void loop() 
{
	// -- doLoop should be called as frequently as possible.
	iotWebConf.doLoop();
	
	currentMillis = millis();  //get the current "time" (actually the number of milliseconds since the program started)
	if (currentMillis - lastMilles >= (timeUbidots*1000))  //test whether the period has elapsed
	{
		//debugMsg("Call sendUbidots");
		sendUbidots();
		lastMilles = currentMillis;
	}
}


