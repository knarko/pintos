/* halt.c

	Simple program to test whether running a user program works.

	Just invokes a system call that shuts down the OS. */

#include <syscall.h>

	int
main (void)
{
	exit(13);
	/*This should not be seen */
	exit(14);
	halt ();
	/* not reached */
}
