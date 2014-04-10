#include "map.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


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

