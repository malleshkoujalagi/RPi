#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/slab.h>
#include "bmp085.h"


static int bmp085_i2c_read_byte(void *client, u8 reg){
   return i2c_smbus_read_byte_data(client, reg);
}

static int bmp085_i2c_write_byte(void *client, u8 reg, u8 value){

   return i2c_smbus_write_byte_data(client, reg, value);
}

static int bmp085_i2c_read_block(void *client, u8 reg, int len, char *buf){

   return i2c_smbus_read_i2c_block_data(client,reg, len, buf);
}

static const struct bmp085_bus_ops bmp085_i2c_bus_ops={
  .read_byte = bmp085_i2c_read_byte,
  .wirte_byre = bmp085_i2c_write_byte,
  .read_block = bmp085_i2c_read_block,
 };



static int __devinit bmp085_i2c_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
  struct bmp085_data_bus data_bus = {
       .bmp085_bus_ops = & bmp085_i2c_bus_ops;
       .client = client;
  }

 return bmp085_probe(&client->dev, &data_bus);
 
}


static int __devexit bmp085_i2c_remove(struct i2c_client *client)
{

  return bmp085_remove(&client->dev);
}


static void bmp085_i2c_shutdown(struct i2c_client *client)
{
   bmp085_disable(&client->dev);
}

#ifdef CONFIG_PM

static int bmp085_i2c_suspend(struct i2c_client *client, pm_message_t msg)
{
 return 0;

}

static int bmp085_i2c_resume(struct i2c_client *client)
{

 return 0;
}

static const struct dev_pm_ops bmp085_i2c_pm = {
 .suspend = bmp085_i2c_suspend,
 .resume = bmp085_i2c_resume,
};

#endif



static struct i2c_device_id bmp085_id_table [] = {
 
    {BMP085_NAME, 0},    
    {} 
};

MODULE_DEVICE_TABLE(i2c, bmp085_id_table);
static struct i2c_driver bmp085_i2c_driver = {
   .probe = bmp085_i2c_probe,
   .remove = __devexit_p(bmp085_i2c_remove),
   .shutdown = bmp085_i2c_shutdown,
   .id = bmp085_id_table,

   .driver = {
       .owner = THIS_MODULE,
       .name = BMP085_NAME,
#ifdef CONFIG_PM
       .pm = &bmp085_i2c_pm,
#endif    
   }, 

};


static int __init bmp085_i2c_init(void)
{
 
  printk(KERN_ALERT "I2C sample code\n");
  i2c_add_driver(&bmp085_i2c_driver);
  return 0;
}



static void __exit bmp085_i2c_exit(void)
{

  printk(KERN_ALERT "I2C sample exit\n");
  i2c_del_driver(&bmp085_i2c_driver);
}



module_init(bmp085_i2c_init);
module_exit(bmp085_i2c_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("I2C Sample driver for bmp085 digital pressure sesonr");
MODULE_AUTHOR("mallesh.koujalagi@gmail.com");

