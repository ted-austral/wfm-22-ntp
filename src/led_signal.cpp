/*
 *  led_ signal.cpp
 *      To signal state of wifi connecion using only one led
 */

#include <Arduino.h>

#include "led_signal.h"

#define forever for(;;)

#define BASE 50
#define KEY_PULSE       250
#define FAIL_PULSE      100

/*
 *  Static functions
 */


static void
do_pulse_blink(int led, int pulse )
{
    digitalWrite(led,HIGH);
    delay(pulse);
    digitalWrite(led,LOW);
    delay(pulse);
}

static void
blink_forever( int led )
{
    forever
        do_pulse_blink(led, FAIL_PULSE);
}

static int
led_pulse_key( int led, int level, int ms )
{
    digitalWrite(led,level);
    delay(ms);
    if( digitalRead(RESET_WIFI) == LOW )
    {
        digitalWrite(led,LOW);
        return 1;
    }
    return 0;
}

/*
 *  Public functions
 */


void
led_signal_init(void)
{
    pinMode(RESET_WIFI, INPUT_PULLUP);
    pinMode(LED_RED,OUTPUT);
}

void
led_signal( const char *msg, int reason )
{
    Serial.println(msg);
    switch(reason)
    {
        case CLR_ALL_SIGNALS:
            digitalWrite(LED_RED,LOW);
            break;
        case IN_AUTOCONNECT:
            digitalWrite(LED_RED,HIGH);
            break;
        case WIFI_FATAL:
            blink_forever(LED_RED);
            ESP.restart();
            break;
        default:
            ESP.restart();
            break;
    }
}

int
get_reset_switch(void)
{
    int i;

    Serial.println("Time slot to push reset config switch....\n");
    for( i = 0; i < TIME_KEY/(2*KEY_PULSE); ++i )
        if( led_pulse_key(LED_RED,HIGH,KEY_PULSE) || led_pulse_key(LED_RED,LOW,KEY_PULSE) )
        {
            Serial.println("Clear APs stored...Going to AutoConnect\n");
            return 1;
        }
    return 0;
}

