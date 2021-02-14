#include <types.h>
#include <syscall.h>
#include <lib.h>
#include <proc.h>
#include <thread.h>
#include <addrspace.h>
#include <current.h>
#include <synch.h>
#include <kern/unistd.h>
#include <kern/errno.h>
#include <mips/trapframe.h>

#include "opt-waitpid.h"
#include "opt-fork.h"

void sys__exit(int status) {
  (void) status;  /* todo handle status */

#if OPT_WAITPID
  struct proc *proc = curproc;
  struct thread *thread = curthread;


  lock_acquire(proc->p_lk);

  /* save proc status */
  proc->p_status = status;
  /* detach thread from process */
  proc_remthread(thread);
  KASSERT(thread->t_proc == NULL);


  /* signal exit */
  cv_signal(proc->p_cv, proc->p_lk);

  lock_release(proc->p_lk);
#else 
  /* get as of current process */
  struct addrspace *as = proc_getas(); 
  /* destroy as */
  as_destroy(as);
#endif

  /* thread exits */
  thread_exit();

  panic("thread_exit returned (should not happen)\n");
}


#if OPT_WAITPID
pid_t
sys_getpid(void) {
  return curproc->p_pid;
}

pid_t 
sys_waitpid(pid_t pid, int status, int options) {

  int s;

  struct proc *proc = proc_get_by_pid(pid);

  s = proc_wait(proc);

  (void)s;

  (void)status;
  (void)options;

  return pid;
}

#if OPT_FORK
static void 
call_enter_forked_process(void *tfv, unsigned long dummy) {
  struct trapframe *tf = (struct trapframe *) tfv;
  (void) dummy;
  enter_forked_process(tf);

  panic("enter_forked_process returned (should not happen)\n");
}

int
sys_fork(struct trapframe *ctf, pid_t *retval) {
  struct trapframe *tf_child;
  struct proc *newp;
  int result;

  KASSERT(curproc != NULL);

  newp = proc_create_runprogram(curproc->p_name);
  if (newp == NULL)
    return ENOMEM;

  /*
   * we need to duplicate the address space 
   * of the current process 
   */
  as_copy(curproc->p_addrspace, &(newp->p_addrspace));
  if (newp->p_addrspace) {
    proc_destroy(newp);
    return ENOMEM;
  }

  /*
   * we need a copy of the parent's trapframe
   */
  tf_child = kmalloc(sizeof(struct trapframe));
  if (tf_child == NULL) {
    proc_destroy(newp);
    return ENOMEM;
  }
  memcpy(tf_child, ctf, sizeof(struct trapframe));

  /*
   * TODO: linking parent/child
   * so that child terminated on parent exit
   */

  result = thread_fork(curthread->t_name, 
		       newp, 
		       call_enter_forked_process,
		       (void *)tf_child,
		       (unsigned long)0); /* unused */

  if (result) {
    proc_destroy(newp);
    kfree(tf_child);
    return ENOMEM;
  }

  *retval = newp->p_pid;

  return 0;
}
#endif
#endif
