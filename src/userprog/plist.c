#include <stddef.h>

#include "plist.h"

void plist_print_process(key_t, struct process*, int);
bool flag_children(key_t, struct process*, key_t);

key_t plist_add_process(struct map* m, int parent_id, char* name, struct semaphore* sema)
{
  struct process* p = malloc(sizeof(struct process));
  strlcpy(p->name, name, 16);
  p->parent = parent_id;
  p->sema = sema;
  p->exit_status = 0;
  p->parent_dead = 0;
  p->is_alive = 1;
  return map_insert(m, p);

}

struct process* plist_find_process(struct map* m, key_t k)
{
  return map_find(m, k);
}

void plist_remove_process(struct map* m , key_t k)
{
  struct process* p = map_find(m, k);
  if(p != NULL)
  {
    p->is_alive = 0;
    map_remove_if(m, &flag_children, k);
  }
}

void plist_force_remove_process(struct map* m, key_t k)
{
  plist_remove_process(m, k);
  free(map_remove(m, k));
}

void plist_print_all(struct map* m)
{
  printf("%-3s %-16s %-11s %-6s %-6s %-13s\n", "PID", "NAME", "EXIT_STATUS", "ALIVE?", "PARENT", "PARENT_ALIVE?");
  map_for_each(m, &plist_print_process, 0);
  printf("\n");
}

void plist_set_exit_status(struct map* m, key_t k, int status)
{
  struct process* p = map_find(m, k);
  p->exit_status = status;
}

void plist_print_process(key_t k, struct process* p, int aux)
{
  if(p != NULL)
  {
    printf("%-3i %-16s %-11i %-6s %-6i %-13s\n",
        k, p->name, p->exit_status, p->is_alive ? "true":"false", p->parent,
        p->parent_dead ? "false":"true");
  }
}

bool flag_children(key_t k, struct process* p, key_t parent)
{
  if(p != NULL)
  {
    if(p->parent == parent)
    {
      p->parent_dead = 1;
    }
    if(p->parent_dead && !p->is_alive)
    {
      free(p->sema);
      free(p);
      return 1;
    }
  }
  return 0;
}

