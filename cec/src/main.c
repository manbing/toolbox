#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/slab.h>

MODULE_AUTHOR("Manbing <manbing3@gmail.com>");
MODULE_DESCRIPTION("current execution context");
MODULE_LICENSE("GPL");

void tasklet_callback(unsigned long data);
/* workqueue callback function */
static void wq_callback(struct work_struct *work);

struct tasklet_struct my_tasklet = {0};
struct work_struct *my_work = NULL;
static char test_data [] = "this is test data";

static void check_context(char *arg)
{
	printk("it is %s\n", arg);
	printk("the current interrupt context level is %hhu\n", interrupt_context_level());

	if (in_nmi())
		printk("current execution context is NMI (non-maskable interrupt). %lx", in_nmi());

	if (in_hardirq()) {
		printk("current execution context is hardirq (top half)\n");
		printk("irqs disabled, %lx, CSR = %d\n", in_hardirq(), irqs_disabled());
	}

	if (in_serving_softirq()) {
		printk("current execution context is softirq (bottom half)\n");
		printk("softirq disabled. %lx\n", in_serving_softirq());
	}

	if (in_task()) {
		printk("current execution context is task (porcess).\n");
	}

	if (IS_ENABLED(CONFIG_PREEMPT_COUNT)) {
		if (!(preempt_count() & PREEMPT_MASK)) {
			printk("preemption enabled.\n");
		} else {
			printk("preemption disabled.\n");
		}
	} else {
		printk("kernel does not support preemption\n");
	}
}

void tasklet_callback(unsigned long data)
{
        //char *input = (char *)data;
        
	//printk("input is [%s]\n", input);	
	check_context("tasklet");
}

void wq_callback(struct work_struct *work)
{
	check_context("workqueue");
        kfree(work);
}

static int __init cec_init(void)
{
        my_work = kzalloc(sizeof(struct work_struct), GFP_ATOMIC);
        INIT_WORK(my_work, wq_callback);
        /* Using linux kernel default work queue, system_wq. */
        schedule_work(my_work);
        
        tasklet_init(&my_tasklet, tasklet_callback, (unsigned long)test_data);
        tasklet_schedule(&my_tasklet);
        return 0;
}

static void __exit cec_exit(void)
{
        tasklet_kill(&my_tasklet);
}

module_init(cec_init);
module_exit(cec_exit);
