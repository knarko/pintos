#include <stddef.h>

#include "plist.h"

void print_process(key_t, struct process*, int);
void flag_children(key_t, struct process*, key_t);

key_t plist_add_process(struct map* m, int parent_id, char* name)
{
  struct process* p = malloc(sizeof(struct process));
  p->parent = parent_id;
  p->name = (char*) malloc(sizeof(char)*16);
  strlcpy(p->name, name, 16);
  p->exit_status = 0;
  p->parent_dead = false;
  p->is_alive = true;
  return map_insert(m, p);
}

struct process* plist_find_process(struct map* m, key_t k)
{
  return map_find(m, k);
}

void plist_remove_if(struct map*, bool(*exec)(key_t, struct process*, int), int);

int plist_remove_process(struct map* m , key_t k)
{
  int status = -1;
  struct process* p = map_find(m, k);
  if(p != NULL)
  {
    p->is_alive = false;
    status = p->exit_status;
  }
  map_remove_if(m, &flag_children, k);
  return status;
}

void plist_for_each(struct map* m, void(*exec)(key_t k, struct process* p, int i), int i)
{
  map_for_each(m, exec, i);
}

void plist_print_process(struct map* m)
{
  printf("%-3s %-16s %-11s %-6s %-6s %-13s\n", "PID", "NAME", "EXIT_STATUS", "ALIVE?", "PARENT", "PARENT_ALIVE?");
  map_for_each(m, &print_process, 0);
  printf("\n");
}

void print_process(key_t k, struct process* p, int aux)
{
  printf("%-3i %-16s %-11i %-6s %-6i %-13s\n",
      k, p->name, p->exit_status, p->is_alive ? "true":"false", p->parent,
      p->parent_dead ? "false":"true");
}

void flag_children(key_t k, struct process* p, key_t parent)
{
  if(p->parent == parent)
  {
    p->parent_dead = true;
  }
  if(p->parent_dead && !p->is_alive)
  {
    free(p->name);
    free(p);
    return true;
  }
  return false;
}

