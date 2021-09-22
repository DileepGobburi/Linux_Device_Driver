#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kthread.h> // therad 
#include <linux/delay.h>   // sleep 
//#incluinux/sched.h>               //task_struct
#include <linux/mutex.h>  
MODULE_LICENSE("GPL");


static atomic_t	rds_ib_allocation = ATOMIC_INIT(0);
static struct task_struct *char_thread;
static struct task_struct *char_thread1;

unsigned int mydata =0;
int my_thread_fun(void *p);
struct mutex my_mutex;


int my_thread_fun(void *p)
{
	while(!kthread_should_stop())
	{		
		mutex_lock(&my_mutex);
		mydata++;
		printk(KERN_INFO "my_thread_fun  %lu\n", mydata);
		mutex_unlock(&my_mutex);
		msleep(500);
	}
	return 0;	
}

int my_thread_fun1(void *p)
{
	while(!kthread_should_stop())
	{
		mutex_lock(&my_mutex);
		mydata++;
		printk(KERN_INFO "my_thread_fun2  %lu\n", mydata);
		mutex_unlock(&my_mutex);
		msleep(500);
	}
	return 0;
}


static int __init my_simpledriver_init(void)
{
	printk(KERN_INFO"My sample driver start");

	mutex_init(&my_mutex); // Mutex init 

	char_thread = kthread_run(my_thread_fun,NULL,"my char thread");
	if(char_thread)
	{
		printk(KERN_INFO" create the thread");
	}
	else 
	{
		printk(KERN_INFO"unable to create the thread");
		
	}
	char_thread1 = kthread_run(my_thread_fun1,NULL,"my char thread1");
	if(char_thread1)
	{
		printk(KERN_INFO" create the thread1");
	}
	else 
	{
		printk(KERN_INFO"unable to create the thread");
		
	}
	return 0;

}


static void __exit my_simpledriver_exit(void)
{
	printk(KERN_INFO"My samle driver exit\n");

	kthread_stop(char_thread);
	kthread_stop(char_thread1);

}
module_init(my_simpledriver_init);
module_exit(my_simpledriver_exit);




