#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include "../chardev/chardev.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Alex Chistyakov");
MODULE_DESCRIPTION("A module which depends on chardev module");
MODULE_VERSION("1.0");

static int __init depnmodule_init(void) {
	printk(KERN_ERR "depnmodule initialized, %d\n", get_number_of_opens());
	return 0;
}

static void __exit depnmodule_exit(void) {
	printk(KERN_INFO "depnmodule unloaded\n");
}

module_init(depnmodule_init);
module_exit(depnmodule_exit);
