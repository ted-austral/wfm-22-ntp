/*
 *  wifi_mgr.cpp
 *      Wrapper for wifi_connect around WiFiManager library
 *      Austral - IntroComp - 2022 - EAM
 */

#include <Arduino.h>

#include <WiFiManager.h>

#include "wifi_mgr.h" 
#include "led_signal.h"

void
wifi_connect(void)
{
    WiFiManager wm;

    pinMode(RESET_WIFI, INPUT_PULLDOWN);
    led_signal_init();
    WiFi.mode(WIFI_STA);

    if( get_reset_switch() )
        wm.resetSettings();
    led_signal("Trying to connect through AutoConnectAP",IN_AUTOCONNECT);
    if( !wm.autoConnect("AutoConnectAP") )
        led_signal("Failed to connect",WIFI_FATAL);
    else
        led_signal("WiFi connected....",CLR_ALL_SIGNALS);
}


