#include <stddef.h>

#include "userprog/flist.h"
#include "threads/thread.h"
#include "userprog/map.h"
#include "threads/synch.h"



void flist_init(struct map* m)
{
 map_init(m);
 m->next_key = 2;

}

/*
  A function that given a file (struct file*, see filesys/file.h)
  and a process id INSERT this in a list of files. Return an
  integer that can be used to find the opened file later.
*/
int32_t flist_add_file(struct file* file, struct thread* t)
{

  int32_t k = map_insert(&(t->open_files), file);

  return k;
}

/*
  A function that given an integer (obtained from above function)
  and a process id FIND the file in a list. Should return NULL if
  the specified process did not insert the file or already removed
  it.
*/
value_t flist_find_file(int fd, struct thread* t)
{

  value_t v = map_find(&(t->open_files), fd);

  return v;
}

/*
  A function that given an integer (obtained from above function)
  and a process id REMOVE the file from a list. Should return NULL
  if the specified process did not insert the file or already
  removed it.
*/
value_t flist_remove_file(int fd, struct thread* t)
{

  value_t v = map_remove(&(t->open_files), fd);

  return v;
}


bool always_true(key_t t, value_t v, int i)
{
  return true;
}

/*
  A function that given a process id REMOVE ALL files the specified
  process have in the list.
*/
void flist_remove_process(struct thread* t)
{
  if (t == NULL) return;


  map_remove_if(&(t->open_files), always_true, 0);

}

