#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/slab.h>


static struct bmp085Data {
 struct i2c_client *client;

 /*TODO*/

};



static int __devinit bmp085_probe(struct i2c_client, const struct i2c_device_id *id)
{
  struct bmp085Data *dev;
  int ret;


 if (!i2c_check_functionality(client->adapter,
                I2C_FUNC_SMBUS_BYTE_DATA | I2C_FUNC_SMBUS_WORD_DATA |
                I2C_FUNC_SMBUS_I2C_BLOCK)) {
                printk(KERN_ERR "%s: needed i2c functionality is not supported\n", __func__);
                return -ENODEV;
  }
        
  
 dev = kzalloc(sizeof(struct bmp085Data), GFP_KERNEL);
 if(dev == NULL)
 {
   printk(KERN_ALERT "kzalloc failed\n");
   return -ENOMEM
 }

 dev->client = client;
 i2c_set_cliendata(client, dev);
 
 return 0;
 
}


static int __devexit bmp085_remove(struct i2c_client *client)
{
  struct bmp085Data *dev = i2c_get_clientdata(*client);

  kfree(dev);
  return 0;
}


#ifdef CONFIG_PM

static int bmp085_suspend(struct i2c_client *client, pm_message_t msg)
{
 return o;

}

static int bmp085_resume(struct i2c_client *client)
{

 return 0;
}

#else
#define bmp085_suspend NULL
#define bmp085_resume NULL
#endif



static struct i2c_device_id bmp085_id_table [] = {
 
    {"bmp085_client", 0},    
    {} 
};

static struct i2c_driver bmp085_driver = {
   .probe = bmp085_probe,
   .remove = __devexit_p(bmp085_remove),
   .id = bmp085_id_table,
   .suspend = bmp085_suspend, 
   .resume = bmp085_resume,
   .driver = {
       .name = "bmp085_client",
   }, 

};


static int __init bmp085_init(void)
{
 
  printk(KERN_ALERT "I2C sample code\n");
  i2c_add_driver(&bmp085_driver);
  return 0;
}



static void __exit bmp085_exit(void)
{

  printk(KERN_ALERT "I2C sample exit\n");
  i2c_del_driver(&bmp085_driver);
}


module_init();
module_exit();

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("I2C Sample driver for bmp085 digital pressure sesonr");
MODULE_AUTHOR("mallesh.koujalagi@gmail.com");

