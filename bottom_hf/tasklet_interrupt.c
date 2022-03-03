#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>


#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fcntl.h> /*Helps fix O_ACCMODE*/
#include <linux/sched.h> /*Helps fix TASK_UNINTERRUPTIBLE */
#include <linux/fs.h> /*Helps fix the struct intializer */

#include<linux/slab.h>                 //kmalloc()
#include<linux/uaccess.h>              //copy_to/from_user()
#include <linux/ioctl.h>

#include <linux/proc_fs.h>    // proc file system
#include <linux/compiler.h>
#include <linux/types.h>
#include <linux/proc_ns.h>

#include <linux/kobject.h>

#include <linux/interrupt.h>

#include <linux/kthread.h> // therad 
#include <linux/delay.h>   // sleep 
#include <linux/sched.h>               //task_struct

#include <linux/timer.h>
#include <linux/jiffies.h>

#include <linux/workqueue.h> // Required for workqueues

#include <linux/proc_fs.h>
#include <linux/jiffies.h>
#include <asm/uaccess.h>

/*MODULE_LICENSE("GPL");
MODULE_AUTHOR("dileep");
MODULE_DESCRIPTION(" tasklet  driver");
MODULE_VERSION("1.0.0");*/


#define SHARED_IRQ 19
static int irq = SHARED_IRQ, my_dev_id, irq_counter = 0;
module_param (irq, int, S_IRUGO);

/*static int my_tasklet_function(void);
static void my_tasklet_exit(void);*/

static void my_tasklet_handler(struct tasklet_struct *my_task);

DECLARE_TASKLET(my_tasklet,my_tasklet_handler);//my_tasklet is name,second handler function and third data

static int __init my_init(void);
static void __exit my_exit(void);

static irqreturn_t my_interrupt (int irq, void *dev_id);


static void my_tasklet_handler(struct tasklet_struct *my_task)
{
	//tasklet_disable(&my_tasklet);
	printk(KERN_INFO"My tasklet handler function");
	//tasklet_enable(&my_tasklet);
	irq_counter++;
	printk (KERN_INFO "In the ISR: counter = %d\n", irq_counter);
}

static irqreturn_t my_interrupt (int irq, void *dev_id)
{
   	tasklet_schedule(&my_tasklet);
    return IRQ_NONE;            /* we return IRQ_NONE because we are just observing */
}

static int __init my_init (void)
{
    if (request_irq(irq, my_interrupt, IRQF_SHARED, "my_interrupt", &my_dev_id))
        return -1;
    printk (KERN_INFO "Successfully loading ISR handler\n");
    return 0;
}

static void __exit my_exit (void)
{
    tasklet_kill(&my_tasklet);//terminate the tasket
    printk(KERN_INFO"My tasklet function exit");
    synchronize_irq (irq);//wait for any pendingIRQs for this interrupts
    free_irq (irq, &my_dev_id);//used to free the interrupt which we are requested for create
    printk (KERN_INFO "Successfully unloading \n");
}

module_init (my_init);
module_exit (my_exit);

MODULE_AUTHOR ("dileep");
MODULE_DESCRIPTION ("tasklet driver ");
MODULE_LICENSE ("GPL");




