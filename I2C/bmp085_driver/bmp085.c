/*  
   bmp085.c - User space bmp085 sensor header data

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
*/

#include <linux/types.h>
#include <linux/init.h>


/*I2C digital pressure sensor device address*/
#define BMP085_ADDR  0x77
#define BMP085_CONTROL_REG 0xF4
#define BMP085_TEMP_VALUE 0x2E
/*TODO*/
#define BMP085_OSS 2 /*Oversampling settings*/
#define FILE_PATH "/dev/i2c-1"



struct __attribute__((__packed__)) bmp085_parameter{
 s16 ac1;
 s16 ac2;
 s16 ac3;
 u16 ac4;
 u16 ac5;
 u16 ac6;
 s16 b1;
 s16 b2;
 s16 mb;
 s16 mc;
 s16 md;
 s16 b5;
};

/* TODO
static struct control_reg{
 unsigned short int usCtrRegValue;
 unsigned char maxConvTime;
};
*/

static struct control_reg ctrlReg[] = {
 {0x34,0x5},
 {0x74,0x8},
 {0xB4,0xE},
 {0xF4,0x1A},
};

static struct bmp085_data{
 struct bmp085_parameter ccParameter;
 u16 temperature,ut;
 u16 pressure,up;
 u16 ctr_reg;
 u16 max_conv_time;

 };



static struct bmp085_data bmp085Data;


/*smpbus functions*/
__s32 bmp085_i2c_read_long(__u8 addr)
{
 __s32 reg = i2c_smbus_read_word_data(bmp085Data.fd, addr);
 if(reg < 0)
 { 
  printf("smbus read word failed: %x", addr);
  exit(1);//return -1;
 }
 //printf("reg %x\n",reg);
 //printf("**********reg shift %x, %x\n",((reg << 8) & 0xff00),((reg >> 8) & 0xff )); 
 reg = (((reg << 8) & 0xff00) | ((reg >> 8) & 0xff )) ;
 //printf("reg shift %x, %x\n",reg); 
 return reg;
} 

/*Calibration of Sensor device*/
void bmp085Calibration()
{
  
  bmp085Data.ccParameter.ac1 = bmp085_i2c_read_long(0xAA);
  bmp085Data.ccParameter.ac2 = bmp085_i2c_read_long(0xAC);
  bmp085Data.ccParameter.ac3 = bmp085_i2c_read_long(0xAE);
  bmp085Data.ccParameter.ac4 = bmp085_i2c_read_long(0xB0);
  bmp085Data.ccParameter.ac5 = bmp085_i2c_read_long(0xB2);
  bmp085Data.ccParameter.ac6 = bmp085_i2c_read_long(0xB4);
  bmp085Data.ccParameter.b1 = bmp085_i2c_read_long(0xB6);
  bmp085Data.ccParameter.b2 = bmp085_i2c_read_long(0xB8);
  bmp085Data.ccParameter.mb = bmp085_i2c_read_long(0xBA);
  bmp085Data.ccParameter.mc = bmp085_i2c_read_long(0xBC);
  bmp085Data.ccParameter.md = bmp085_i2c_read_long(0xBE);

}

/*Tempature Calculation */
void bmp085_RealTemperature()
{
 int x1, x2;

 /*write temp value to control reg*/
 i2c_smbus_write_word_data(bmp085Data.fd, BMP085_CONTROL_REG, BMP085_TEMP_VALUE);
 /*Wait for conversion*/
 usleep(5000);
 bmp085Data.ut=bmp085_i2c_read_long(0xF6);

 x1 = (((int)bmp085Data.ut - (int) bmp085Data.ccParameter.ac6) *(int) bmp085Data.ccParameter.ac5) >> 15;
 x2 = (((int)bmp085Data.ccParameter.mc << 11)/(x1+bmp085Data.ccParameter.md));
 bmp085Data.ccParameter.b5 = x1+x2;
 bmp085Data.temperature = (unsigned int)(bmp085Data.ccParameter.b5+8) >> 4;  
 printf("Temperature\t%0.1f%cC\n", ((double)bmp085Data.temperature)/10,0x00B0);
}


/*Presure Calculation*/
void bmp085_RealPressure()
{
  int b3,b6,x1,x2,x3,p;
  unsigned int b4,b7;
  __u8 buf[3];

  i2c_smbus_write_word_data(bmp085Data.fd, BMP085_CONTROL_REG,ctrlReg[BMP085_OSS].usCtrRegValue+(BMP085_OSS << 6));
  usleep(ctrlReg[BMP085_OSS].maxConvTime*1000);
  
  //bmp085Data.up = (bmp085_i2c_read_long(0xF6)+bmp085_i2c_read_long(0xF8)) >> (8-BMP085_OSS);
  if (i2c_smbus_read_i2c_block_data(bmp085Data.fd, 0xF6, 3, buf) < 0)
    printf("Block data failed %x\n", 0xF6);
  
  bmp085Data.up =(((unsigned int)buf[0] << 16) + ((unsigned int)buf[1] << 8) + (unsigned int)buf[2]) >> (8-BMP085_OSS);
  
  
  b6 = bmp085Data.ccParameter.b5 - 4000;
  x1 = (bmp085Data.ccParameter.b2*((b6*b6)>>12)) >> 11;
  x2 = (bmp085Data.ccParameter.ac2*b6) >> 11;
  x3 = x1 + x2;
  b3 =(((bmp085Data.ccParameter.ac1*4+x3) << BMP085_OSS) + 2) >> 2; 
  x1 = (bmp085Data.ccParameter.ac3*b6) >> 13;
  x2 = (bmp085Data.ccParameter.b1*((b6*b6)>>12))>>16;
  x3 = ((x1+x2)+2)>>2;
  b4 = (bmp085Data.ccParameter.ac4*(unsigned long)(x3+32768))>>15;
  b7 = ((unsigned long)bmp085Data.up-b3)*(50000 >> BMP085_OSS);
  if(b7 < 0x80000000)
   p = (b7<<1)/b4;
  else
   p = (b7/b4)<<1;
  x1 = (p >> 8)*(p >> 8);
  x1 = (x1*3038) >> 16;
  x2 = (-7357*p) >> 16;
  bmp085Data.pressure = p = p+((x1+x2+3791) >> 4);
  
  printf("Pressure\t%0.2fhPa\n", ((double) bmp085Data.pressure)/100);
  printf("Altitude %0.2fm\n", (double)(((bmp085Data.pressure)/100)*8.43)); 
}

int __devinit bmp085_probe(struct device *dev, struct bmp085_data_bus *data_bus){


}

int __devexit bmp085_remove(struct device *dev){


}

MODULE_AUTHOR("mallesh.koujalagi@gmail.com");
MODULE_DISCRIPTION("BMP085 driver");
MODULE_LICENSE("GPL");
