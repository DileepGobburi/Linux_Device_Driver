#include <linux/kernel.h>	// kernel header
#include <linux/kthread.h>	// kthread header
#include <linux/delay.h>	// sleep 
#include <linux/mutex.h>	// mutex header
#include <linux/module.h>


MODULE_AUTHOR("vinay");
MODULE_LICENSE("GPL");

int data = 0;
static struct task_struct *char_thread;
static struct task_struct *char_thread1;
struct mutex my_mutex;

int my_thread1(void *p){
        int locked;
	while(!(kthread_should_stop())){
	locked = mutex_trylock(&my_mutex);
	pr_info("Thread1 try to lock using mutex_trylock %d\n",locked);
	printk(KERN_INFO"Mutex(my_thread1) locked status : %d\n",mutex_is_locked(&my_mutex));
	printk(KERN_INFO"my_thread1 invoked data = %d\n",++data);
        //if(locked)
        
	mutex_unlock(&my_mutex);
	printk(KERN_INFO"Mutex(my_thread1) locked status : %d\n",mutex_is_locked(&my_mutex));
	
	msleep(1000);
	}
	return 0;
}

int my_thread2(void *p){
        int locked;
	while(!(kthread_should_stop())){
	  mutex_lock(&my_mutex);
	//pr_info("Thread2 try to lock using mutex_trylock %d\n",locked);
	printk(KERN_INFO"Mutex(my_thread2) locked status : %d\n",mutex_is_locked(&my_mutex));
	printk(KERN_INFO"my_thread2 invoked data = %d\n",++data);
	//if(locked)
       // msleep(5000);
        mutex_unlock(&my_mutex);
	printk(KERN_INFO"Mutex(my_thread2) locked status : %d\n",mutex_is_locked(&my_mutex));
	
	msleep(1000);
	}
	return 0;
}

static int __init thread_func_start(void){

	printk(KERN_INFO"Sample thread driver started..\n");
	
	mutex_init(&my_mutex);

	char_thread = kthread_run(my_thread1,NULL,"my char thread");
	if(char_thread<0)
		printk(KERN_INFO"Unable to create and wake up thread\n");
	else
		printk(KERN_INFO"Thread Created and running\n");
		
	char_thread1 = kthread_run(my_thread2,NULL,"my char thread1");
	if(char_thread1<0)
		printk(KERN_INFO"Unable to create and wake up thread\n");
	else
		printk(KERN_INFO"Thread Created and running\n");
	return 0;
}


static void __exit thread_func_end(void){
	kthread_stop(char_thread);
	kthread_stop(char_thread1);
	printk(KERN_INFO"Sample thread driver exited..\n");
}


module_init(thread_func_start);
module_exit(thread_func_end);
