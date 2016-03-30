#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/cdev.h>
#include<linux/fs.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sarthak Jain");
MODULE_VERSION("0.2");

struct cdev *newDev;
int maj_no;
int ret;
dev_t crdev;
#define DEVICE_NAME "CryptoDevCHARDEVDRVR"

static int hello_init(void)
{
	printk(KERN_ALERT "This is my first kernel module\n");
	ret = alloc_chrdev_region(&crdev,0,1,DEVICE_NAME);
	if(ret < 0)
	{
		printk(KERN_ALERT "\n%s : Unable to assign Character Device Driver Region",DEVICE_NAME);
		return ret;
	}
	maj_no = MAJOR(crdev);
	printk(KERN_ALERT "%s : Major Number:%d\n",DEVICE_NAME,maj_no);
	newDev = cdev_alloc();
	newDev->owner = THIS_MODULE;
	ret = cdev_add(newDev,crdev,1);
	if(ret < 0)
	{
		printk(KERN_ALERT "%s : Unable to Register Device Driver\n",DEVICE_NAME);
	}
	printk(KERN_ALERT "Successfully Initialised Device Driver\n");
	return 0;
}

static void hello_destroy(void)
{
	printk(KERN_ALERT "Killing Hello-Start.c ... Byeeee\n");
	cdev_del(newDev);
	unregister_chrdev_region(crdev,1);
	printk(KERN_ALERT "%s : Successfully Unregistered Driver\n",DEVICE_NAME);
	printk(KERN_ALERT "Done");
}

module_init(hello_init);
module_exit(hello_destroy);
