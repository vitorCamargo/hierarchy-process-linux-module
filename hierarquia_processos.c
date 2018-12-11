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
MODULE_AUTHOR("Thais Zoraski && Vitor Bueno");
MODULE_DESCRIPTION("Módulo do Linux para hierarquia e contagem de processos");
MODULE_VERSION("1.4.8");

size_t process_counter = 0;

void verificaFilhos(struct task_struct *task, int hierarquia) {
    struct task_struct *filho;
    size_t process_filho_counter = 0;
    int aux = hierarquia;

    printk(KERN_NOTICE "%*s--> (%s) [%d]\n", (4*hierarquia), "", task->comm, task->pid);

    for_each_process(filho) {
        if(task->pid == filho->parent->tgid) {
            ++process_filho_counter;
            verificaFilhos(filho, (hierarquia+1));
        }
    }
    if(process_filho_counter > 0) printk(KERN_NOTICE "%*sPROCESSO %s [%d]: qtd filhos: %zu\n\n", (4*aux), "", task->comm, task->pid, process_filho_counter);
}

static int __init ex_init(void) {
    struct task_struct *task;
    printk(KERN_NOTICE "--> (COMM) [PID DO PROCESSO]\n\n");

    for_each_process(task) {
        if(task->parent->tgid == 0) verificaFilhos(task, 0);
        ++process_counter;
    }
    printk(KERN_NOTICE "== Número total de processos: %zu\n\n\n\n\n", process_counter);

    return 0;
}

static void __exit ex_fini(void) {
    printk(KERN_NOTICE "=== Obrigado por usar, módulo desenvolvido por Thais e Vitor ===");
}

module_init(ex_init);
module_exit(ex_fini);
