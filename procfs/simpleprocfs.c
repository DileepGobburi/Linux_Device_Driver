#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/kernel.h>   
#include <linux/proc_fs.h>
#include<linux/uaccess.h> 

#define BUFSIZE  100

char my_array[BUFSIZE]="initial data in array";
static int len = 1;

/*
	proc_dir_entry	adds file or directory entries to the /proc file system. They associate processing routines 
	with each node of the /proc tree
*/
static struct proc_dir_entry *parent;

static int my_proc_open(struct inode *inode, struct file *file)
{
    pr_info("proc file opend.....\t");
    return 0;
}
/*
** This function will be called when we close the procfs file
*/
static int my_proc_close(struct inode *inode, struct file *file)
{
    pr_info("proc file released.....\n");
    return 0;
}
/*
** This function will be called when we read the procfs file
*/
static ssize_t my_proc_read(struct file *filp, char __user *buffer, size_t length,loff_t * offset)
{
    pr_info("proc file read.....\n");
   if(len)
    {
        len=0;
    }
    else
    {
        len=1;
        return 0;
    }
   
    if( copy_to_user(buffer,my_array,100) )
    {
        pr_err("Data Send : Err!\n");
    }
 
    return length;;
}
/*
** This function will be called when we write the procfs file
*/
static ssize_t my_proc_write(struct file *filp, const char *buff, size_t len, loff_t * off)
{
    pr_info("proc file wrote.....\n");
    
    if( copy_from_user(my_array,buff,len) )
    {
        pr_err("Data Write : Err!\n");
    }
    
    return len;
}

/*
static struct file_operations myProcOps = 
{
	.owner = THIS_MODULE,
	.open = my_proc_open,
	.release = my_proc_close,
	.read = my_proc_read,
	.write = my_proc_write,
};*/
static struct proc_ops myProcOps = 
{
	.owner = THIS_MODULE,
	.proc_open  	= my_proc_open,
	.proc_release 	= my_proc_close,
	.proc_read 	= my_proc_read,
	.proc_write 	= my_proc_write,


};
static int __init my_proc_init(void)
{
	proc_create("my_proc",0660,NULL,&myProcOps);
	return 0;
}

static void __exit my_proc_exit(void)
{
	remove_proc_entry("my_proc",NULL);
}
module_init(my_proc_init);
module_exit(my_proc_exit);

