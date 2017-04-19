#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <asm/uaccess.h>
#include <linux/fs.h> 
#include <linux/file.h>
#include <linux/cdev.h>
#include <linux/proc_fs.h>
#include <linux/pid.h>
#include <linux/rcupdate.h>
#include <linux/fdtable.h>
#include <linux/fs_struct.h>
#include <linux/dcache.h>
#include <linux/slab.h>

#define next_task(p) \
list_entry_rcu((p)->tasks.next, struct task_struct, tasks)

#define for_each_process(p) \
for (p = &init_task ; (p = next_task(p)) != &init_task ; )

	MODULE_LICENSE("GPL");
MODULE_AUTHOR("Serhan GURSOY");
MODULE_DESCRIPTION("CS342 PROJECT 4 KERNEL MODULE. IT DOES REALLY TRICKY JOBS. REALLY. SO, WATCH OUT FOR ITS OUTPUTS.");

short isDebug = 0;
int input_pid = -1;
module_param(input_pid, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

static int hello_init(void)
{
	printk(KERN_INFO "************ BEGINNING OF MODULE ************\n");

	if (isDebug) {
		printk(KERN_WARNING "Input PID is %i\n", input_pid);
		printk("%s: The calling process is \"%s\" (pid %i)\n",__FUNCTION__, current->comm, current->pid);
	}

	struct task_struct *temp = current;
	short checkerByte = 0;
	for_each_process(temp) {
		if (temp->pid == input_pid) {

			printk(KERN_WARNING "This process's name is %s\nParent process's name is %s  with pid %i\n", temp->comm, temp->parent->comm, temp->parent->pid);

			printk(KERN_WARNING "|>%s pid:%i\n", temp->parent->comm, temp->parent->pid);

	 		struct list_head *p; //Pointer
	 		struct task_struct child_task;
	 		list_for_each(p, &(temp->parent->children)){
	 			child_task = *list_entry(p, struct task_struct, sibling);
	 			if (child_task.pid != input_pid)
	 				printk("      |-->%s pid:%d\n",child_task.comm, child_task.pid);
	 			else{
	 				printk("      |-->%s pid:%d\n",child_task.comm, child_task.pid);
	 				struct list_head *p2; //Pointer
	 				struct task_struct child_task2;
	 				list_for_each(p2, &(temp->children)){
	 					child_task2 = *list_entry(p2, struct task_struct, sibling);
	 					if (child_task2.pid != input_pid)
	 						printk("          |-->%s pid:%d\n",child_task2.comm, child_task2.pid);
	 				}
	 			}
	 		}

	 		checkerByte = 1;


	 		struct mm_struct *m;
	 		struct vm_area_struct *v;
	 		m = temp->mm;
	 		v = m->mmap;
	 		long totalSize = 0;

	 		while(v != NULL) {
	 			printk("VM_START: %ld, VM_END: %ld\n", v->vm_start, v->vm_end);
	 			totalSize += v->vm_end - v->vm_start;
	 			v = v->vm_next;
	 		}

	 		printk("Total VM size, %ld byte\n", totalSize);

	 		
// This part goes into kernel panic. Yeah, you heard it right, I said kernel panic.
/*
	 	struct files_struct *files_task;
	 	struct fdtable *file_t;

	 	files_task = temp->files;
	 	file_t = files_fdtable(files_task);
	 	int count = 0;
	 	while(file_t->fd[count] != NULL) {
	 		struct path file_path = file_t->fd[count]->f_path;
	 		char *path_str = d_path(&file_path,(char*)kmalloc(GFP_KERNEL,100*sizeof(char)),100*sizeof(char));
	 		printk("File name is %s\n", path_str);
	 		count++;
	 	}
*/
	 	}
	 }

	 if (checkerByte==0)
	 	printk(KERN_ERR "Couldn't find process with pid %i, are you sure it's exists?\n", input_pid);

	 return 0;
	}

	static void hello_exit(void)
	{
		printk(KERN_INFO "************ END OF MODULE ************\n");
	}


	module_init(hello_init);
	module_exit(hello_exit);
