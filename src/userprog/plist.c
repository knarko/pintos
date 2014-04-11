#include <stddef.h>

#include "plist.h"

key_t plist_add_process(struct process* p, struct map* m)
{
  return map_insert(m, p);
}

///*
//  A function that given an integer (obtained from above function)
//  and a process id FIND the file in a list. Should return NULL if
//  the specified process did not insert the file or already removed
//  it.
//*/
//value_t flist_find_file(int fd, struct thread* t)
//{
//  return map_find(&(t->open_files), fd);
//}
//
///*
//  A function that given an integer (obtained from above function)
//  and a process id REMOVE the file from a list. Should return NULL
//  if the specified process did not insert the file or already
//  removed it.
//*/
//value_t flist_remove_file(int fd, struct thread* t)
//{
//  return map_remove(&(t->open_files), fd);
//}
//
//
//bool always_true(key_t t, value_t v, int i)
//{
//  return true;
//}
//
///*
//  A function that given a process id REMOVE ALL files the specified
//  process have in the list.
//*/
//void flist_remove_process(struct thread* t)
//{
//  if (t == NULL) return;
//
//  map_remove_if(&(t->open_files), always_true, 0);
//}
//
