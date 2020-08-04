#ifndef _MEU_TimeUtils_H_
#define _MEU_TimeUtils_H_

#include <TimeLib.h>
#include <WiFiUdp.h>

// NTP Servers:
static const char ntpServerName[] = "us.pool.ntp.org";
//static const char ntpServerName[] = "time.nist.gov";
//static const char ntpServerName[] = "time-a.timefreq.bldrdoc.gov";
//static const char ntpServerName[] = "time-b.timefreq.bldrdoc.gov";
//static const char ntpServerName[] = "time-c.timefreq.bldrdoc.gov";


char* formatarDataHora(time_t t){
	struct tm *lt = localtime(&t);

	char str[32];
	Serial.print("Hora Formatada: ");
	strftime(str, sizeof str, "%Y-%m-%d %H:%M:%S", lt); 
	Serial.println(str);
	return str;
}


void digitalClockDisplay()
{
	time_t now = time(nullptr);
 	struct tm timeinfo;
	gmtime_r(&now, &timeinfo);
	Serial.print(F("Current time: "));
	Serial.print(asctime(&timeinfo));
	
}

void printDigits(int digits)
{
  // utility for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if (digits < 10)
    Serial.print('0');
  Serial.print(digits);
}


bool sincronizarRelogio() {	
	bool _debug = 1;
	// Synchronizes time using SNTP. This is necessary to verify that
	// the TLS certificates offered by the server are currently valid.
	if (_debug) {
		Serial.print(F("Setting time using SNTP"));
	}
	configTime(-3 * 3600, 0, "pool.ntp.org"); //, "time.nist.gov"
	time_t now = time(nullptr);
	uint8_t attempts = 0;
	while (now < 8 * 3600 * 2 && attempts <= 5) {
		if (_debug) {
		Serial.print(".");
		}
		now = time(nullptr);
		attempts += 1;
		delay(500);
	}

	if (attempts > 5) {
		if (_debug) {
			Serial.println(F("[ERROR] Could not set time using remote SNTP to verify Cert"));
		}
		return false;
	}

	struct tm timeinfo;
	gmtime_r(&now, &timeinfo);
	if (_debug) {
		Serial.print(F("Current time: "));
		Serial.print(asctime(&timeinfo));
	}
	return true;
}

void setupDateTime()
{
	setSyncInterval(60*30);
	sincronizarRelogio();
}

#endif