#include <linux/uaccess.h>//mem cpy from kernel userspace
#include <linux/init.h> //Moule stuff
#include <linux/proc_fs.h> //file system calls
#include <linux/module.h>//Module Stuff
#include <linux/kernel.h>



/*
HOW TO:
look at example code on canvas
example_code/02 helps A LOT
basically adapted it from there
website helps setting it up but code is kinda bad


*/

MODULE_LICENSE("Dual BSD/GPL");
//MODULE_AUTHOR("Diego Castro & Liz Parra");

#define BUF_LEN 100
//max length of read/write message (from slides)

static struct proc_dir_entry* proc_entry; //pointer to proc entry (from slies)
static char message[BUF_LEN]; //buffer to store read/write messages
static int procfs_buf_len; //variable to hold length of message
static int num_call = 0;
static int secs;
static int nsecs;

static ssize_t procfile_read(struct file* file, char * ubuf, size_t count, loff_t *ppos)
{
	/*
	timespec
	Member Objects
	tv_sec for seconds since epoch
	tv_nsec for nsecs since epoch
	we are making curr_time hold the current time
	*/
	struct timespec curr_time;
	long int elapsed_nsec;
	long int elapsed_secs;	
       //printk(KERN_INFO "proc_read\n");

        if(*ppos > 0 || count < procfs_buf_len)
	{
	//in this case message_lenght is the same as procfs_buf_len from procfs
                return 0;
	}
	curr_time = current_kernel_time();

        if(num_call < 1)//aka has not been called before
        {
                num_call = 1;
                sprintf(message, "current time: %ld.%ld\n",curr_time.tv_sec, curr_time.tv_nsec);
        }
       else
        {
		/*
		Case 1: same num of nsecs:
			just subtracts nsecs and secs
		Case 2: curr_nsec  is less than stored:
			subtract nsecs and secs
		Case 3: curr_nsec is more than stored:
			subtract  cur from stored and add a whole,
			// take one from seconds
		*/
                if(nsecs == curr_time.tv_nsec)
                {
                        elapsed_secs = curr_time.tv_sec - secs;
                        elapsed_nsec = curr_time.tv_nsec - nsecs;
                }
		if(nsecs < curr_time.tv_sec)
		{
			elapsed_secs = curr_time.tv_sec -secs;
			elapsed_nsec = curr_time.tv_sec -nsecs;
		}
                else
                {
                        elapsed_secs = curr_time.tv_sec - secs ;
                        elapsed_nsec = curr_time.tv_nsec - nsecs + 1000000000;
			elapsed_secs=elapsed_secs-1;
                }
                sprintf(message, "current time: %ld.%ld\nelapsed time: %ld.%ld\n",curr_time.tv_sec, curr_time.tv_nsec, elapsed_secs, elapsed_nsec);
		// sprintf(message, "elapsed time: %ld.%ld\n",elapsed_secs, elapsed_nsec);
	}
        secs = curr_time.tv_sec;
        nsecs = curr_time.tv_nsec;

        procfs_buf_len = strlen(message);
        if(copy_to_user(ubuf,message,procfs_buf_len))
                return -EFAULT;
        *ppos = procfs_buf_len;
        return procfs_buf_len;
	//look at the example code
	//this is OK ^^^ adapted from prof in example code
}


static ssize_t procfile_write(struct file* file, const char * ubuf, size_t count, loff_t* ppos)
{
	//printk(KERN_INFO "proc_write\n"); 
	//the example get data from user we dont need to do that

}
//Dont thik we really need this bc were not writing

static struct file_operations procfile_fops = {
        .owner = THIS_MODULE,
        .read = procfile_read,
        .write = procfile_write,
};//the syntax for this is a lillt werid
//for clarification go to example code/2 procfs


static int timer_init(void) //basically got this from hello_init
{
        proc_entry = proc_create("timer", 0660, NULL, &procfile_fops);
        if(proc_entry == NULL)
                return -ENOMEM;
        return 0;
}

static void timer_exit(void) 
{
        proc_remove(proc_entry);
        return;

}
//literally the same as hello_exit...should be ok

module_init(timer_init);

module_exit(timer_exit);
