#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/cdev.h>
#include<linux/fs.h>
#include<linux/semaphore.h>
#include<asm/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sarthak Jain");
MODULE_VERSION("0.2");

struct cdev *newDev;
int maj_no;
int ret;
dev_t crdev;
#define DEVICE_NAME "CryptoDevCHARDEVDRVR"

struct dummy{
	char string[100];
	int length;
	struct semaphore sem;
}device;

int device_open(struct inode *node,struct file *fp)
{
	printk(KERN_ALERT "Atempting to open device file\n");
	if(down_interruptible(&device.sem) != 0)
	{
		printk(KERN_ALERT "%s : Unable to Lock file while open\n",DEVICE_NAME);
		return -1;
	}
	printk(KERN_ALERT "File open operation Complete\n");
	return 0;
}

ssize_t device_read(struct file* fp,char* buffer, size_t bufsize, loff_t* buffoff)
{
	printk(KERN_ALERT "Reading from the device...\n");
	ret = copy_to_user(buffer,device.string,bufsize);
	device.length = bufsize;
	return ret;
}

ssize_t device_write(struct file* fp,const char* buffer, size_t bufsize, loff_t* buffoff)
{
	printk(KERN_ALERT "Writing to the device...\n");
	ret = copy_from_user(device.string,buffer,bufsize);
	printk(KERN_ALERT "%s\n",device.string);
	printk(KERN_ALERT "Written\n");
	device.length = bufsize;
	return ret;
}

int device_close(struct inode* node, struct file* fp)
{
	printk(KERN_ALERT "Closing Device File");
	up(&device.sem);
	printk(KERN_ALERT "Device Close Successfully");
	return 0;
}

struct file_operations fop = {
	.owner = THIS_MODULE,
	.open = device_open,
	.release = device_close,
	.read = device_read,
	.write = device_write
};

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
	newDev->ops = &fop;
	newDev->owner = THIS_MODULE;
	ret = cdev_add(newDev,crdev,1);
	if(ret < 0)
	{
		printk(KERN_ALERT "%s : Unable to Register Device Driver\n",DEVICE_NAME);
	}
	sema_init(&device.sem,1);
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
