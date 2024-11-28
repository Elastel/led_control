#ifndef LED_H
#define LED_H

#define SLAVE_ADDR 0x20
#define PCA9554_REG_INP                 0x00
#define PCA9554_REG_OUT                 0x01
#define PCA9554_REG_POL                 0x02
#define PCA9554_REG_CTRL                0x03

#define LED_POWER_ON 0xbf		// &
#define LED_POWER_OFF 0x40		// |
#define LED_SIG_NONE 0x15		// no network | CSQ = 99
#define LED_SIG_LOW  0xef		// < -105dBm | CSQ < 11
#define LED_SIG_MID 0xeb		// < -85dBm, > -105dBm | 11 < CSQ < 18
#define LED_SIG_HIGH 0xea		// > -85dBm | 18 < CSQ < 31
#define LED_ONLINE_ON 0x7f
#define LED_ONLINE_OFF 0x80
#define LED_SYS_ON 0xdf
#define LED_SYS_OFF 0x20
#define LED_WIFI_ON 0xf7
#define LED_WIFI_OFF 0x08
#define LED_ALARM_ON 0xfd
#define LED_ALARM_OFF 0x02

/***************
0x2a

  	—— —— >
  ^	0 0  0 0
  |
  |	0 1  1 1

0010 1010
***************/

enum {
	LED_ON,
	LED_OFF,
	LED_BLINK_FAST,	  /* 0.1 second on, 0.1 second off */
	LED_BLINK_SLOW,   /* 1 second on, 1 second off */
	LED_BLINK_NORMAL, /* 0.5 second on, 0.5 second off */
	LED_BLINK_STOP,	  /* close led */
};

enum {
	POWER_MODE,
	SYS_MODE,
	WIFI_MODE,
	ALARM_MODE,
	ONLINE_MODE,
	SIG_MODE,
};

enum {
	SIG_NONE,
	SIG_LOW,
	SIG_MID,
	SIG_HIGH,
};

int set_led_power_on(void);
int set_led_sys_on(void);
int set_led_wifi_on(void);
int set_led_alarm_on(void);
int set_led_online_on(void);
int set_led_power_off(void);
int set_led_sys_off(void);
int set_led_wifi_off(void);
int set_led_alarm_off(void);
int set_led_online_off(void);
int set_led_LTE(int signal);


#endif

