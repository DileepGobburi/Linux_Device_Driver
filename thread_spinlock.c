#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/spinlock.h>
MODULE_LICENSE("GPL");

static struct task_struct *char_thread;
static struct task_struct *char_thread1;
unsigned int my_data=100;
int thread(void *p);
int thread1(void *p);
spinlock_t etx_spinlock;

int thread(void *p)
{	while(!kthread_should_stop())
	{
	spin_lock(&etx_spinlock);
	my_data++;
	printk(KERN_INFO"thread data %lu\n",my_data);
	spin_unlock(&etx_spinlock);
	msleep(1000);
	}
	return 0;
}
int thread1(void *p)
{	while(!kthread_should_stop())
	{
	spin_lock(&etx_spinlock);
	my_data--;
	printk(KERN_INFO"thread1 data %lu\n",my_data);
	spin_unlock(&etx_spinlock);
	msleep(1000);
	}
	return 0;
}

static int __init my_spinlock_init(void)
{
	spin_lock_init(&etx_spinlock);
	
	printk(KERN_INFO"my driver starts");
	char_thread=kthread_run(thread,NULL,"my thread");
	if(char_thread)
	{
		printk(KERN_INFO"thread is created");
	}
	else
	{
		printk(KERN_INFO"thread unable to create");
	}
	char_thread1=kthread_run(thread1,NULL,"my thread1");
	if(char_thread1)
	{
		printk(KERN_INFO"thread1 is created");
	}
	else
	{
		printk(KERN_INFO"thread1 unable to create");
	}
	
	return 0;
}

static void __exit my_spinlock_exit(void)
{
	//printk(KERN_INFO"my driver is removed");
	kthread_stop(char_thread);
	kthread_stop(char_thread1);
	printk(KERN_INFO"my driver is removed\n");
}
	

	
module_init(my_spinlock_init);
module_exit(my_spinlock_exit);

