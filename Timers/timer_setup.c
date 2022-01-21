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

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Dileep");
MODULE_DESCRIPTION(" major and minor  driver");
MODULE_VERSION("1.0.0");

#define TIMEOUT 5000
static struct timer_list my_timer;
static unsigned int count = 0;
unsigned long js,je,tet;

void mytimer_fun(struct timer_list *data )
{
	printk(KERN_INFO"my timer  function %d\n",count++);
	
	//reset up timer interval based up on  timeout macro
	// modify the timer for next time
	//getconf CLK_TCK ==> 100;HZ = no. ticks/sec
	// jiffies ==> On kernel boot-up, jiffies is initialized to a special initial value, and it is incremented by one for each timer interrupt.
	//jiffies = no. of ticks
	//seconds to unit jiffies: seconds * HZ
	//jiffies to seconds:jiffies / HZ
	//unsigned long now_tick = jiffies ; // now
	//unsigned long next_tick = jiffies + 1 ; // one tick from now
	//unsigned long timer_later = jiffies + (10*HZ) ; // 10s from now
	//unsigned long time_fraction = jiffies + (HZ/10) ; // 0.1s from now
	mod_timer(&my_timer, jiffies + HZ*5);
	je=jiffies;
	printk("\n[jiffies end time: %lu]\nmodule removed \n",je);
       tet=je-js;
       printk("\n end time[%lu] - start time [%lu]  \n total elapsed time [&lu]\n",js,je,tet); 
}

static int __init my_driver_init(void)
{
	printk(KERN_INFO "kernel timer driver init  ... done \n");
	timer_setup(&my_timer,mytimer_fun,0);
	
	js=jiffies;
	printk("\n[jiffies start time: %lu]\nmodule started \n",js);
	//setup timer interval based up on  timeout macro
	mod_timer(&my_timer,jiffies+HZ*5);
	
	return 0;
}

static void __exit my_driver_exit(void)
{	
	printk(KERN_INFO "kernel driver removed  ... done \n");
	/* remove kernel timer when unloading module */
 
    del_timer(&my_timer);
}
module_init(my_driver_init);
module_exit(my_driver_exit);
