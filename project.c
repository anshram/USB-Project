#include<linux/module.h>
#include<linux/init.h>
#include<linux/kernel.h>
#include<linux/types.h>
#include<linux/cdev.h>
#include<linux/fs.h>
#include<linux/device.h>
#include<linux/errno.h>
#include<linux/usb.h>

static dev_t devId;
static int ret;
static struct class *USBDevCls;
static struct device *retDev;
static struct cdev USBDev;

/*******------Driver File Operations-------*******/

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

/*****----Driver Probe And Disconnect------*******/

static struct usb_device_id USBDev_id[] = 
{
	{ USB_DEVICE(0x1cbe,0x0003) },
	{}
};

int USB_probe(struct usb_interface *intf,
	  const struct usb_device_id *id)
{
	printk(KERN_ALERT"In probe\n");
	ret = alloc_chrdev_region(&devId,0,1,"USBDev");
	if(ret < 0)
	{
		return -ret;
	}

	cdev_init(&USBDev,&USB_ops);

	ret = cdev_add(&USBDev,devId,1);
	if(ret < 0)
	{
		unregister_chrdev_region(devId,1);

	}

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

void USB_disconnect(struct usb_interface *intf)
{
	printk(KERN_ALERT"In disconnect\n");
	cdev_del(&USBDev);
	device_destroy(USBDevCls, devId);
	class_destroy(USBDevCls);
	unregister_chrdev_region(devId,1);

}

static struct usb_driver USBDevDriver = 
{
	.name = "Project USB Device Driver",
	.id_table = USBDev_id,
	.probe = USB_probe,
	.disconnect = USB_disconnect
};


/*****----Driver Init And Exit------*******/
static int __init project_init(void)
{
	ret = usb_register(&USBDevDriver);

	return 0;
}

static void __exit  project_exit(void)
{
	usb_deregister(&USBDevDriver);	


	printk(KERN_ALERT"Bye World\n");
}

module_init(project_init);
module_exit(project_exit);
MODULE_LICENSE("GPL");
