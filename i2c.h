#ifndef I2C_H
#define I2C_H

#include <linux/i2c.h>
#include <linux/i2c-dev.h>

#define I2C_DEFAULT_TIMEOUT		1
#define I2C_DEFAULT_RETRY		3

typedef unsigned char           u8;
typedef unsigned short          u16;
typedef unsigned int            u32;
typedef unsigned long long      u64;
typedef signed char             s8;
typedef short                   s16;            
typedef int                     s32;
typedef long long               s64;

unsigned bcd2bin(unsigned char val);

unsigned bcd2bin(unsigned char val);

static int __i2c_send(int fd, struct i2c_rdwr_ioctl_data *data);

static int __i2c_set(int fd, unsigned int timeout, unsigned int retry);

int i2c_read_data(u16 addr, u8 offset, u8 *val);

int i2c_write_data(u16 addr, u8 offset, u8 val);

int i2c_open(unsigned char* dev);

int read_i2c_data(u16 addr, u8 offset, u8 *val);

int write_i2c_data(u16 addr, u8 offset, u8 val);


#endif
