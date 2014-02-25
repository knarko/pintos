#include <stddef.h>

#include "userprog/flist.h"
#include "threads/malloc.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "threads/thread.h"

/*
  A function that given a file (struct file*, see filesys/file.h)
  and a process id INSERT this in a list of files. Return an
  integer that can be used to find the opened file later.
*/
int32_t flist_add_file(struct file* file, struct thread* t)
{
  return map_insert(&(t->open_files), file);
}

/*
  A function that given an integer (obtained from above function)
  and a process id FIND the file in a list. Should return NULL if
  the specified process did not insert the file or already removed
  it.
*/
value_t flist_find_file(int fd, struct thread* t)
{
  return map_find(&(t->open_files), fd);
}

/*
  A function that given an integer (obtained from above function)
  and a process id REMOVE the file from a list. Should return NULL
  if the specified process did not insert the file or already
  removed it.
*/
value_t flist_remove_file(int fd, struct thread* t)
{   
  return map_remove(&(t->open_files), fd);
}


bool always_true(key_t, value_t, int i)
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

  map_remove_if(&(t->file_list), always_true, 0);
}



/*
  Map functions.
*/

void map_init(struct map* m)
{
  list_init(&m->content);
  m->next_key = 2;
  return;
}

int map_insert(struct map* m, value_t c)
{
  struct association *my_ass = malloc(sizeof(struct association));
  my_ass->value = c;
  my_ass->key = m->next_key++;
  list_push_back(&(m->content), &(my_ass->elem));
  return my_ass->key;
}

value_t map_find(struct map* m, key_t i)
{
  struct list_elem *e;
  for(e = list_rbegin(&(m->content)); e != list_rend(&(m->content)); e = list_prev(e)){
    if(e->prev == NULL) break;
    struct association *f = list_entry(e, struct association, elem);
    if(f->key == i){
      return &(f->value);
    }
  }
  return NULL;
}

value_t map_remove(struct map* m, key_t i)
{
  struct list_elem *e;
  for(e = list_rbegin(&(m->content)); e != list_rend(&(m->content)); e = list_prev(e)){
    if(e->prev == NULL) break;
    struct association *f = list_entry(e, struct association, elem);
    if(f->key == i){
      //TODO: copy f->value to temp;
      free(&(f->value));
      list_remove(&(f->elem));
      free(f);
      return NULL;
    }
  }
  return NULL;
}


/*
  REMOVE?
*/
void map_for_each(struct map* m, void(*exec)(key_t k, value_t v, int i), int i)
{
  struct list_elem *e;
  for(e = list_rbegin(&(m->content)); e != list_rend(&(m->content)); e = list_prev(e)){
    if(e->prev == NULL) break;
    struct association *f = list_entry(e, struct association, elem);
    exec(f->key, f->value, i);
  }
  return;
}

void map_remove_if(struct map* m, bool(*exec)(key_t, value_t, int), int i)
{
  struct list_elem *e;
  for(e = list_rbegin(&(m->content)); e != list_rend(&(m->content));){
    if(e->prev == NULL) break;
    struct association *f = list_entry(e, struct association, elem);
    if(exec(f->key, f->value, i)){
      e = list_prev(e);
      list_remove(&(f->elem));
      free(f);
    }
  }
  return;
}
