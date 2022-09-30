/*
 *  led_ signal.h
 *      To signal state of wifi connecion using only one led
 */

enum
{
    CLR_ALL_SIGNALS, IN_AUTOCONNECT, WIFI_FATAL
};

void led_signal_init(void);

void led_signal( const char *msg, int status);

int get_reset_switch(void);

