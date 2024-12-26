# Current Execution Context (CEC)

## Introduction

It has four execution context in Linux kernel. Non-Maskable interrupt (NMI context), interrupt top half (hardirq context), 
interrupt bottom half (softirq context) and task (normal context).

In NMI context, although NMIs can trigger when interrupts are disabled and even when the CPU is processing a normal interrupt,
there is a specific time when an NMI will not trigger: when the CPU is processing another NMI. On most architectures,
the CPU will not process a second NMI until the first NMI has finished. When a NMI triggers and calls the NMI handler,
new NMIs must wait till the handler of the first NMI has completed. NMI handlers do not need to worry about nesting, and the
Linux NMI handlers are written with this fact in mind. [1][3]

In top half context, hardirq is disable.

In bottom half context, bottom half is disable, does not allow sleepable/blockable function also. E.g.,
`kmalloc()`, `netlink_broadcast()`, `kobject_uevent_env()` and so on.

User process and kernel process run in normal context.

## Implementing

Linux kernel (linux-5.15.167) stores this information in field, preempt_count, of struct, thread_info (`current_thread_info()->preempt_count`).
preempt_count field consists of `PREEMPT`, `SOFTIRQ`, `HARDIRQ`, `NMI` and `PREEMPT_NEED_RESCHED`. `PREEMPT` occupies 8 bits; `SOFTIRQ` occupies
8 bits, `HARDIRQ` occupies 4 bits, `NMI` occupies 4 bits and `PREEMPT_NEED_RESCHED` occupies 1 bit:

* PREEMPT_MASK:			0x000000ff
* SOFTIRQ_MASK:			0x0000ff00
* HARDIRQ_MASK:			0x000f0000
* NMI_MASK:        		0x00f00000
* PREEMPT_NEED_RESCHED:		0x80000000

If `PREEMPT` is not 0, it means current thread is not preemptable. In a non-preemptive kernel, `schedule()` is
called when returning to userspace (and wherever a system call blocks, also on the idle task). In a preemptive kernel,
`schedule()` is also called when returning from any interrupt, and also in a few other places, e,g,. on `mutex_unlock()`
slow path, on certain conditions while receiveing network packets and so on [2].


## Conclusion 

Linux kernel had provided convenient API to get current execution context.
Developer should figure out where the code go, before modification.

[1] [Interrupts and Interrupt Handling. Part 6.](https://0xax.gitbooks.io/linux-insides/content/Interrupts/linux-interrupts-6.html)

[2] [Preemptive](https://stackoverflow.com/questions/40204506/what-is-the-difference-between-nonpreemptive-and-preemptive-kernels-when-switch)

[3] [The x86 NMI iret problem](https://lwn.net/Articles/484932/)
