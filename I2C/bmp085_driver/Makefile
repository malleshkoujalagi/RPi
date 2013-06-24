KERN_DIR=/home/sinseman44/WORK/raspberry_pi/buildroot-2012.05/output/build/linux-HEAD
obj-m += raspberrypi_test_led.o

all:
	$(MAKE) -C $(KERN_DIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KERN_DIR) M=$(PWD) clean
