#include <linux/linkage.h>
#include <linux/kernel.h>
#include <linux/unistd.h>
#include <linux/sched.h>

asmlinkage long sys_get_task_info(void)
{
	struct task_struct *task = current;
	printk("PID: %d\n", task->pid);
	printk("TGID: %d\n", task->tgid);
	printk("STATE: 0x%x\n", task->state);
	printk("PRIORITY: %d\n", task->rt_priority);
	printk("FLAGS: 0x%x\n", task->flags);
	printk("NR_DIRTIED: %d\n", task->nr_dirtied);
}
