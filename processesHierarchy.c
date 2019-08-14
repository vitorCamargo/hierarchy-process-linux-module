#include <linux/sched/signal.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/cred.h>
#include <linux/pid.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Thais Zorawski && Vitor Bueno");
MODULE_DESCRIPTION("Linux module for hierarchy and process counting");
MODULE_VERSION("1.4.8");

size_t process_counter = 0;

void verifyChildren(struct task_struct *task, int hierarchy) {
    struct task_struct *child;
    size_t child_process_counter = 0;
    int aux = hierarchy;

    printk(KERN_NOTICE "%*s--> (%s) [%d]\n", (4 * hierarchy), "", task->comm, task->pid);

    for_each_process(child) {
        if(task->pid == child->parent->tgid) {
            ++child_process_counter;
            verifyChildren(child, (hierarchy + 1));
        }
    }
    if(child_process_counter > 0) printk(KERN_NOTICE "%*sPROCESS %s [%d]: number of children: %zu\n\n", (4 * aux), "", task->comm, task->pid, child_process_counter);
}

static int __init ex_init(void) {
    struct task_struct *task;
    printk(KERN_NOTICE "--> (COMM) [PROCESS' PID]\n\n");

    for_each_process(task) {
        if(task->parent->tgid == 0) verifyChildren(task, 0);
        ++process_counter;
    }
    printk(KERN_NOTICE "== Total number of processes: %zu\n\n\n\n\n", process_counter);

    return 0;
}

static void __exit ex_fini(void) {
    printk(KERN_NOTICE "=== Thanks for using, module developed by Thais and Vitor ===");
}

module_init(ex_init);
module_exit(ex_fini);
