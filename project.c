#include<linux/module.h>
#include<linux/init.h>
#include<linux/kernel.h>
#include<linux/types.h>
#include<linux/cdev.h>
#include<linux/fs.h>
#include<linux/device.h>
#include<linux/errno.h>
#include<linux/usb.h>
#include<linux/slab.h>
#include<linux/uaccess.h>
static int ret;
static struct device *retDev;
typedef struct USBDevice 
{
	struct cdev USBCDev;
	struct usb_device *USBDev;
	uint8_t buffer[64];
	struct urb USB_in_urb;
	struct urb USB_out_urb;
	dev_t devId;
	struct class *USBDevCls;

}USBDevice;
/*******------Driver File Operations-------*******/

int USB_open (struct inode *indp , struct file * filp)
{
	USBDevice *dev = container_of(indp->i_cdev,USBDevice,USBCDev);
	filp->private_data = dev;
	printk(KERN_ALERT"In open\n");
	return 0;
}

int USB_release (struct inode *indp, struct file *filp)
{
	printk(KERN_ALERT"In release\n");
	return 0;
}

ssize_t USB_read(struct file *filp, char __user *usrData, size_t len, loff_t *loff)
{
	printk(KERN_ALERT"In read\n");
	return 	copy_to_user(usrData,"hello&Hi\n",9);

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
	USBDevice *dev;
	struct usb_device *udev;
	printk(KERN_ALERT"In probe\n");
	dev = kzalloc(sizeof(USBDevice),GFP_KERNEL);
	if(NULL == dev)
		return -ENOMEM;
	udev = interface_to_usbdev(intf);
	dev->USBDev = usb_get_dev(udev);
	
	usb_set_intfdata(intf,dev);

	ret = alloc_chrdev_region(&dev->devId,0,1,"USBDev");
	if(ret < 0)
	{
		kfree(dev);
		return -ret;
	}


	cdev_init(&dev->USBCDev,&USB_ops);

	ret = cdev_add(&dev->USBCDev,dev->devId,1);
	if(ret < 0)
	{
		kfree(dev);
		unregister_chrdev_region(dev->devId,1);

	}

	dev->USBDevCls = class_create(THIS_MODULE,"USBDev");
	if(NULL == dev->USBDevCls)
	{
		cdev_del(&dev->USBCDev);
		unregister_chrdev_region(dev->devId,1);
		kfree(dev);
		return -ENOMEM;
	}
	retDev = device_create(dev->USBDevCls,NULL,dev->devId,NULL,"USBDev%d",MAJOR(dev->devId));
	if(NULL == retDev)
	{
		cdev_del(&dev->USBCDev);
		unregister_chrdev_region(dev->devId,1);
		class_destroy(dev->USBDevCls);
		kfree(dev);
		return -1;
	}
	
	printk(KERN_ALERT"Hello World\n");
	return 0;

}

void USB_disconnect(struct usb_interface *intf)
{
	USBDevice *dev;
	dev = usb_get_intfdata(intf);
	printk(KERN_ALERT"In disconnect\n");
	cdev_del(&dev->USBCDev);
	device_destroy(dev->USBDevCls, dev->devId);
	class_destroy(dev->USBDevCls);
	kfree(dev);
	unregister_chrdev_region(dev->devId,1);

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
MODULE_DEVICE_TABLE(usb,USBDev_id);
