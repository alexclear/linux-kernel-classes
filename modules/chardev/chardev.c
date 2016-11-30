#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <asm/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Alex Chistyakov");
MODULE_DESCRIPTION("A simple character device driver module");
MODULE_VERSION("1.0");

#define MAJOR_NUM 223
static int number_of_opens;
static int chunk_size;

ssize_t chardev_read (struct file *chardev_file, char __user *buf, size_t size, loff_t *offset) {
	int read_size, i, j;
	printk(KERN_INFO "In chardev_read, file: %s, size: %d\n", chardev_file->f_path.dentry->d_name.name, size);
	char* chunk = kmalloc(chunk_size, GFP_KERNEL);
	for(i=0; i<(size/chunk_size); i++) {
		for(j=0; j<chunk_size; j++) {
			chunk[j] = 'a';
		}
		copy_to_user(buf + (i*chunk_size), chunk, chunk_size);
		read_size += (i * chunk_size);
	}
	kfree(chunk);
	return read_size;
}

ssize_t chardev_write (struct file *chardev_file, const char __user *buf, size_t size, loff_t *offset) {
	printk(KERN_INFO "In chardev_write, file: %s, size: %d\n", chardev_file->f_path.dentry->d_name.name, size);
	return 0;
}

int chardev_open (struct inode *chardev_inode, struct file *chardev_file) {
	number_of_opens++;
	printk(KERN_INFO "In chardev_open, %d, file: %s\n", number_of_opens, chardev_file->f_path.dentry->d_name.name);
	return 0;
}

int chardev_release (struct inode *chardev_inode, struct file *chardev_file) {
	printk(KERN_INFO "In chardev_release, file: %s\n", chardev_file->f_path.dentry->d_name.name);
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
	number_of_opens = 0;
	chunk_size = 4;
	
	result = register_chrdev(MAJOR_NUM, "chardev", &file_ops);
	
	printk(KERN_INFO "Result: %d\n", result);
	return 0;
}

static void __exit chardev_exit(void) {
	unregister_chrdev(MAJOR_NUM, "chardev");
	printk(KERN_INFO "Deinitializing a chardev driver\n");
}

module_init(chardev_init);
module_exit(chardev_exit);
