#ifndef _Constantes_H_
#define _Constantes_H_

#ifndef IOTWEBCONF_CONFIG_USE_MDNS
	#define IOTWEBCONF_CONFIG_USE_MDNS 1
#endif

const bool EhDebug = true;
const bool EhParaEnviarAoServidor = true;
// const bool VersaoDEMO = false;
const bool FormatHD = false;
const bool ResetWIFI = false;
const unsigned int TimeDelay = 5000;
size_t sizeDocument = 100;

void debugMsg(const char* msg){
	if (EhDebug ){
		Serial.println(msg);
	}
}

String formatarBool(bool valor){	
	if ( valor == true){
		return "SIM";
	} 
	return "NÃO";
}
String formatarTempo(int valor){
	int tempo = valor;
	unsigned int hora = tempo / 3600;	
	tempo = tempo - hora * 3600;
	unsigned int min = tempo / 60;
	tempo = tempo - min * 60;
	
	String s = "";
	s += hora;
	s += " hr ";
	s += min;
	s += " min ";
	s += tempo;
	s += " seg";
	return s;
}

//========================
String criarCard(String titulo, String valor)
  {
	String s = "<div class='card row'><h3 class='lg'>";
	s += titulo;
	s += "</h3><p>";
	s += valor;
	s += "</p><hr></div>";
	return s;
  }

  String getMenu(){
	String s = "<a href='/'>Home</a> | <a href='config'>Configuração</a> | <a href='ubidots'>Enviar dados</a> ";
	s += "| <a href='time'>Atualizar Relógio</a>";
	return s;
}

bool between(int value, int min, int max){	
	if (value < min && value > max)
	{
		return false;
	}
	return true;
}

#endif // _Constantes_H_


