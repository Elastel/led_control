#include <linux/types.h>
#include <linux/rtc.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <string.h>
#include "i2c.h"

#define TIMEOUT	3
#define RETRY	3
#define I2C_DEV "/dev/i2c-1"

static int fd;

int i2c_read_data(u16 addr, u8 offset, u8 *val)
{
	int i,ret = 0;

	struct i2c_rdwr_ioctl_data *data;

	if ((data = (struct i2c_rdwr_ioctl_data *)malloc(sizeof(struct i2c_rdwr_ioctl_data))) == NULL)
	return -1;

	data->nmsgs = 2;
	if ((data->msgs = (struct i2c_msg *)malloc(data->nmsgs * sizeof(struct i2c_msg))) == NULL) {
		ret = -1;
		goto errexit3;
	}
	if ((data->msgs[0].buf = (unsigned char *)malloc(sizeof(unsigned char))) == NULL) {
		ret = -1;
		goto errexit2;
	}
	if ((data->msgs[1].buf = (unsigned char *)malloc(sizeof(unsigned char))) == NULL) {
		ret = -1;
		goto errexit1;
	}

	data->msgs[0].addr = addr;
	data->msgs[0].flags = 0;
	data->msgs[0].len = 1;
	data->msgs[0].buf[0] = offset;

	data->msgs[1].addr = addr;
	data->msgs[1].flags = I2C_M_RD;
	data->msgs[1].len = 1;			//original data is 1
	data->msgs[1].buf[0] = 0;

	if ((ret = __i2c_send(fd, data)) < 0)
		goto errexit0;

	for(i = 0 ;i < data->msgs[1].len; i++)
		val[i] = data->msgs[1].buf[i];

errexit0:
	free(data->msgs[1].buf);
errexit1:
	free(data->msgs[0].buf);
errexit2:
	free(data->msgs);
errexit3:
	free(data);

	return ret;
}

int i2c_write_data(u16 addr, u8 offset, u8 val)
{
	int ret = 0;

	struct i2c_rdwr_ioctl_data *data;

	if ((data = (struct i2c_rdwr_ioctl_data *)malloc(sizeof(struct i2c_rdwr_ioctl_data))) == NULL)
		return -1;

	data->nmsgs = 1;
	if ((data->msgs = (struct i2c_msg *)malloc(data->nmsgs * sizeof(struct i2c_msg))) == NULL) {
		ret = -1;
		goto errexit2;
	}
	if ((data->msgs[0].buf = (unsigned char *)malloc(2 * sizeof(unsigned char))) == NULL) {
		ret = -1;
		goto errexit1;
	}

	data->msgs[0].addr = addr;
	data->msgs[0].flags = 0;
	data->msgs[0].len = 2;
	data->msgs[0].buf[0] = offset;
	data->msgs[0].buf[1] = val;

	if ((ret = __i2c_send(fd, data)) < 0)
		goto errexit0;

errexit0:
	free(data->msgs[0].buf);
errexit1:
	free(data->msgs);
errexit2:
	free(data);

	return ret;
}

int i2c_open(unsigned char* dev)
{
	if ((fd = open(dev, O_RDWR)) < 0) {
		printf("i2c_open error!\n");
		return fd;
	}
	
	__i2c_set(fd, TIMEOUT, RETRY);

	return fd;
}

static int __i2c_send(int fd, struct i2c_rdwr_ioctl_data *data)
{
	if (fd < 0)
		return -1;

	if (data == NULL)
		return -1;

	if (data->msgs == NULL || data->nmsgs == 0)
		return -1;
	
	return ioctl(fd, I2C_RDWR, (unsigned long)data) ;
}

static int __i2c_set(int fd, unsigned int timeout, unsigned int retry)
{
	if (fd == 0 )
		return -1;

	ioctl(fd, I2C_TIMEOUT, timeout ? timeout : I2C_DEFAULT_TIMEOUT);

	ioctl(fd, I2C_RETRIES, retry ? retry : I2C_DEFAULT_RETRY);
	
	return 0;
}

void i2c_close(int fd)
{
	if (fd < 0)
		return;

	close(fd);
}

unsigned bcd2bin(unsigned char val)
{       
	return (val & 0x0f) + (val >> 4) * 10;  
}

unsigned char bin2bcd(unsigned val)
{       
	return ((val / 10) << 4) + val % 10;
}

int read_i2c_data(u16 addr, u8 offset, u8 *val)
{
	int ret = 0;
	int n_fd = 0;
	
	n_fd = i2c_open(I2C_DEV);

	if (n_fd > 0) {
		ret = i2c_read_data(addr, offset, val);
		if(ret < 0){
			printf("%s error!\n",__FUNCTION__);
		}
		// printf("read success, val  = %02x\n", *val);

		i2c_close(fd);
	}
	
	return ret;
}

int write_i2c_data(u16 addr, u8 offset, u8 val)
{
		
	int ret = 0;
	int n_fd = 0;
	
	n_fd = i2c_open(I2C_DEV);

	if (n_fd > 0) {
		ret = i2c_write_data(addr, offset, val);
		if(ret < 0){
			printf("%s error!\n",__FUNCTION__);
		}

		i2c_close(fd);
	}
	
	return ret;
}
