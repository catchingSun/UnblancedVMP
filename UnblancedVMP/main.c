#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "classify_vm_by_ga.h"
#include "place_vm.h"


/* run this program using the console pauser or add your own getch, system("pause") or input loop */


int main(int argc, char *argv[]) {
	char vmlist_file_path[20];
	char pmlist_file_path[20];
	int *classfity_result;
	LIST vmlist;
	LIST pmlist;
	int divide_count;
	int i;
	SVM *scarcity_order = NULL;
	VMLIST *sort_vmlist = NULL;
	PList *plist = NULL;

	
	load_file(vmlist_file_path, pmlist_file_path, &divide_count, &vmlist, &pmlist);
	classfity_result = (int *)malloc(vmlist.list_len * sizeof(int));
	genetic_algorithm(GENETIC_TIMES, INIT_POPULATION_COUNT, SUB_POPULATION_COUNT, vmlist, pmlist, classfity_result);
	printf("********************************************************************************\n");
	for(i = 0; i < vmlist.list_len; i++){
		printf("%d ", classfity_result[i]);
	}
	printf("\n");
	for(i = 0; i < pmlist.list_len; i++){
		printf("%d\n", pmlist.list[i].cpu);
	}
	printf("********************************************************************************\n");
	scarcity_order = (SVM *)malloc(pmlist.list_len * sizeof(SVM));

	//memset(sort_vmlist, 0, pmlist.list_len * sizeof(VMLIST));

	plist = (PList *)malloc(pmlist.list_len * sizeof(PList));
	place(classfity_result, scarcity_order, vmlist, pmlist, sort_vmlist, plist);

	return 0;
}

