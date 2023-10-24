#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/slab.h>                 
#include <linux/uaccess.h>             
#include <linux/ioctl.h>   
#include <linux/sched/signal.h> 
#include <linux/proc_fs.h> 
#include <linux/fcntl.h> 
#include <linux/types.h>
#include <linux/gpio.h> 
#include <linux/interrupt.h>
#include "utils.h"

#define DEVICE_NAME "motion"
#define CLASS_NAME "motionClass"
#define REG_CURRENT_TASK _IOW('a','a',int32_t*)

// #define SIGH 10 	// SIGUSR1
#define SIGH SIGUSR1

#define IOCTL_PID 1
#define GPIO_INPUT 0

MODULE_LICENSE("GPL");

// Debounce mechanisms
#define EN_DEBOUNCE 
#ifdef EN_DEBOUNCE

#include <linux/jiffies.h> 
extern unsigned long volatile jiffies;
unsigned long old_jiffie = 0;

#endif // !EN_DEBOUNCE

static struct kernel_siginfo info;
static pid_t pid;
static struct task_struct *task = NULL;

static dev_t dev;
static struct class *dev_class = NULL;
static struct cdev c_dev;  // Character device structure

struct GpioRegisters *s_pGpioRegisters;
static unsigned int pinNum = 16;
static unsigned int irqNumber;

static int __init motion_driver_init(void);
static void __exit motion_driver_exit(void);
static int motion_open(struct inode *inode, struct file *file);
static int motion_close(struct inode *inode, struct file *file);
static ssize_t motion_read(struct file *filp, char __user *buf, size_t len, loff_t *off);
static ssize_t motion_write(struct file *filp, const char *buf, size_t len, loff_t *off);
static long motion_ioctl(struct file *file, unsigned int cmd, unsigned long arg);

static irqreturn_t irq_handler(int irq, void *dev_id)
{  	
	int pinVal = gpio_get_value(pinNum);

//Debounce mechanisms
#ifdef EN_DEBOUNCE
	unsigned long diff = jiffies - old_jiffie;

	if (diff < 200)
	{
		return IRQ_HANDLED;
	}

	old_jiffie = jiffies;
#endif

	printk(KERN_INFO "[MOTION] Interruption handler: PIN -> %d.\n", pinVal);
	info.si_signo = SIGH;
	info.si_code = SI_QUEUE;
	info.si_int = pinVal;
 	
 	task = pid_task(find_pid_ns(pid, &init_pid_ns), PIDTYPE_PID);
	
	if(task != NULL)
		send_sig_info(SIGH, &info, task);

	return IRQ_HANDLED;
}

static int motion_open(struct inode* inode, struct file *file)
{
	printk(KERN_INFO "[MOTION] Device File Opened\n");
	return 0;
}

static int motion_close(struct inode *inode, struct file * file)
{
	printk(KERN_INFO "[MOTION] Device File Closed\n");
	return 0;
}

static ssize_t motion_write(struct file *filp, const char __user *buf, size_t len, loff_t *off) 
{	
   	printk(KERN_INFO "[MOTION] Write function\n");
   	return 0;
}

static ssize_t motion_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
	char buffer[2];
	int i = gpio_get_value(pinNum);
	int ret = 0;

	sprintf(buffer, "%d", i);
	ret = copy_to_user(buf, buffer, 1);
	printk(KERN_INFO "[MOTION] PIN -> %d\n", i);
	
	// returns number of bytes that cannot be read
	// in success it must be zero
	return ret;
}

static long motion_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{	
	if(cmd == IOCTL_PID)
	{
		if(copy_from_user(&pid, (pid_t*)arg, sizeof(pid_t))) 
		{
			printk(KERN_INFO "[MOTION] copy_from_user failed\n"); 
			return -1;
		}
		printk(KERN_INFO "[MOTION] Requested by PID %d\n", pid);
	}
	else
	{
		printk(KERN_INFO "[MOTION] ioctl failed\n");	
	}
	return 0;
}

static struct file_operations fops = 
{
	.owner = THIS_MODULE,
	.write = motion_write,
	.read = motion_read,
	.release = motion_close,
	.open = motion_open,
	.unlocked_ioctl = motion_ioctl,
};

static int __init motion_driver_init(void) 
{
	if ((alloc_chrdev_region(&dev, 0, 1, DEVICE_NAME)) < 0) 
	{
		printk(KERN_INFO "[MOTION] Cannot allocate major number\n");
	    return -1;
	}

	/*Creating cdev structure*/
	cdev_init(&c_dev, &fops);

	/*Adding character device to the system*/
	if((cdev_add(&c_dev,dev,1)) < 0)
	{
		printk(KERN_INFO "[MOTION] Cannot add the device to the system\n");
		unregister_chrdev_region(dev,1);
	}	

	/*Creating struct class*/
	if((dev_class = class_create(THIS_MODULE, CLASS_NAME)) == NULL)
	{
		printk(KERN_INFO "[MOTION] Cannot create the struct class\n");
		unregister_chrdev_region(dev,1);
	}

	/*Creating device*/
	if((device_create(dev_class, NULL, dev, NULL, DEVICE_NAME)) == NULL)
	{
		printk(KERN_INFO "[MOTION] Cannot create the Device\n");
		class_destroy(dev_class);
		unregister_chrdev_region(dev,1);
	}
    	
	irqNumber = gpio_to_irq(pinNum);

	if (request_irq(irqNumber, irq_handler, IRQF_TRIGGER_RISING, DEVICE_NAME, (void *)(irq_handler)))
	{
		printk(KERN_INFO "[MOTION] Cannot register IRQ\n");
		free_irq(irqNumber,(void *)(irq_handler));
		class_destroy(dev_class);
		unregister_chrdev_region(dev,1);
	}
    
    	s_pGpioRegisters = (struct GpioRegisters *)ioremap(GPIO_BASE, sizeof(struct GpioRegisters));

	SetGPIOFunction(s_pGpioRegisters, pinNum, GPIO_INPUT);

	return 0;
}

static void __exit motion_driver_exit(void) 
{
	SetGPIOFunction(s_pGpioRegisters, pinNum, GPIO_INPUT);
	iounmap(s_pGpioRegisters);
	
	free_irq(irqNumber,(void *)(irq_handler));
	device_destroy(dev_class, dev);
	class_destroy(dev_class);
	cdev_del(&c_dev);
	unregister_chrdev_region(dev, 1);
	printk(KERN_INFO "[MOTION] Device Driver Remove\n");
}

module_init(motion_driver_init);
module_exit(motion_driver_exit);
