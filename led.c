#include "led.h"
#include "i2c.h"

void i2c_init(void)
{
	u8 o_val;
	u8 n_val;

	read_i2c_data(SLAVE_ADDR, PCA9554_REG_OUT, &o_val);
	if (o_val != 0) {
		return write_i2c_data(SLAVE_ADDR, PCA9554_REG_OUT, 0);	
	}
}

int control_led_status(int status, u8 val)
{
	u8 o_val;
	u8 n_val;
	int ret = 0;
	
	i2c_init();
	
	ret = read_i2c_data(SLAVE_ADDR, PCA9554_REG_CTRL, &o_val);

	if (ret < 0) {
		return ret;
	}
	
	if (status == LED_ON) {
		n_val = o_val & val;
	} else if (status == LED_OFF) {
		n_val = o_val | val;
	}

	return write_i2c_data(SLAVE_ADDR, PCA9554_REG_CTRL, n_val);	
}

int set_led_power_on(void)
{
	return control_led_status(LED_ON, LED_POWER_ON);
}

int set_led_power_off(void)
{
	return control_led_status(LED_OFF, LED_POWER_OFF);
}

int set_led_sys_on(void)
{
	return control_led_status(LED_ON, LED_SYS_ON);
}

int set_led_sys_off(void)
{
	return control_led_status(LED_OFF, LED_SYS_OFF);
}

int set_led_wifi_on(void)
{
	return control_led_status(LED_ON, LED_WIFI_ON);
}

int set_led_wifi_off(void)
{
	return control_led_status(LED_OFF, LED_WIFI_OFF);
}

int set_led_alarm_on(void)
{
	return control_led_status(LED_ON, LED_ALARM_ON);
}

int set_led_alarm_off(void)
{
	return control_led_status(LED_OFF, LED_ALARM_OFF);
}

int set_led_online_on(void)
{
	return control_led_status(LED_ON, LED_ONLINE_ON);
}

int set_led_online_off(void)
{
	return control_led_status(LED_OFF, LED_ONLINE_OFF);
}

int set_led_LTE(int signal)
{
	u8 o_val;
	u8 n_val;
	u8 val;
	int ret = 0;
	
	ret = read_i2c_data(SLAVE_ADDR, PCA9554_REG_CTRL, &o_val);
	if (ret < 0) {
		return ret;
	}
	
	val = o_val | LED_SIG_NONE;
	ret = write_i2c_data(SLAVE_ADDR, PCA9554_REG_CTRL, val);
	if (ret < 0) {
		return ret;
	}
	
	switch (signal) {
		case SIG_NONE:
			printf("no signal\n");
			return 1;
			break;
		case SIG_LOW:
			n_val = val & LED_SIG_LOW;
			break;
		case SIG_MID:
			n_val = val & LED_SIG_MID;
			break;
		case SIG_HIGH:
			n_val = val & LED_SIG_HIGH;
			break;
	}

	return write_i2c_data(SLAVE_ADDR, PCA9554_REG_CTRL, n_val);	
}

