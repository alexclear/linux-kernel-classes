#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Alex Chistyakov");
MODULE_DESCRIPTION("A hello world module");
MODULE_VERSION("1.0");

static int __init hello_init(void) {
	printk(KERN_ERR "Hello world!\n");
	return 0;
}

static void __exit hello_exit(void) {
	printk(KERN_INFO "Unloading a module\n");
}

module_init(hello_init);
module_exit(hello_exit);
