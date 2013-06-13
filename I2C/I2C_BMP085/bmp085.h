/*
    bmp085.h - User space bmp085 sensor header data 

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

*/

#ifndef LIB_I2C_BMP085_H
#define LIB_I2C_BMP085_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <unistd.h>
#include "smbus.h"



/*I2C digital pressure sensor device address*/
#define BMP085_ADDR  0x77
#define BMP085_CONTROL_REG 0xF4
#define BMP085_TEMP_VALUE 0x2E
#define BMP085_OSS 2 /*Oversampling settings*/
#define FILE_PATH "/dev/i2c-1"



struct __attribute__((__packed__)) bmp085_parameter{
 short int ac1;
 short int ac2;
 short int ac3;
 unsigned short int ac4;
 unsigned short int ac5;
 unsigned short int ac6;
 short int b1;
 short int b2;
 short int mb;
 short int mc;
 short int md;
 short int b5;
};


struct control_reg{
 unsigned short int usCtrRegValue;
 unsigned char maxConvTime;
};


static struct control_reg ctrlReg[] = {
 {0x34,0x5},
 {0x74,0x8},
 {0xB4,0xE},
 {0xF4,0x1A},
};

struct bmp085_data{
 struct bmp085_parameter ccParameter;
 unsigned int temperature,ut;
 unsigned int pressure,up;
 int fd;
 };

#endif //LIB_I2C_BMP085_H
