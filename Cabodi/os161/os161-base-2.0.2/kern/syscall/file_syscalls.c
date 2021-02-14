#include <types.h>
#include <kern/unistd.h>
#include <kern/errno.h>
#include <clock.h>
#include <syscall.h>
#include <current.h>
#include <lib.h>

#if OPT_FILESYSTEM

#include <copyinout.h>
#include <vnode.h>
#include <vfs.h>
#include <limits.h>
#include <uio.h>
#include <proc.h>

/* max num of syste wide open files */
#define SYSTEM_OPEN_MAX (10*OPEN_MAX)

#define USE_KERNEL_BUFFER 0

/* system open file table */
struct openfile {
  struct vnode *vn;
  off_t offset;
  unsigned int countRef;
};

struct openfile systemFileTable[SYSTEM_OPEN_MAX];

void openfileIncrRefCount(struct openfile *of) {
  if (of != NULL)
    of->countRef++;
}

#if USE_KERNEL_BUFFER

static int
file_read(int fd, userptr_t buf_ptr,size_t size) {
  struct iovec iov;
  struct uio ku;
  int result, nread;
  struct vnode *vn;
  struct openfile *of;
  void *kbuf;

  if (fd<0||fd>OPEN_MAX) return -1;
  of = curproc->fileTable[fd];
  if (of == NULL) return -1;
  vn = of->vn;
  if (vn == NULL) return -1;

  kbuf = kmalloc(size);
  uio_kinit(&iov, &ku, kbuf, size, of->offset, UIO_READ);
  result = VOP_READ(vn, &ku);

  if (result)
    return result;

  of->offset = ku.uio_offset;
  nread = size - ku.uio_resid;
  copyout(kbuf, buf_ptr, nread);
  kfree(kbuf);
  return nread;
} 

static int
file_write(int fd, userptr_t buf_ptr, size_t size) {
  struct iovec iov;
  struct uio ku;
  int result, nwrite;
  struct vnode *vn;
  struct openfile *of;
  void *kbuf;

  if (fd<0||fd>OPEN_MAX) return -1;
  of = curproc->fileTable[fd];
  if (of ==NULL) return -1;
  vn = of->vn;
  if (vn == NULL) return -1;

  kbuf = kmalloc(size);
  copyin(buf_ptr, kbuf, size);
  uio_kinit(&iov, &ku, kbuf, size, of->offset, UIO_WRITE);
  result = VOP_WRITE(vn, &ku);

  if (result)
    return result;

  kfree(kbuf);
  of->offset = ku.uio_offset;
  nwrite = size - ku.uio_resid;
  return nwrite;
}

#else /* USE_KERNEL_BUFFER */

static int
file_read(int fd, userptr_t buf_ptr,size_t size) {
  struct iovec iov;
  struct uio u;
  int result, nread;
  struct vnode *vn;
  struct openfile *of;

  if (fd<0||fd>OPEN_MAX) return -1;
  of = curproc->fileTable[fd];
  if (of == NULL) return -1;
  vn = of->vn;
  if (vn == NULL) return -1;

  iov.iov_ubase = buf_ptr;
  iov.iov_len = size;

  u.uio_iov = &iov;
  u.uio_iovcnt = 1;
  u.uio_resid = size;
  u.uio_offset = of->offset;
  u.uio_segflg = UIO_USERISPACE;
  u.uio_rw = UIO_READ;
  u.uio_space = curproc->p_addrspace;

  result = VOP_READ(vn, &u);

  if (result)
    return result;

  of->offset = u.uio_offset;
  nread = size - u.uio_resid;
  return nread;
}
 

static int
file_write(int fd, userptr_t buf_ptr, size_t size) {
  struct iovec iov;
  struct uio u;
  int result, nwrite;
  struct vnode *vn;
  struct openfile *of;

  if (fd<0||fd>OPEN_MAX) return -1;
  of = curproc->fileTable[fd];
  if (of ==NULL) return -1;
  vn = of->vn;
  if (vn == NULL) return -1;

  iov.iov_ubase = buf_ptr;
  iov.iov_len = size;

  u.uio_iov = &iov;
  u.uio_iovcnt = 1;
  u.uio_resid = size;
  u.uio_offset = of->offset;
  u.uio_segflg = UIO_USERISPACE;
  u.uio_rw = UIO_WRITE;
  u.uio_space = curproc->p_addrspace;


  result = VOP_WRITE(vn, &u);

  if (result)
    return result;

  of->offset = u.uio_offset;
  nwrite = size - u.uio_resid;
  return nwrite;
}
#endif /* USE_KERNEL_BUFFER */

int
sys_open(userptr_t path, int openflags, mode_t mode, int *errp) {
  int fd, i;
  struct vnode *v;
  struct openfile *of = NULL;
  int result;

  result = vfs_open((char*)path, openflags, mode, &v);
  if (result) {
    *errp = ENOENT;
    return -1;
  }

  /* search system open file table */
  for (i = 0; i < SYSTEM_OPEN_MAX; i++) {
    if (systemFileTable[i].vn == NULL) {
      of = &systemFileTable[i];
      of->vn = v;
      of->offset = 0;
      of->countRef = 1;
      break;
    }
  }

  if (of == NULL) {
    /* no free slot in system open file table */
    *errp = ENFILE;
  } else {
    for (fd = STDERR_FILENO + 1; fd < OPEN_MAX; i++) {
      if (curproc->fileTable[i] == NULL) {
	curproc->fileTable[i] = of;
	return fd;
      }
    }
    *errp = EMFILE;
  }

  vfs_close(v);
  return -1;
}

int 
sys_close(int fd) {
  struct openfile * of = NULL;
  struct vnode *vn;

  if (fd<0||fd>OPEN_MAX) return -1;
  of = curproc->fileTable[fd];
  if (of == NULL) return -1;
  curproc->fileTable[fd] = NULL;

  if (--of->countRef > 0) return 0;
  vn = of->vn;
  if (vn == NULL) return -1;

  vfs_close(vn);
  return 0;
}

#endif /* OPT_FILESYSTEM */

int sys_read(int fd, userptr_t buf_ptr, size_t size) {
  int i;
  char *str = (char*) buf_ptr;

  if (fd != STDIN_FILENO) {
#if OPT_FILESYSTEM
    return file_read(fd, buf_ptr, size);
#else
    kprintf("sys_read supportd only stdin.\n");
    return -1;
#endif
  }

  for (i = 0; i < (int) size; i++) {
    str[i] = getch();
    if (str[i] < 0) {
      return -1;
    }
  }

  return (int)size;
}


int sys_write(int fd, userptr_t buf_ptr, size_t size) {
  int i;
  char *str = (char*) buf_ptr;

  if (fd != STDOUT_FILENO && fd != STDERR_FILENO) {
#if OPT_FILESYSTEM
    return file_write(fd, buf_ptr, size);
#else
    kprintf("sys_write supported only stdout.\n");
    return -1;
#endif
  }
 
  for (i = 0; i < (int) size; i++) {
    putch(str[i]);
  }

  return (int) size;
}
