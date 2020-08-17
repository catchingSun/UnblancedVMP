#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "linklist.h"


#define flag 0

int init_pvmlist(PlaceVmlist *H, const int vm_id, const LIST vmlist){
	*H = (PlaceVmlist)malloc(sizeof(PVList));
	(*H)->vm_id = vmlist.list[vm_id].id;
	(*H)->next = NULL;
	return 0;
}

int init_ppmlist(PlacePmlist *H, const VM pm){
	*H = (PlacePmlist)malloc(sizeof(PPList));
	(*H)->len = 0;
	(*H)->remain_cpu = pm.cpu;
	(*H)->reman_memory = pm.memory;
	(*H)->pvmlist = NULL;
	(*H)->next = NULL;
	return 0;
}


int insert_pvmlist_from_tail(PlaceVmlist H, int vm_id, const LIST vmlist){
	PVList *l, *s;
	l = (PVList *)malloc(sizeof(PVList));
	l->vm_id = vmlist.list[vm_id].id;
	l->next = NULL;
	s = H;
	
	while(s->next != NULL){
		s = s->next;
	}
	s->next = l;
	s = l;
	return 0;
}

int insert_ppmlist_from_tail(PlacePmlist H, const VM pm){
	PPList *l, *s;
	l = (PPList *)malloc(sizeof(PPList));
	l->len = 0;
	l->remain_cpu = pm.cpu;
	l->reman_memory = pm.memory;
	l->pvmlist = NULL;
	l->next = NULL;
	s = H;
	
	while(s->next != NULL){
		s = s->next;
	}
	s->next = l;
	s = l;
	return 0;
}

