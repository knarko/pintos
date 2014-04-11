#include "map.h"

#include <stdlib.h>


void map_init(struct map* m)
{
  list_init(&m->content);
  m->next_key = 2;
  return;
}

key_t map_insert(struct map* m, value_t value)
{
  struct association *my_ass = malloc(sizeof(struct association));
  my_ass->value = value;
  my_ass->key = m->next_key++;

  list_push_back(&m->content, &my_ass->elem);
  return my_ass->key;
}

value_t map_find(struct map* m, key_t key)
{
  struct list_elem* e;
  for(e = list_begin(&(m->content));
      e != list_end(&(m->content)); e = list_next(e))
  {
    struct association *f = list_entry(e, struct association, elem);
    if (f->key == key)
    {
      return f->value;
    }
  }
  return NULL;
}

value_t map_remove(struct map* m, key_t key)
{
  struct list_elem *e;
  for(e = list_begin(&(m->content));
      e != list_end(&(m->content)); e = list_next(e))
  {
    struct association *f = list_entry(e, struct association, elem);
    if (f->key == key)
    {
      list_remove(e);
      value_t ret = f->value;
      free(f);
      return ret;
    }
  }
  return NULL;
}

void map_for_each(struct map* m, void(*exec)(key_t k, value_t v, int i), int aux)
{
  struct list_elem *e;
  for(e = list_begin(&(m->content));
      e != list_end(&(m->content)); e = list_next(e))
  {
    struct association *f = list_entry(e, struct association, elem);
    exec(f->key, f->value, aux);
  }
  return;
}

void map_remove_if(struct map* m, bool(*exec)(key_t, value_t, int), int aux)
{
  struct list_elem *e;
    for(e = list_begin(&(m->content));
        e != list_end(&(m->content));)
    {
      struct association *f = list_entry(e, struct association, elem);
      if (exec(f->key, f->value, aux))
      {
        e = list_remove(e);
        free(f);
      }
      else
      {
        e = list_next(e);
      }
    }
  return;
}
