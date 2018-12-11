#include <linux/sched/signal.h>
#include <linux/memcontrol.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/cred.h>
#include <linux/pid.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Thais Zoraski && Vitor Bueno");
MODULE_DESCRIPTION("Módulo do Linux para hierarquia e contagem de processos");
MODULE_VERSION("1.4.8");

void verificaFilhos(struct task_struct *task, int hierarquia) {
    struct task_struct *filho;
    size_t process_filho_counter = 0;
    int aux = hierarquia;

    printk(KERN_NOTICE "%*s--> (%s) [%d]\n", 2*hierarquia, "", task->comm, task->pid);

    for_each_process(filho) {
        if(task->pid == filho->real_parent->pid) {
            verificaFilhos(filho, ++hierarquia);
            ++process_filho_counter;
        }
    }
    printk(KERN_NOTICE "%*sPROCESSO %s: qtd filhos: %zu\n\n", 2*aux, "", task->comm, process_filho_counter);
}

static int __init ex_init(void) {
    struct task_struct *task;
    size_t process_counter = 0;
    printk(KERN_NOTICE "NUM_HIERA --> (COMM) [PID]\n\n");    

    for_each_process(task) {
        verificaFilhos(task, 0);
        ++process_counter;
    }
    printk(KERN_NOTICE "== Number of process: %zu\n\n\n\n\n", process_counter);

    return 0;
}

static void __exit ex_fini(void) { }

module_init(ex_init);
module_exit(ex_fini);
