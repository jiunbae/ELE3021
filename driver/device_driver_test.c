#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <asm/uaccess.h>

#define DEVICE_NAME "mydrv"
#define MYDRV_MAX_LENGTH 4096
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

struct class *myclass;
struct cdev *mycdev;
struct device *mydevice;
dev_t mydev;

static char * mydrv_data;
static int mydrv_read_offset, mydrv_write_offset;

static int mydrv_open(struct inode *inode, struct file *file) {
	printk("%s\n", __FUNCTION__);
	return 0;
}

static int mydrv_release(struct inode *inode, struct file *file) {
	printk("%s\n", __FUNCTION__);
	return 0;
}

static ssize_t mydrv_read(struct file *file, char *buf, size_t count, loff_t *ppos) {
	printk("mydrv_read: %s\n", buf);
	if ((buf == NULL) || (count < 0)) {
		return -EINVAL;
	}
	if ((mydrv_write_offset - mydrv_read_offset) <= 0) {
		return 0;
	}
	count = MIN((mydrv_write_offset - mydrv_read_offset), count);
	if (copy_to_user(buf, mydrv_data + mydrv_read_offset, count)) {
		return -EFAULT;
	}
	mydrv_read_offset += count;
	return count;
}

static ssize_t mydrv_write(struct file *file, const char * buf, size_t count, loff_t *ppos) {
	printk("mydrv_write: %s\n", buf);
	if ((buf == NULL) || (count < 0)) {
		return -EINVAL;
	}
	if (count + mydrv_write_offset >= MYDRV_MAX_LENGTH) {
		return 0;
	}
	if (copy_from_user(mydrv_data + mydrv_write_offset, buf, count)) {
		return -EFAULT;
	}
	mydrv_write_offset += count;
	return count;
}

struct file_operations mydrv_fops = {
	.owner = THIS_MODULE,
	.read = mydrv_read,
	.write = mydrv_write,
	.open = mydrv_open,
	.release = mydrv_release,
};

int mydrv_init(void) {
	if (alloc_chrdev_region(&mydev, 0, 1, DEVICE_NAME) < 0) {
		return -EBUSY;
	}
	myclass = class_create(THIS_MODULE, "mycharclass");
	if (IS_ERR(myclass)) {
		class_destroy(myclass);
		unregister_chrdev_region(mydev, 1);
		return PTR_ERR(mydevice);
	}
	mycdev = cdev_alloc();
	mycdev->ops = &mydrv_fops;
	mycdev->owner = THIS_MODULE;
	if (cdev_add(mycdev, mydev, 1) < 0) {
		device_destroy(myclass, mydev);
		class_destroy(myclass);
		unregister_chrdev_region(mydev, 1);
		return -EBUSY;
	}
	mydrv_data = (char*)kmalloc(MYDRV_MAX_LENGTH * sizeof(char), GFP_KERNEL);
	mydrv_read_offset = mydrv_write_offset = 0;
	return 0;
}

void mydrv_cleanup(void) {
	kfree(mydrv_data);
	cdev_del(mycdev);
	device_destroy(myclass, mydev);
	class_destroy(myclass);
	unregister_chrdev_region(mydev, 1);
}

module_init(mydrv_init);
module_exit(mydrv_cleanup);
MODULE_LICENSE("GPL");
