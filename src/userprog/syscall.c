#include <stdio.h>
#include <syscall-nr.h>
#include "userprog/syscall.h"
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/init.h"

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

	void
syscall_init (void)
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

static void syscall_handler (struct intr_frame *f) {
	int32_t* esp = (int32_t*)f->esp;

	DBG("# esp is %i", esp[0]);
	switch ( esp[0] ) {
		case SYS_HALT:
			DBG("# in halt");
			power_off();
			break;

		case SYS_EXIT:
			DBG("# in exit");
			DBG("# status is %i", esp[1]);
			thread_exit();
			break;

		case SYS_READ:
			if(esp[1] == STDIN_FILENO){
				DBG("# start reading");
				int i;
				for(i = 0; i < esp[3]; ++i){
					DBG("i is %i, esp3 is %i",i,esp[3]);
					((char*)esp[2])[i] =	input_getc();
					if(((char*)esp[2])[i] == '\r')
						((char*)esp[2])[i] = '\n';
					putchar(((char*)esp[2])[i]);
				}
				DBG("#end reading");
				f->eax = i;
			}else{
				f->eax = -1;
			}
			break;

		case SYS_WRITE:
			if(esp[1] == STDOUT_FILENO){
				putbuf(esp[2], esp[3]);
				f->eax = esp[3];
			}else{
				f->eax = -1;
			}
			break;


		default:
			printf ("Executed an unknown system call!\n");

			printf ("Stack top + 0: %d\n", esp[0]);
			printf ("Stack top + 1: %d\n", esp[1]);

			thread_exit ();
	}
}
