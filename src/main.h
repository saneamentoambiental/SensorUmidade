#ifndef _MEU_MAIN_H_
#define _MEU_MAIN_H_


#include <CustomHtmlFormatProvider.h>
#include <Constantes.h>
#include <Ubidots.h>
#include <IotWebConf.h>
#include <timeUtils.h>
#include <SensorUmidade.h>


#define STRING_LEN 128
#define NUMBER_LEN 32
#define CONFIG_VERSION "v5"


// -- Callback method declarations.
float obterValorUmidade(byte sensorNro);
void setupIotWebConf();
void configSaved();
boolean formValidator();
String getFooter();
//String criarCard(String titulo, String valor);
void handleRoot();
String obterDescricaoUmidade(byte sensorNro);
void printStatusUbidots();
String criarCardsResumo();
bool sendUbidots();
void atualizarVariaveis();

// -- Initial name of the Thing. Used e.g. as SSID of the own Access Point.
const char thingName[] = "GPSA-Solo";

// -- Initial password to connect to the Thing, when it creates an own Access Point.
const char wifiInitialApPassword[] = "12345678";

DNSServer dnsServer;
WebServer server(80);
unsigned long ultimoEnvio = -1;
unsigned long lastMilles = millis();
unsigned long currentMillis;
Ubidots* ubidots;



IotWebConf iotWebConf(thingName, &dnsServer, &server, wifiInitialApPassword, CONFIG_VERSION);

IotWebConfSeparator separatorUbidots = IotWebConfSeparator("Ubidots");
char tokenUbidotsValue[STRING_LEN];
IotWebConfParameter tokenUbidotsParam = IotWebConfParameter("Token Ubidtos", "tkUGPSA", tokenUbidotsValue, STRING_LEN, "text");
char timeUbidotsValue[NUMBER_LEN];
int timeUbidots = 3600;
IotWebConfParameter timeUbidotsParam = IotWebConfParameter("Enviar a cada (seg)", "teUGPSA", timeUbidotsValue, NUMBER_LEN, "number", "ex. 3600 (para 1 hr)", "31536000", "step='30'");
IotWebConfSeparator separatorConfigGeral = IotWebConfSeparator("Configurações Gerais");
char qtdSensoresValue[NUMBER_LEN];
byte qtdSensores = 1;
IotWebConfParameter qtdSensoresParam = IotWebConfParameter("Qtd Sensores", "qSGPSA", qtdSensoresValue, NUMBER_LEN, "number", "ex. 1", "1", "step='1' min='1' max='4'");
char timeZoneValue[STRING_LEN];
// (https://en.wikipedia.org/wiki/List_of_tz_database_time_zones)
IotWebConfParameter timeZoneParam = IotWebConfParameter("Time Zone", "tzGPSA", timeZoneValue, STRING_LEN, "text", "ex. America/Maceio", "America/Maceio");


// -- An instance must be created from the class defined above.
CustomHtmlFormatProvider customHtmlFormatProvider;

String getFooter(){
	String s = "<div class='footer'><p>Grupo de Saneamento ambiental - IFS - Versão do app: ";
	s += CONFIG_VERSION;
	s+="</p></div>";
	return s;
}


String obterDescricaoUmidade(byte sensorNro){
	return "";
}

String criarCardsResumo(){	
	String s = criarCard("Número de Sensores", qtdSensoresValue);
	s += criarCard("Enviar a cada", formatarTempo(timeUbidots) );
	s += criarCard("Último envio", formatarTempo(millis()/1000 - ultimoEnvio));
	s += criarCard("Versão do app", CONFIG_VERSION );	
	s += criarCard("WIFI OK?", formatarBool(WiFi.isConnected()));	
	s += criarCard("MAC", WiFi.macAddress());	
	s += criarCard("IP", WiFi.localIP().toString());
	s += criarCard("Time Zone", timeZoneValue);
	s += criarCard("Data/Hora", formatarDataHora(time(nullptr)));
	Serial.println("Card resumo criado");
	return s;
}

void pageSendUbidots(){
	
	bool ok = sendUbidots();
	String s = "\
<!DOCTYPE html>\
<html>\
<title>";
	s += iotWebConf.getThingName();
	s+= "</title><meta name='viewport' content='width=device-width, initial-scale=1'><style>";
	s += FPSTR(CUSTOMHTML_CSS_INNER);
	s += "</style>";
	s += "<body class='center f'>";
	s += FPSTR(CUSTOMHTML_BODY_INNER);
	s += "<div class='container row f'><div class='card row f'><div class='container lg'><header><h3>";
	s += "Sensor de Umidade</h3>";
	s += "</header>";
	s += getMenu();
	s += "</div><br></div><br></div><div class='container f'><div class='card row f'><br>\
<div class='container lg'><h4>Situação</h4></div>";
	s += criarCard("Último envio", formatarTempo(millis()/1000 - ultimoEnvio));
	s += criarCard("Dados enviados?", formatarBool(ok));
	s += criarCardsResumo();
	s +="</div><br></div>";
	s += "</body></html>";

	server.send(200, "text/html; charset=UTF-8", s);
}

void pageSincronizarRelogio(){
	setupDateTime();
	String s = "\
<!DOCTYPE html>\
<html>\
<title>";
	s += iotWebConf.getThingName();
	s+= "</title><meta name='viewport' content='width=device-width, initial-scale=1'><style>";
	s += FPSTR(CUSTOMHTML_CSS_INNER);
	s += "</style>";
	s += "<body class='center f'>";
	s += FPSTR(CUSTOMHTML_BODY_INNER);
	s += "<div class='container row f'><div class='card row f'><div class='container lg'><header><h3>";
	s += "Sensor de Umidade</h3>";
	s += "</header>";
	s += getMenu();
	s += "</div><br></div><br></div><div class='container f'><div class='card row f'><br>\
<div class='container lg'><h4>Situação</h4></div>";	
	s += criarCard("Data/Hora", formatarDataHora(time(nullptr)));
	s +="</div><br></div>";
	s += "</body></html>";

	server.send(200, "text/html; charset=UTF-8", s);

}
/**
 * Handle web requests to "/" path.
 */
void handleRoot()
{
	// -- Let IotWebConf test and handle captive portal requests.
	if (iotWebConf.handleCaptivePortal())
	{
		// -- Captive portal request were already served.
		return;
	}

	String s = "<!DOCTYPE html><html><title>";
	s += iotWebConf.getThingName();
	s+= "</title><meta name='viewport' content='width=device-width, initial-scale=1'><style>";
	s += FPSTR(CUSTOMHTML_CSS_INNER);
	s += "</style>";
	s += "<body class='center f'>";
	s += FPSTR(CUSTOMHTML_BODY_INNER);
	s += "<div class='container row f'><div class='card row f'><div class='container lg'><header><h3>";
	s += "Sensor de Umidade</h3>";
	s += "</header>";
	s += getMenu();
	s += "</div><br></div><br></div><div class='container f'><div class='card row f'><br>\
<div class='container lg'><h4>Valor dos Sensores</h4></div>";
	for(byte i = 1; i <=  qtdSensores; i++){
		SensorUmidadeResult resultado;
		obterValorUmidade(i, &resultado);
		
		
		s += "<div class='card row'><h3 class='lg'>Sensor ";
		s += i;
		s += "</h3><p background-color='";
		if(resultado.status == SensorUmidade_status_t::ERROR ){
			s += "red";
	 	} else {
			  s+="black";
		}
		s += "'>";
		s += resultado.value;
		s += " %</p><hr><h5>(";
		s += resultado.rawValue;
		s += ")</h5></div>";
	}

	s += "</div></div><div class='container f row'><div class='card row f'><br>";
	s += "<div class='container lg'><h4>Configurações Salvas</h4></div>";
	s += criarCardsResumo();
	s += "</div><br></div>";
	s += "</body></html>";
	//printStatusUbidots();
	server.send(200, "text/html; charset=UTF-8", s);
}
float obterValorUmidade(byte sensorNro){
	return millis();
}



#endif // _MEU_MAIN_H_