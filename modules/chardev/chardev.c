#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Alex Chistyakov");
MODULE_DESCRIPTION("A simple character device driver module");
MODULE_VERSION("1.0");

#define MAJOR_NUM 223

ssize_t chardev_read (struct file *chardev_file, char __user *user, size_t size, loff_t *offset) {
	printk(KERN_INFO "In chardev_read\n");
	return 0;
}

ssize_t chardev_write (struct file *chardev_file, const char __user *user, size_t size, loff_t *offset) {
	printk(KERN_INFO "In chardev_write\n");
	return 0;
}

int chardev_open (struct inode *chardev_inode, struct file *chardev_file) {
	printk(KERN_INFO "In chardev_open\n");
	return 0;
}

int chardev_release (struct inode *chardev_inode, struct file *chardev_file) {
	printk(KERN_INFO "In chardev_release\n");
	return 0;
}

static const struct file_operations file_ops = {
        .owner          = THIS_MODULE,
        .read           = chardev_read,
        .write          = chardev_write,
        .open           = chardev_open,
        .release        = chardev_release
};

static int __init chardev_init(void) {
	int result;

	printk(KERN_INFO "Initializing the chardev driver\n");
	
	result = register_chrdev(MAJOR_NUM, "chardev", &file_ops);
	printk(KERN_INFO "Result: %d\n", result);
	return 0;
}

static void __exit chardev_exit(void) {
	printk(KERN_INFO "Deinitializing a chardev driver\n");
}

module_init(chardev_init);
module_exit(chardev_exit);
