#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <syslog.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <gpio.h>
#include <util.h>
#include <linux/ioctl.h>
#include "led.h"


void print_usage()
{
	printf("led_ctl -m < system | wifi | alarm | online | sig >"

			" -s <on | off | stop | none | low | mid | high>\n");
	exit(0);
}

void print_usage_sig()
{
	printf("led_ctl -m <sig> -s <none | low | mid | high>\n");
	exit(0);
}

int get_led_mode(char *mode)
{
	int ret = -1;

	if (!strcmp(mode, "power"))
		ret = POWER_MODE;
	else if(!strcmp(mode, "system"))
		ret = SYS_MODE;
	else if(!strcmp(mode, "wifi"))
		ret = WIFI_MODE;
	else if(!strcmp(mode, "alarm"))
		ret = ALARM_MODE;
	else if(!strcmp(mode, "online"))
		ret = ONLINE_MODE;
	else if(!strcmp(mode, "sig"))
		ret = SIG_MODE;
	else
		ret = -1;

	return ret;
}


int get_led_action(char *action)
{
	int ret = -1;
	
	if(!strcmp(action, "on"))
		ret = LED_ON;
	else if (!strcmp(action, "off"))
		ret = LED_OFF;
	else if (!strcmp(action, "blink"))
		ret = LED_BLINK_FAST;
	else if (!strcmp(action, "stop"))
		ret = LED_BLINK_STOP;
	else if (!strcmp(action, "slow"))
		ret = LED_BLINK_SLOW;
	else if (!strcmp(action, "normal"))
		ret = LED_BLINK_NORMAL;
	else
		ret = -1;

	return ret;
}

int get_sig_action(char *action)
{
	int ret = -1;
	
	if(!strcmp(action, "low"))
		ret = SIG_LOW;
	else if (!strcmp(action, "mid"))
		ret = SIG_MID;
	else if (!strcmp(action, "high"))
		ret = SIG_HIGH;
	else if (!strcmp(action, "none"))
		ret = SIG_NONE;
	else
		ret = -1;

	return ret;
}


int flash_led(int mode, int action)
{
	char msg[16];
	int on, off;

	switch(action) {
	case LED_BLINK_FAST:
		on = 200;
		off = 200;
		break;
	case LED_BLINK_SLOW:
		on = 2000;
		off = 2000;
		break;
	case LED_BLINK_NORMAL:
		on = 500;
		off = 500;
		break;
	default:
		on = 500;
		off = 500;
		break;
	}
	
	memset(msg, 0, 16);
	sprintf(msg, "%d-%d-%d-%d", mode, action, on, off);
	
	return 0;
	// return send_msg_to_named_pipe("/tmp/system.pipe", msg);
}

int set_led_status(int mode, int action)
{
	int ret;
	
	if (action == LED_ON) {
		flash_led(mode, LED_BLINK_STOP);
		switch (mode) {
			case POWER_MODE:
				set_led_power_on();
				break;
			case SYS_MODE:
				set_led_sys_on();
				break;
			case WIFI_MODE:
				set_led_wifi_on();
				break;
			case ALARM_MODE:
				set_led_alarm_on();
				break;
			case ONLINE_MODE:
				set_led_online_on();
				break;
		}
	} else if (action == LED_OFF) {
		flash_led(mode, LED_BLINK_STOP);
		switch (mode) {
			case POWER_MODE:
				set_led_power_off();
				break;
			case SYS_MODE:
				set_led_sys_off();
				break;
			case WIFI_MODE:
				set_led_wifi_off();
				break;
			case ALARM_MODE:
				set_led_alarm_off();
				break;
			case ONLINE_MODE:
				set_led_online_off();
				break;
		}
	} else {
		ret = flash_led(mode, action);
	}
	
	return ret;
}

int main(int argc, char **argv)
{
	int ch;
	char str_mode[12], str_pin[12], str_action[12];
	int mode, pin, action, sig_action;

	mode = -1;
	pin = -1;
	action = -1;

	if (argc != 5) {
		print_usage();
	}

	while((ch = getopt(argc, argv, "m:s:")) != -1) {
		switch(ch) {
		case 'm':
			strcpy(str_mode, optarg);
			break;
		case 's':
			strcpy(str_action, optarg);
			break;
		default:
			print_usage();
			break;
		}
	}
	
    mode = get_led_mode(str_mode);
    if (mode == -1) {
        print_usage();
        return 1;
    }

	if (mode != SIG_MODE) {
	    action = get_led_action(str_action);
	    if (action == -1) {
	        print_usage();
	        return 1;
	    }

	    set_led_status(mode, action);
	} else {
		sig_action = get_sig_action(str_action);
		if (sig_action == -1) {
	        print_usage_sig();
	        return 1;
	    }

		set_led_LTE(sig_action); 
	}

    return 0;
}
