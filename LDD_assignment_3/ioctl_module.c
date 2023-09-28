#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/ioctl.h>
#include <linux/err.h>
 
#define BASIC_MAGIC 'a'
#define MAX_DEV 1
#define RD_VALUE _IOR(BASIC_MAGIC, 1, char *)
#define WR_VALUE _IOW(BASIC_MAGIC, 2, char *)
#define B_SIZE 20
 
char mybuff[B_SIZE];
int mjr, mnr, devno; 
static struct class *dev_class;
static struct cdev my_cdev;

static int my_driver_init(void);
static void my_driver_exit(void);
static int my_open(struct inode *inode, struct file *file);
static int my_release(struct inode *inode, struct file *file);
static ssize_t my_read(struct file *filp, char *buf, 
		size_t len, loff_t * off);
static ssize_t my_write(struct file *filp, const char *buf, 
		size_t len, loff_t * off);
static long my_ioctl(struct file *file, unsigned int cmd, 
		unsigned long arg);

static struct file_operations fops =
{
        .owner          = THIS_MODULE, 
        .read           = my_read, 
        .write          = my_write, 
        .open           = my_open, 
        .unlocked_ioctl = my_ioctl, 
        .release        = my_release, 
};

static int my_open(struct inode *inode, struct file *file)
{
        pr_info("Device File Opened...\n");
        
	return 0;
}

static int my_release(struct inode *inode, struct file *file)
{
        pr_info("Device File Closed...\n");
        
	return 0;
}

static ssize_t my_read(struct file *filp, char *buf, size_t len, 
		loff_t *off)
{
        pr_info("Read Function invoked\n");
        
	return 0;
}

static ssize_t my_write(struct file *filp, const char *buf, 
		size_t len, loff_t *off)
{
        pr_info("Write function invoked\n");
        
	return len;
}

static long my_ioctl(struct file *file, unsigned int cmd, 
		unsigned long arg)
{
	pr_info("%s invoked\n", __func__);
         switch (cmd) {
                case WR_VALUE:
                        if (copy_from_user(mybuff, (char *) arg, 
						B_SIZE)) {
                                pr_err("Data Write : Err!\n");
                        }
                        pr_info("%s \n", mybuff);
                        break;

                case RD_VALUE:
                        if( copy_to_user((char *) arg, "msg from kernel", 
						15)) {
                                pr_err("Data Read : Err!\n");
                        }
                        pr_info("messege sent\n");
                        break;
        }
        return 0;
}
 
static int my_driver_init(void)
{
	if((alloc_chrdev_region(&devno, 0, MAX_DEV, "my_Dev")) <0){
		pr_err("Cannot allocate major number\n");
		return -1;
	}
	pr_info("Major = %d \nMinor = %d \n", mjr = MAJOR(devno),
			mnr = MINOR(devno));

	cdev_init(&my_cdev, &fops);

	if((cdev_add(&my_cdev, devno, 1)) < 0){
		pr_err("Cannot add the device to the system\n");
		return -2;
	}

	if(IS_ERR(dev_class = class_create(THIS_MODULE, "my_class"))){
		pr_err("Cannot create the struct class\n");
		return -3;
	}

	if(IS_ERR(device_create(dev_class, NULL, devno, NULL, "mydev_file"))){
		pr_err("Cannot create the Device 1\n");
		return -4;
	}
	pr_info("Device Driver Insert...Done\n");

	return 0;
}

static void my_driver_exit(void)
{
	device_destroy(dev_class, devno);
	class_destroy(dev_class);
	cdev_del(&my_cdev);
	unregister_chrdev_region(devno, MAX_DEV);
	pr_info("Device Driver Remove...Done!!!\n");

	return;
}

module_init(my_driver_init);
module_exit(my_driver_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("BALU");
//MODULE_DESCRIPTION("IOCTL MODULE");
MODULE_DESCRIPTION("IOCTL MODULE");
