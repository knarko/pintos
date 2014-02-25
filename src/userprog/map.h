#ifndef MAP_H
#define MAP_H

#include "list.h"

#define PANIC() exit(1)

typedef struct file* value_t;
typedef int key_t;
//typedef enum bool {true,false} bool;

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
int map_insert(struct map*, char*);
char* map_find(struct map*, int);
char* map_remove(struct map*, int);
void map_for_each(struct map*, void(*exec)(key_t, value_t, int), int);
void map_remove_if(struct map*, bool(*exec)(key_t, value_t, int), int);

#endif
