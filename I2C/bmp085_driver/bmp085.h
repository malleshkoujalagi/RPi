
#ifndef __BMP085_H
#define __BMP085_H

#define BMP085_NAME "bmp085"

struct bmp085_platform_data {
  u8 chip_id;
  u8 over_sampling;
  u32 temp_period;
  (*init_hw) (void );
  (*deinit_hw)(void);  

};

struct bmp085_bus_ops {
int (*read_word)(void *client, u8 reg);
int (*write_word)(void *client, u8 reg, u8 value);
int (*read_block)(void *clinet, u8 reg, int len, char *buf); 
};

struct bmp085_data_bus{
 const stuct bmp085_bus_ops *ops;
 void *client;
}

int bmp085_probe(struct device *dev, struct bmp085_data_bus *data_bus)
int bmp085_remove(stuct device *dev);

#ifdef CONFIG_PM
int bmp085_enable(struct device *dev);
int bmp085_disable(struct device *dev);
#endif

#endif 
