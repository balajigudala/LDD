#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/kdev_t.h>
#include <linux/poll.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/slab.h>

#define DRIVER_NAME "test_driver"
#define COUNT 1
#define BUFFER_SIZE 5

static dev_t devno;
static long int mjrno = 42, mnrno = 0;
static struct class *cls;
static struct device *dvc;

struct mybuff {
	char *buff;
	struct cdev mycdev;
} mybuff_var;

static int myopen(struct inode *, struct file *);
static int myclose(struct inode *, struct file *);
static ssize_t mywrite(struct file *, const char *, size_t, loff_t *);
static ssize_t myread(struct file *, char *, size_t, loff_t *);

static struct file_operations myfile_ops = {
	.owner = THIS_MODULE,
	.open = myopen,
	.read = myread,
	.write = mywrite,
	.release = myclose
};

static int myopen(struct inode *iinode, struct file *fle)
{
	/* Allocate memory for the buffer and set private data */
	
	mybuff_var.buff = (char *)kmalloc(BUFFER_SIZE, GFP_KERNEL);
	if (NULL == mybuff_var.buff) {
		pr_info("failed to alloc memory for buffer\n");
		return -1;
	}

	fle->private_data = &mybuff_var;
	pr_info("myopen function invoked\n");
	
	return 0;
}

static int myclose(struct inode *iinode, struct file *file)
{
	/* Free the buffer */
	
	kfree(mybuff_var.buff);
	pr_info("myclose function invoked\n");
	
	return 0;
}

static ssize_t myread(struct file *fle, char *buff, size_t len, loff_t *offset)
{
	unsigned long int res;
	struct mybuff *dum = fle->private_data;
	
	pr_info("myread function invoked\n");

	if (len > BUFFER_SIZE)
		len = BUFFER_SIZE;

	res = copy_to_user(buff, dum->buff, len);

	return len;
}

static ssize_t mywrite(struct file *fle, const char *buff, size_t len, loff_t *offset)
{
	unsigned long int res;
	struct mybuff *dum = fle->private_data;
	pr_info("mywrite function invoked\n");

	if (len > BUFFER_SIZE)
		len = BUFFER_SIZE;

	res = copy_from_user(dum->buff, buff, len);
	
	return len; 
}

static int fun_start(void)
{
	int res = 0;

	pr_info("my module is loaded successfully\n");
	devno = MKDEV(mjrno, mnrno);
	
	pr_info("devno: %d\n", devno);
	pr_info("mjr: %ld\n", mjrno);
	pr_info("mnr: %ld\n", mnrno);

	res = register_chrdev_region(devno, COUNT, DRIVER_NAME);
	if (0 > res) {
		pr_err("failed to register the driver\n");
		return -1;
	}
	pr_info("driver is registered\n");

	cls = class_create("mydriver_folder");
	if (NULL == cls) {
		pr_info("folder is not created for the device file\n");
		return -2;
	}
	pr_info("device folder is created\n");

	dvc = device_create(cls, NULL, devno, NULL, "mydriver_file");
	if (NULL == dvc) {
		pr_info("device file is not created\n");
		return -3;
	}
	pr_info("device file is created\n");

	cdev_init(&mybuff_var.mycdev, &myfile_ops);
	res = cdev_add(&mybuff_var.mycdev, devno, 1);
	if (0 > res) {
		pr_info("failed to add device info to cdev structure\n");
		return -4;
	}
	pr_info("the driver info is added to cdev structure\n");

	pr_info("our char driver module is added to the kernel\n");

	return 0;
}

static void fun_end(void)
{
	device_destroy(cls, devno);
	pr_info("the device file is destroyed\n");
	
	class_destroy(cls);
	pr_info("the device folder is destroyed\n");
	
	cdev_del(&mybuff_var.mycdev);
	pr_info("the cdev object for the driver is deleted\n");
	
	unregister_chrdev_region(devno, COUNT);
	pr_info("the driver is unregistered\n");

	pr_info("my module: unloaded successfully\n");

	return;
}
module_init(fun_start);
module_exit(fun_end);

MODULE_AUTHOR("BALAJI");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("basic_module");
