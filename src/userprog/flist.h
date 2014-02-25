#ifndef USERPROG_FLIST_H
#define USERPROG_FLIST_H

#include "filesys/file.h"
#include <list.h>
//#include "threads/thread.h"
struct thread;

/* Place functions to handle a process open files here (file list).

   flist.h : Your function declarations and documentation.
   flist.c : Your implementation.

   The following is strongly recommended:

   - A function that given a file (struct file*, see filesys/file.h)
     and a process id INSERT this in a list of files. Return an
     integer that can be used to find the opened file later.

   - A function that given an integer (obtained from above function)
     and a process id FIND the file in a list. Should return NULL if
     the specified process did not insert the file or already removed
     it.

   - A function that given an integer (obtained from above function)
     and a process id REMOVE the file from a list. Should return NULL
     if the specified process did not insert the file or already
     removed it.

   - A function that given a process id REMOVE ALL files the specified
     process have in the list.

   All files obtained from filesys/filesys.c:filesys_open() are
   considered OPEN files and must be added to a list or else kept
   track of, to guarantee ALL open files are eventyally CLOSED
   (probably when removed from the list(s)).
 */
typedef struct file* value_t;
typedef int key_t;


struct association{
	key_t key;
	value_t value;
	struct list_elem elem;
};

struct map{
	struct list content;
	int next_key;
};

void map_init(struct map*);
int map_insert(struct map*, value_t);
value_t map_find(struct map*, key_t);
value_t map_remove(struct map*, key_t);
void map_for_each(struct map*, void(*exec)(key_t, value_t, int), int);
void map_remove_if(struct map*, bool(*exec)(key_t, value_t, int), int);

int32_t flist_add_file(struct file*, struct thread*);
value_t flist_find_file(int fd, struct thread*);
value_t flist_remove_file(int fd, struct thread*);
bool flist_remove_process(struct thread*);
#endif
