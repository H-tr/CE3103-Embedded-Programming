#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <asm/uaccess.h> // for put_user and get_user

static int device_open(struct inode*, struct file*);
static int device_release(struct inode*, struct file*);
static ssize_t device_read(struct file*, char*, size_t, loff_t*);
static ssize_t device_write(struct file*, const char*, size_t, loff_t*);

static struct file_operations fops = {.read = device_read,
                                      .write = device_write,
                                      .open = device_open,
                                      .release = device_release};

char msg_buf[20], ctr = 0;  // buffer to store message from user space
char msg1[9] = "myDriver";  // message to be sent to the user space
int errorno, msg1len = 8;

static int __init myDriver_init(void) {
    printk(KERN_ALERT "Loading modIO\n");
    register_chrdev(101, "myDriver", &fops);
    return 0;
}

static void __exit myDriver_cleanup(void) {
    unregister_chrdev(101, "myDriver");
    printk(KERN_ALERT "Unloading modIO\n");
}

static int device_open(struct inode* inode, struct file* file) {
    printk(KERN_ALERT "Someone opened modIO\n");
    return 0;
}

static int device_release(struct inode* inode, struct file* file) {
    printk(KERN_ALERT "Some closed modIO\n");
    return 0;
}

static ssize_t device_read(struct file* filp, char* buf, size_t len, loff_t* loff) {
    printk(KERN_ALERT "Reading the device 101\n");
    if (ctr == 0)
        return 0;
    errorno = copy_to_user(buf, msg1, msg1len); // write to user space
    ctr = 0;
    return msg1len;
}

static ssize_t device_write(struct file* filp, const char* buf, size_t len, loff_t* loff) {
    printk(KERN_ALERT "writing to device 101\n");
    if (ctr > 0)
        return 0;   // no space to accept another char*
    errorno = copy_from_user(msg_buf, buf, 12);
    printk(KERN_ALERT "%s", msg_buf);
    ctr = 1;
    return 1;   // one message read from buffer
}

module_init(myDriver_init);
module_exit(myDriver_cleanup);

MODULE_LICENSE("GPL");  // provides information (via moodinfo) about the licensing terms of the module
MODULE_AUTHOR("Hu Tianrun");
MODULE_DESCRIPTION("my first driver -v1");
MODULE_VERSION("V1");