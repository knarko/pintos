#include <stdio.h>
#include <syscall-nr.h>
#include "userprog/syscall.h"
#include "threads/interrupt.h"
#include "threads/thread.h"

/* header files you probably need, they are not used yet */
#include <string.h>
#include "filesys/filesys.h"
#include "filesys/file.h"
#include "threads/vaddr.h"
#include "threads/init.h"
#include "userprog/pagedir.h"
#include "userprog/process.h"
#include "devices/input.h"

#define DBG(format, ...) printf(format "\n", ##__VA_ARGS__)

static void syscall_handler (struct intr_frame *);

void syscall_init (void)
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}


/* This array defined the number of arguments each syscall expects.
   For example, if you want to find out the number of arguments for
   the read system call you shall write:

   int sys_read_arg_count = argc[ SYS_READ ];

   All system calls have a name such as SYS_READ defined as an enum
   type, see `lib/syscall-nr.h'. Use them instead of numbers.
*/
const int argc[] = {
  /* basic calls */
  0, 1, 1, 1, 2, 1, 1, 1, 3, 3, 2, 1, 1,
  /* not implemented */
  2, 1,    1, 1, 2, 1, 1,
  /* extended */
  0
};

static int32_t
sys_open(char* fname)
{
	struct file* ofile = filesys_open(fname);
	if(ofile != NULL)
	{
		return flist_add_file(ofile, pid);
	} else {
		return -1;
	}
}

static int32_t
sys_read(const int32_t fd, char* buf, const int32_t len)
{
  if (fd == STDIN_FILENO)
    {
      char c;
      int i;
      for (i = 0; i < len; ++i)
	{
	  c = input_getc();
	  buf[i] = c == '\r'? '\n' : c;
	  putchar(c);
	}
      return i;
    }
  return -1;
}

static int32_t
sys_write(const int32_t fd, char* buf, const int32_t len)
{
  if (fd == STDOUT_FILENO)
    {
      putbuf(buf, len);
      return len;
    }
  return -1;
}


static void
syscall_handler (struct intr_frame *f)
{
  int32_t* esp = (int32_t*)f->esp;

  switch ( esp[0] )
    {
    case SYS_HALT:
      power_off();
      break;

    case SYS_EXIT:
      DBG("#Exit status: %i", esp[1]);
      thread_exit();
      break;

    case SYS_CREATE:
		f->eax = filesys_create((const char*)esp[1], esp[2]);
      break;

    case SYS_OPEN:
		f->eax = sys_open(esp[1]);
      break;

    case SYS_READ:
      f->eax = sys_read(esp[1], (char*)esp[2], esp[3]);
      break;

    case SYS_WRITE:
      f->eax = sys_write(esp[1], (char*)esp[2], esp[3]);
      break;

    case SYS_CLOSE:
		filesys_close(esp[1]);
      break;

    case SYS_REMOVE:
      break;



    default:
      {
	printf ("Executed an unknown system call!\n");

	printf ("Stack top + 0: %d\n", esp[0]);
	printf ("Stack top + 1: %d\n", esp[1]);

	thread_exit ();
      }
    }
}
