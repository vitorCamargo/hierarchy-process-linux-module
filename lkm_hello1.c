#include <linux/module.h>
#include <linux/printk.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sched/signal.h>
#include <linux/sched.h>
#include <linux/memcontrol.h>

#include <linux/list.h>   
#include <linux/cred.h>     
#include <linux/pid.h>

static int __init ex_init(void)
{
    struct task_struct *task, *kid_task;
    size_t process_counter = 0, kid_process_counter = 0;

    for_each_process(task) {
        kid_process_counter = 0;

        for_each_process(kid_task) {
            if(task->pid == kid_task->real_parent->pid) { ++kid_process_counter; }
        }

        pr_info("--PAI (%s) [%d] - qtd filhos: %zu\n", task->comm, task->pid, kid_process_counter);
        ++process_counter;
    }
    printk(KERN_INFO "== Number of process: %zu\n\n\n\n\n", process_counter);

    return 0;
}

static void __exit ex_fini(void) {

}

module_init(ex_init);
module_exit(ex_fini);

MODULE_LICENSE("GPL");
