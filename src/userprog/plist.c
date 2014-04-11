#include <stddef.h>

#include "plist.h"

key_t plist_add_process(struct map* m, struct process* p)
{
  return map_insert(m, p);
}

/*
  A function that given an integer (obtained from above function)
  and a process id FIND the file in a list. Should return NULL if
  the specified process did not insert the file or already removed
  it.
*/
struct process* plist_find_process(struct map* m, key_t k)
{
  return map_find(m, k);
}


void plist_remove_if(struct map*, bool(*exec)(key_t, struct process*, int), int);
/*
  A function that given an integer (obtained from above function)
  and a process id REMOVE the file from a list. Should return NULL
  if the specified process did not insert the file or already
  removed it.
*/
struct process* plist_remove_process(struct map* m , key_t k)
{
  return map_remove(m, k);
}

void plist_for_each(struct map* m, void(*exec)(key_t k, struct process* p, int i), int i)
{
  map_for_each(m, exec, i);
}

