#ifndef __LINKLIST_

#include "load_sys_file.h"

#define __LINKLIST_


typedef struct Node 
{
    VM data;
    struct Node *next;
}LNode,*LinkList;


typedef struct place_vmlist{
	
	int vm_id;
	struct place_vmlist *next;
	
}PVList, *PlaceVmlist;

typedef struct place_pmlist{
	int len;
	int remain_cpu;
	int reman_memory;
	PlaceVmlist pvmlist;
	struct place_pmlist *next;
	
}PPList, *PlacePmlist;

int init_pvmlist(PlaceVmlist *H, const int vm_id, const LIST vmlist);
int init_ppmlist(PlacePmlist *H, const VM pm);
int insert_pvmlist_from_tail(PlaceVmlist H, int vm_id, const LIST vmlist);
int insert_ppmlist_from_tail(PlacePmlist H, const VM pm);

#endif
