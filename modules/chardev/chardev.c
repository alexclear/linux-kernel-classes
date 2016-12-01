#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <asm/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Alex Chistyakov");
MODULE_DESCRIPTION("A simple character device driver module");
MODULE_VERSION("1.0");

#define MAJOR_NUM 223
#define CHUNK_SIZE 4
#define WRITEBUF_SIZE 4096000
static int number_of_opens;
static int chunk_size;
static char* write_buffer;
static spinlock_t writer_spinlock;

static int get_number_of_opens (void) {
	return number_of_opens;
}
EXPORT_SYMBOL(get_number_of_opens);

ssize_t chardev_read (struct file *chardev_file, char __user *buf, size_t size, loff_t *offset) {
	int read_size, i, j;
	read_size=0;
	printk(KERN_INFO "In chardev_read, file: %s, size: %d\n", chardev_file->f_path.dentry->d_name.name, size);
	char* chunk = kmalloc(chunk_size, GFP_KERNEL);
	for(i=0; i<(size/chunk_size); i++) {
		//printk("i: %d\n", i);
		for(j=0; j<chunk_size; j++) {
			//printk("j: %d\n", j);
			chunk[j] = 'a';
		}
		//printk("Copying to: %d\n", (i*chunk_size));
		copy_to_user(buf + (i*chunk_size), chunk, chunk_size);
		//printk("read_size before: %d\n", read_size);
		read_size += chunk_size;
		//printk("read_size after: %d\n", read_size);
	}
	kfree(chunk);
	return read_size;
}

ssize_t chardev_write (struct file *chardev_file, const char __user *buf, size_t size, loff_t *offset) {
	int i, j;
	char symbol;
	spin_lock(&writer_spinlock);
	//printk(KERN_INFO "In chardev_write, file: %s, size: %d\n", chardev_file->f_path.dentry->d_name.name, size);
	if(size > WRITEBUF_SIZE) {
		return -1;
	}
	if((size % chunk_size) != 0) {
		return -1;
	}
	for(i=0; i<(size/chunk_size); i++) {
		copy_from_user(write_buffer + (i*chunk_size), buf + (i*chunk_size), chunk_size);
	}
	symbol = write_buffer[0];
	for(i=1; i<size; i++) {
		if(symbol != write_buffer[i]) {
			printk("Race conditions are possible!\n");
			break;
		}
	}
	spin_unlock(&writer_spinlock);
	return size;
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
	chunk_size = CHUNK_SIZE;

	printk(KERN_INFO "Allocating a write buffer\n");
	write_buffer = kmalloc(WRITEBUF_SIZE, GFP_KERNEL);
	printk(KERN_INFO "Buffer allocated: %d\n", write_buffer);
	
	spin_lock_init(&writer_spinlock);

	result = register_chrdev(MAJOR_NUM, "chardev", &file_ops);
	
	printk(KERN_INFO "Result: %d\n", result);
	return 0;
}

static void __exit chardev_exit(void) {
	unregister_chrdev(MAJOR_NUM, "chardev");
	kfree(write_buffer);
	printk(KERN_INFO "Deinitializing a chardev driver\n");
}

module_init(chardev_init);
module_exit(chardev_exit);
