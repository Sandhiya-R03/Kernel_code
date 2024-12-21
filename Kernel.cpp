#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/proc_fs.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A comprehensive Linux kernel module");
MODULE_VERSION("1.0");

// Module parameters
#define DEVICE_NAME "example_char_device"
#define PROC_FILENAME "example_proc"
#define BUFFER_SIZE 1024

// Device number
static int major_number;

// Buffers for device and proc
static char device_buffer[BUFFER_SIZE];
static char *proc_buffer;

// Open, close, read, and write counters
static int open_count = 0;
static int close_count = 0;
static int read_count = 0;
static int write_count = 0;

// Prototypes
static int dev_open(struct inode *, struct file *);
static int dev_release(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char __user *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char __user *, size_t, loff_t *);
static ssize_t proc_read(struct file *, char __user *, size_t, loff_t *);
static ssize_t proc_write(struct file *, const char __user *, size_t, loff_t *);

// File operations for character device
static struct file_operations fops = {
    .open = dev_open,
    .release = dev_release,
    .read = dev_read,
    .write = dev_write,
};

// File operations for proc entry
static struct proc_ops proc_ops = {
    .proc_read = proc_read,
    .proc_write = proc_write,
};

// Module load function
static int __init example_init(void) {
    printk(KERN_INFO "ExampleModule: Initializing the module\n");

    // Allocate major number
    major_number = register_chrdev(0, DEVICE_NAME, &fops);
    if (major_number < 0) {
        printk(KERN_ALERT "ExampleModule: Failed to register a major number\n");
        return major_number;
    }
    printk(KERN_INFO "ExampleModule: Registered with major number %d\n", major_number);

    // Create proc entry
    proc_buffer = kmalloc(BUFFER_SIZE, GFP_KERNEL);
    if (!proc_buffer) {
        printk(KERN_ALERT "ExampleModule: Failed to allocate memory for proc\n");
        unregister_chrdev(major_number, DEVICE_NAME);
        return -ENOMEM;
    }

    struct proc_dir_entry *proc_entry = proc_create(PROC_FILENAME, 0666, NULL, &proc_ops);
    if (!proc_entry) {
        printk(KERN_ALERT "ExampleModule: Failed to create proc entry\n");
        kfree(proc_buffer);
        unregister_chrdev(major_number, DEVICE_NAME);
        return -ENOMEM;
    }

    printk(KERN_INFO "ExampleModule: Module initialized successfully\n");
    return 0;
}

// Module unload function
static void __exit example_exit(void) {
    remove_proc_entry(PROC_FILENAME, NULL);
    unregister_chrdev(major_number, DEVICE_NAME);
    kfree(proc_buffer);
    printk(KERN_INFO "ExampleModule: Module unloaded\n");
}

// Device open function
static int dev_open(struct inode *inodep, struct file *filep) {
    open_count++;
    printk(KERN_INFO "ExampleModule: Device opened (%d times)\n", open_count);
    return 0;
}

// Device close function
static int dev_release(struct inode *inodep, struct file *filep) {
    close_count++;
    printk(KERN_INFO "ExampleModule: Device closed (%d times)\n", close_count);
    return 0;
}

// Device read function
static ssize_t dev_read(struct file *filep, char __user *buffer, size_t len, loff_t *offset) {
    int bytes_read = 0;
    if (*offset >= BUFFER_SIZE || device_buffer[*offset] == '\0') {
        return 0; // End of buffer
    }

    while (len > 0 && *offset < BUFFER_SIZE && device_buffer[*offset] != '\0') {
        put_user(device_buffer[*offset], buffer++);
        len--;
        bytes_read++;
        (*offset)++;
    }
    read_count++;
    printk(KERN_INFO "ExampleModule: Read %d bytes (%d reads so far)\n", bytes_read, read_count);
    return bytes_read;
}

// Device write function
static ssize_t dev_write(struct file *filep, const char __user *buffer, size_t len, loff_t *offset) {
    int bytes_written = 0;

    if (*offset >= BUFFER_SIZE - 1) {
        return -EFAULT; // No space left
    }

    while (len > 0 && *offset < BUFFER_SIZE - 1) {
        get_user(device_buffer[*offset], buffer++);
        (*offset)++;
        bytes_written++;
        len--;
    }
    device_buffer[*offset] = '\0'; // Null-terminate
    write_count++;
    printk(KERN_INFO "ExampleModule: Wrote %d bytes (%d writes so far)\n", bytes_written, write_count);
    return bytes_written;
}

// Proc read function
static ssize_t proc_read(struct file *filep, char __user *buffer, size_t len, loff_t *offset) {
    int bytes_read = 0;
    const char *message = "This is the proc entry\n";
    while (len > 0 && *offset < strlen(message)) {
        put_user(message[*offset], buffer++);
        (*offset)++;
        bytes_read++;
        len--;
    }
    return bytes_read;
}

// Proc write function
static ssize_t proc_write(struct file *filep, const char __user *buffer, size_t len, loff_t *offset) {
    int bytes_written = 0;
    while (len > 0 && *offset < BUFFER_SIZE - 1) {
        get_user(proc_buffer[*offset], buffer++);
        (*offset)++;
        bytes_written++;
        len--;
    }
    proc_buffer[*offset] = '\0';
    printk(KERN_INFO "ExampleModule: Proc entry updated\n");
    return bytes_written;
}

// Register module entry and exit points
module_init(example_init);
module_exit(example_exit);
