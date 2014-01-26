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

struct USBDevice {

	struct cdev USBCDev;
	struct usb_device *USBDev;
	struct usb_interface *USBIntf;
	uint8_t *outBuffer;
	uint8_t *inBuffer;
	uint8_t inAddr;
	uint8_t outAddr;
	size_t inBufSize;
	size_t outBufSize;
	struct urb USB_in_urb;
	struct urb USB_out_urb;
	dev_t devId;
	struct class *USBDevCls;

};
/*******------Driver File Operations-------*******/

int USB_open(struct inode *indp , struct file *filp)
{
	struct USBDevice *dev = container_of(indp->i_cdev,
						struct USBDevice, USBCDev);
	filp->private_data = dev;
	return 0;
}

int USB_release(struct inode *indp, struct file *filp)
{
	return 0;
}

ssize_t USB_read(struct file *filp, char __user *usrData, size_t len,
		loff_t *loff)
{
	int count;
	struct USBDevice *dev = (struct USBDevice *) filp->private_data;
	ret = usb_interrupt_msg(dev->USBDev, usb_rcvbulkpipe(dev->USBDev,
				dev->inAddr),
			dev->inBuffer, min(dev->inBufSize, len), &count, HZ*10);
	copy_to_user(usrData, dev->inBuffer, min(dev->outBufSize, len));
	return ret;
}

ssize_t USB_write(struct file *filp, const char __user *usrData, size_t len,
		loff_t *loff)
{
	int count;
	struct USBDevice *dev = (struct USBDevice *) filp->private_data;
	copy_from_user(dev->outBuffer, usrData, min(dev->outBufSize, len));
	ret = usb_interrupt_msg(dev->USBDev, usb_sndbulkpipe(dev->USBDev,
				dev->outAddr), dev->outBuffer,
			min(dev->outBufSize, len), &count, HZ*10);
	return ret;

}


static const struct file_operations USB_ops = {
	.open = USB_open,
	.release = USB_release,
	.read = USB_read,
	.write = USB_write
};

/*****----Driver Probe And Disconnect------*******/

static struct usb_device_id USBDev_id[] = {
	{ USB_DEVICE(0x1cbe, 0x0003) },
	{}
};

int USB_probe(struct usb_interface *intf,
	  const struct usb_device_id *id)
{
	uint8_t i = 0;
	struct USBDevice *dev;
	struct usb_device *udev;
	struct usb_host_interface *USBIntf;
	struct usb_endpoint_descriptor *USBEpDescp;
	dev = kzalloc(sizeof(struct USBDevice), GFP_KERNEL);
	if (NULL == dev)
		return -ENOMEM;
	dev->inBuffer = NULL;
	dev->outBuffer = NULL;
	udev = interface_to_usbdev(intf);
	dev->USBDev = usb_get_dev(udev);
	dev->USBIntf = intf;
	usb_set_intfdata(intf, dev);

	ret = alloc_chrdev_region(&dev->devId, 0, 1, "USBDev");
	if (ret < 0)
		goto err1;
	cdev_init(&dev->USBCDev, &USB_ops);

	ret = cdev_add(&dev->USBCDev, dev->devId, 1);
	if (ret < 0)
		goto err2;

	dev->USBDevCls = class_create(THIS_MODULE, "USBDev");
	if (NULL == dev->USBDevCls)
		goto err3;
	retDev = device_create(dev->USBDevCls, NULL, dev->devId, NULL,
				"USBDev%d", MAJOR(dev->devId));
	if (NULL == retDev)
		goto err4;
	USBIntf = intf->cur_altsetting;
	for (i = 0; i < USBIntf->desc.bNumEndpoints; ++i) {
		USBEpDescp = &USBIntf->endpoint[i].desc;
		if ((USBEpDescp->bEndpointAddress & USB_DIR_IN) &&
		((USBEpDescp->bmAttributes & USB_ENDPOINT_XFERTYPE_MASK)
		 == USB_ENDPOINT_XFER_INT)) {
			dev->inAddr = USBEpDescp->bEndpointAddress;
			dev->inBufSize = USBEpDescp->wMaxPacketSize;
			dev->inBuffer = kzalloc(USBEpDescp->wMaxPacketSize,
						GFP_KERNEL);
			if (NULL == dev->inBuffer) {
				ret = ENOMEM;
				goto err5;
			}

		}
		if (!(USBEpDescp->bEndpointAddress & USB_DIR_IN) &&
		((USBEpDescp->bmAttributes & USB_ENDPOINT_XFERTYPE_MASK)
		 == USB_ENDPOINT_XFER_INT)) {
			dev->outAddr = USBEpDescp->bEndpointAddress;
			dev->outBufSize = USBEpDescp->wMaxPacketSize;
			dev->outBuffer = kzalloc(USBEpDescp->wMaxPacketSize,
						GFP_KERNEL);
			if (NULL == dev->outBuffer) {
				ret = ENOMEM;
				goto err5;
			}

		}
	}

	if (!(dev->outAddr && dev->inAddr))
		goto err5;
	printk(KERN_ALERT"1\n");
	return 0;
err5:
	printk(KERN_ALERT"2\n");
	if (dev->inBuffer != NULL)
		kfree(dev->inBuffer);
	if (dev->outBuffer != NULL)
		kfree(dev->outBuffer);

	device_destroy(dev->USBDevCls, dev->devId);
err4:
	printk(KERN_ALERT"3\n");
	class_destroy(dev->USBDevCls);
err3:
	printk(KERN_ALERT"4\n");
	cdev_del(&dev->USBCDev);
err2:
	printk(KERN_ALERT"5\n");
	unregister_chrdev_region(dev->devId, 1);
err1:
	printk(KERN_ALERT"6\n");
	usb_put_dev(dev->USBDev);
	kfree(dev);
	usb_set_intfdata(intf, NULL);
	printk(KERN_ALERT"7\n");
	return -ret;

}

void USB_disconnect(struct usb_interface *intf)
{
	struct USBDevice *dev;
	dev = usb_get_intfdata(intf);
	if (NULL != dev) {
	if (dev->inBuffer != NULL)
		kfree(dev->inBuffer);
	if (dev->outBuffer != NULL)
		kfree(dev->outBuffer);
	cdev_del(&dev->USBCDev);
	if (NULL != dev->USBDevCls) {
	device_destroy(dev->USBDevCls, dev->devId);
	class_destroy(dev->USBDevCls);
	}
	usb_put_dev(dev->USBDev);
	unregister_chrdev_region(dev->devId, 1);
	kfree(dev);
	}
}

static struct usb_driver USBDevDriver = {
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

static void __exit project_exit(void)
{
	usb_deregister(&USBDevDriver);
}

module_init(project_init);
module_exit(project_exit);
MODULE_LICENSE("GPL");
MODULE_DEVICE_TABLE(usb, USBDev_id);
