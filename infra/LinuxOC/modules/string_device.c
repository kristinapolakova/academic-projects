#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>      //file_operations, регистрации устройств
#include <linux/uaccess.h> //copy_to_user, copy_from_user
#include <linux/rwlock.h>
#include <linux/device.h>
#include <linux/cdev.h>

#define DEVICE_NAME "string_device"
#define STRING_SIZE 256

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Kris");

static char device_string[STRING_SIZE];
static rwlock_t string_lock;
static dev_t dev_num;
static struct class *device_class = NULL;
static struct device *device_node = NULL;
static struct cdev char_device;

static int device_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "String device: opened\n");
    return 0;
}

static int device_release(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "String device: closed\n");
    return 0;
}

static ssize_t device_read(struct file *filp, char *buffer, size_t length, loff_t *offset)
{
    unsigned long flags;
    int bytes_read = 0;

    read_lock_irqsave(&string_lock, flags);

    if (*offset >= STRING_SIZE)
    {
        read_unlock_irqrestore(&string_lock, flags);
        return 0;
    }
    if (device_string[0] == '\0')
    {
        read_unlock_irqrestore(&string_lock, flags);
        return -EAGAIN;
    }

    bytes_read = strlen(device_string) - *offset;
    if (bytes_read > length)
        bytes_read = length;

    if (copy_to_user(buffer, device_string + *offset, bytes_read))
    {
        read_unlock_irqrestore(&string_lock, flags);
        return -EFAULT;
    }
    *offset += bytes_read;
    read_unlock_irqrestore(&string_lock, flags);
    return bytes_read;
}

static ssize_t device_write(struct file *filp, const char *buffer, size_t length, loff_t *offset)
{
    unsigned long flags;

    if (length >= STRING_SIZE)
        return -EINVAL;

    write_lock_irqsave(&string_lock, flags);
    memset(device_string, 0, STRING_SIZE);

    if (copy_from_user(device_string, buffer, length))
    {
        write_unlock_irqrestore(&string_lock, flags);
        return -EFAULT;
    }
    device_string[length] = '\0';
    printk(KERN_INFO "String device: wrote '%s'\n", device_string);

    write_unlock_irqrestore(&string_lock, flags);
    return length;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = device_open,
    .release = device_release,
    .read = device_read,
    .write = device_write,
};

static int __init string_device_init(void)
{
    int result;

    printk(KERN_INFO "String device: initializing\n");
    memset(device_string, 0, STRING_SIZE);
    rwlock_init(&string_lock);

    result = alloc_chrdev_region(&dev_num, 0, 1, DEVICE_NAME);
    if (result < 0)
    {
        printk(KERN_ALERT "String device: failed to register device\n");
        return result;
    }

    cdev_init(&char_device, &fops);
    char_device.owner = THIS_MODULE;

    result = cdev_add(&char_device, dev_num, 1);
    if (result < 0)
    {
        unregister_chrdev_region(dev_num, 1);
        printk(KERN_ALERT "String device: failed to add cdev\n");
        return result;
    }

    device_class = class_create(THIS_MODULE, DEVICE_NAME);
    if (IS_ERR(device_class))
    {
        cdev_del(&char_device);
        unregister_chrdev_region(dev_num, 1);
        printk(KERN_ALERT "String device: failed to create class\n");
        return PTR_ERR(device_class);
    }

    device_node = device_create(device_class, NULL, dev_num, NULL, DEVICE_NAME);
    if (IS_ERR(device_node))
    {
        class_destroy(device_class);
        cdev_del(&char_device);
        unregister_chrdev_region(dev_num, 1);
        printk(KERN_ALERT "String device: failed to create device\n");
        return PTR_ERR(device_node);
    }

    printk(KERN_INFO "String device: registered with major=%d minor=%d\n", MAJOR(dev_num), MINOR(dev_num));
    return 0;
}

static void __exit string_device_exit(void)
{
    device_destroy(device_class, dev_num);
    class_destroy(device_class);
    cdev_del(&char_device);
    unregister_chrdev_region(dev_num, 1);
    printk(KERN_INFO "String device: unloaded\n");
}

module_init(string_device_init);
module_exit(string_device_exit);