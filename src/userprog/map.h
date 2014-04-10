#ifndef _MAP_H
#define _MAP_H

#include <list.h>


typedef void* value_t;
typedef int key_t;

struct association
{
	key_t key;
	value_t value;
	struct list_elem elem;
};

struct map
{
	struct list content;
	int next_key;
};

void map_init(struct map*);
int map_insert(struct map*, value_t);
value_t map_find(struct map*, key_t);
value_t map_remove(struct map*, key_t);
void map_for_each(struct map*, void(*exec)(key_t, value_t, int), int);
void map_remove_if(struct map*, bool(*exec)(key_t, value_t, int), int);

#endif
