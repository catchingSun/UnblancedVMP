#ifndef __PLACE_VM_

#include "linklist.h"

#define __PLACE_VM_

/*typedef struct VMLIST{
	int pm_id;
	int vmlist_len;
	LinkList *vmlist;
}VMLIST;
*/
typedef TVM SVM;

typedef struct VMLIST{
	int pm_id;
	int vmlist_len;
	int *vmlist_id;
}VMLIST;

typedef struct Plist{
	int num;
	int pm_id;
	PlacePmlist plist;
}PList;


int place(const int *classfity_result, SVM *scarcity_order, const LIST vmlist, const LIST pmlist, VMLIST *sort_vmlist, PList *plist);


int init_sort_vmlist(const int *classfity_result, const LIST vmlist, const LIST pmlist, VMLIST **sort_vmlist);

int set_scarcity_order(const int *classfity_result, SVM *scarcity_order, const LIST vmlist, const LIST pmlist);

int sort_vmlist_by_scarcity_order( const SVM *scarcity_order, int pm_type, const LIST vmlist,int *sort_vmlist, int len);

int get_sorted_vmlist(const int *classfity_result, const SVM *scarcity_order, const LIST vmlist, const LIST pmlist, VMLIST *sort_vmlist);

int place_vmlist(const LIST vmlist, const LIST pmlist, const VMLIST *sort_vmlist, PList *plist);

int print_placement(const LIST pmlist, PList *plist);

#endif
