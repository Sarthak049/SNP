#include<linux/printk.h>
#include<linux/module.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sarthak Jain");
MODULE_VERSION("0.2");

static int hello_init(void)
{
	pr_alert("This is my first kernel module\n");
	return 0;
}

static void hello_destroy(void)
{
	pr_alert("Killing Hello-Start.c ... Byeeee\n");
}

module_init(hello_init);
module_exit(hello_destroy);
