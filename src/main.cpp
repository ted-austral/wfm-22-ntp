/*
 *  Project wfm-22-ntp - main.cpp
 *
 *      Same as 22-ntp but connection with WiFi is made using WiFiManager Librray
 *
 *      Austral - IntroComp - 2022 - EAM
 *
 *      Connects with time server and
 *      shows week day and complete time for
 *      the time zone defined
 * 
 *      Also, through Unix library <time.h> one can
 *      print date and time with desired format using
 *      call to function 'strftime' based in 'epoch time'
 *      deseado a través de 'strftime' basado en 'epoch time'
 *
 */

/*
 *  Definitions included  in platformio.ini
 * 
 *      TIME_ZONE   Time zone in hours
 *      NTP_SERVER  NTP server name
 *      SERIAL_BAUD Serial port Baud rate
 */

/*
 *  Includes
 */

#include <Arduino.h>

#include <NTPClient.h>          // for access to time servers
#include <WiFiUdp.h>            // for using UDP protocol
#include <time.h>               // to access to Unix library calls

#include "wifi_mgr.h"           // Wrapper of WiFiManager library

#define TZ(tz)      (tz*3600)               //  Macro to convert from time zone in hours to seconds

//  Week names

const char *daysOfTheWeek[] =
{
    "Domingo", "Lunes", "Martes", "Miercoles", "Jueves", "Viernes", "Sabado"
};

/*
 *  Creacion de objetos
 */

WiFiUDP ntpUDP;                                                 //  Object for using UDP
NTPClient timeClient(ntpUDP, NTP_SERVER, TZ(TIME_ZONE) );       //  Object NTPClient

void
setup(void)
{
    Serial.begin(SERIAL_BAUD);
    wifi_connect();             // Call to wrapper of WiFiManager library
    timeClient.begin();
}

void
loop(void)
{
    time_t epoch_time;
    char buf[80];

    timeClient.update();                    //  Get time and date from server

    Serial.print( daysOfTheWeek[timeClient.getDay()] );
    Serial.print( ": ");
    Serial.println(timeClient.getFormattedTime());

    Serial.print( "Epoch = " );
    epoch_time = timeClient.getEpochTime();
    Serial.println( epoch_time );
    strftime(buf, sizeof(buf),"%A: %d %B %Y %H:%M:%S", localtime(&epoch_time));
    Serial.println(buf); 

    Serial.println();

    delay(1000);
}
