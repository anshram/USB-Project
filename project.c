#include<linux/module.h>
#include<linux/init.h>
#include<linux/kernel.h>
#include<linux/types.h>
#include<linux/cdev.h>
#include<linux/fs.h>
#include<linux/device.h>
#include<linux/errno.h>
static dev_t devId;
static int ret;
static struct class *USBDevCls;
static struct device *retDev;
static struct cdev USBDev; 

int USB_open (struct inode *node , struct file * filp)
{
	printk(KERN_ALERT"In open\n");
	return 0;
}
int USB_release (struct inode *node, struct file *filp)
{
	printk(KERN_ALERT"In release\n");
	return 0;
}
ssize_t USB_read(struct file *filp, char __user *usrData, size_t len, loff_t *loff)
{
	printk(KERN_ALERT"In read\n");
	return 0;

}
ssize_t USB_write(struct file *filp, const char __user *usrData, size_t len, loff_t *loff)
{
	printk(KERN_ALERT"In write\n");
	return 0;

}


static struct file_operations USB_ops = 
{
	.open =USB_open,
	.release = USB_release,
	.read = USB_read,
	.write = USB_write
};



static int __init project_init(void)
{
	ret = alloc_chrdev_region(&devId,0,1,"USBDev");
	if(ret < 0)
	{
		return -ret;
	}

	cdev_init(&USBDev,&USB_ops);

	ret = cdev_add(&USBDev,devId,1);

	USBDevCls = class_create(THIS_MODULE,"USBDev");
	if(NULL == USBDevCls)
	{
		cdev_del(&USBDev);
		unregister_chrdev_region(devId,1);
		return -ENOMEM;
	}
	retDev = device_create(USBDevCls,NULL,devId,NULL,"USBDev");
	if(NULL == retDev)
	{
		cdev_del(&USBDev);
		unregister_chrdev_region(devId,1);
		class_destroy(USBDevCls);
		return -1;
	}
	
	printk(KERN_ALERT"Hello World\n");

	return 0;
}

static void __exit  project_exit(void)
{
	
	cdev_del(&USBDev);
	device_destroy(USBDevCls, devId);
	class_destroy(USBDevCls);
	unregister_chrdev_region(devId,1);


	printk(KERN_ALERT"Bye World\n");
}

module_init(project_init);
module_exit(project_exit);
MODULE_LICENSE("GPL");
