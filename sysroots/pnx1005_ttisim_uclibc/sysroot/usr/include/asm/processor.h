#ifndef _ASM_TRIMEDIA_PROCESSOR_H
#define _ASM_TRIMEDIA_PROCESSOR_H

/*
 * Default implementation of macro that returns current
 * instruction pointer ("program counter").
 * FIXME: I don't know how to implement it in TriMedia
 */
#define current_text_addr() ({ __label__ _l; _l: &&_l;})

#include <linux/config.h>
#include <linux/threads.h>
#include <asm/types.h>
#include <asm/ptrace.h>
#include <asm/current.h>
#include <asm/atomic.h>

#ifdef CONFIG_DYNLOAD
#include <tmlib/DynamicLoader.h>
#endif 

/*
 * User space process size: 3.75GB. This is hardcoded into a few places,
 * so don't change it unless you know what you are doing.
 * FIXME: I don't know whether or not it is suitable
 */
#define TASK_SIZE	(0xF0000000UL)

/*
 * This decides where the kernel will search for a free chunk of vm
 * space during mmap's. We won't be using it
 * FIXME: I don't know whether or not it is suitable
 */
#define TASK_UNMAPPED_BASE	0

#ifdef CONFIG_DYNLOAD
struct dynload_module
{
	atomic_t count;
	DynLoad_Code_Segment_Handle module;
};

extern struct kmem_cache_t* dynload_module_cache;

static inline void 
get_dynload_module(struct dynload_module* module)
{
	if(module){
		atomic_inc(&module->count);
	}
}
#endif 

struct thread_struct
{
#ifdef CONFIG_DYNLOAD
	/*
	 *if dynmodule==NULL, the thread was loaded statically,
	 *if dynmodule!=NULL, the thread was loaded dynamiclly,
	 */
	struct dynload_module* dynmodule;
#else
	unsigned long dummy;
#endif 
};

#define INIT_THREAD  { NULL }

/*
 * Do necessary setup to start up a newly executed thread.
 *
 * pass the data segment into user programs if it exists,
 * it can't hurt anything as far as I can tell
 */
extern void start_thread(struct pt_regs* regs,unsigned long pc,unsigned long usp);

/* Forward declaration, a strange C thing */
struct task_struct;

/* Free all resources held by a thread. */
extern void release_thread(struct task_struct *dead_task);

/* Prepare to copy thread state - unlazy all lazy status */
#define prepare_to_copy(tsk)	do { } while (0)

extern int kernel_thread(int (*fn)(void *), void * arg, unsigned long flags);

/*
 * Free current thread data structures etc..
 */
extern void exit_thread(void);

/*
 *FIXME: I don't know how to implement it.
 */
unsigned long thread_saved_pc(struct task_struct *tsk);
unsigned long get_wchan(struct task_struct *p);

/*
 *FIXME: I don't know whether or not it is suitable
 */
#define cpu_relax()    do { } while (0)

#endif

