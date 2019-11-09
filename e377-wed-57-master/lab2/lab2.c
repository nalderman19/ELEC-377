/*+
 * Module:  lab2.c
 *
 * Purpose: Skeleton solution to ELEC 377 Lab2.
 *
-*/

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>

// defines constant for NR_THREADS location in memory
#define NR_THREADS 0xc038b3a8

static struct task_struct *theTask, *lastTask;

int cnt;

int my_read_proc(char * page, char **start, off_t fpos, int blen, int * eof, void * data){
    int numChars = 0;
    // First process case, formats table and assigns pointer variable for nr_threads
    if (fpos == 0){
        int* nr_threads = NR_THREADS;
        // Generates buffer to format table information
        numChars += sprintf(page, "Number of running processes: %d\n", nr_running);
	     numChars += sprintf(page + numChars, "Number of running threads: %d\n", *nr_threads);
	     numChars += sprintf(page + numChars, "PID\tUID\tNICE\n");
	     // First Task
	     theTask = &init_task;
	     lastTask = theTask;
         // If statement ensures PID is not 0, or the 'swapper'
	     if (theTask -> pid != 0){
             // adds proc information to buffer for first case
	         numChars += sprintf(page + numChars, "%d\t%d\t%d\n", theTask->pid, theTask->uid, theTask->nice);
	     }
         // Move to next_task
	     lastTask = lastTask->next_task;
    } else {
        // Checks if we have looped back around to the first proc entry
        if (lastTask == theTask){
            *eof = 0;
            *start = page;
            return 0;
        }
        // Adds each proc entry information to buffer
        if (lastTask -> pid != 0){
            numChars += sprintf(page, "%d\t%d\t%d\n", lastTask->pid, lastTask->uid, lastTask->nice);
        }
        lastTask = lastTask->next_task;
    }
    *eof = 1;
    *start = page;
    return numChars;
}

// Initialization module creates a proc entry in the kernel, function is called by default
int init_module(){
   struct proc_dir_entry * proc_entry;
   // creates a proc entry with a file permission of read only for user, group and owner
   proc_entry = create_proc_entry("lab2", 0444, NULL);
   // If proc entry can't be created properly then remove
   if (proc_entry == NULL) {
        remove_proc_entry("lab2", &proc_root);
        printk(KERN_ALERT "Error: Could not initialize /proc/lab2\n");
        return -ENOMEM;
   }
   // Attach callback function to proc entry
   proc_entry->read_proc = my_read_proc;
   return 0;
}

// Cleanup module removes proc entry from the kernel
void cleanup_module(){
    remove_proc_entry("lab2", &proc_root);
}
