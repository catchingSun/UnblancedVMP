#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "place_vm.h"



int place(const int *classfity_result, SVM *scarcity_order, const LIST vmlist, const LIST pmlist, VMLIST *sort_vmlist, PList *plist){

	init_sort_vmlist(classfity_result, vmlist, pmlist, &sort_vmlist);
	set_scarcity_order(classfity_result, scarcity_order, vmlist, pmlist);

	get_sorted_vmlist(classfity_result, scarcity_order, vmlist, pmlist, sort_vmlist);
	
	place_vmlist(vmlist, pmlist, sort_vmlist, plist);

	print_placement(pmlist, plist);
	return 0;
}


int set_scarcity_order(const int *classfity_result, SVM *scarcity_order, const LIST vmlist, const LIST pmlist){

	int i;
	
	TVM *total_need;
	
	total_need = (TVM *)malloc(pmlist.list_len * sizeof(TVM));

	memset(total_need, 0, pmlist.list_len * sizeof(TVM));
		
	
	for(i = 0; i < vmlist.list_len; i++){
		if(classfity_result[i] == 0){
			total_need[0].cpu += vmlist.list[i].cpu;
			total_need[0].memory += vmlist.list[i].memory;
			
		}else if(classfity_result[i] == 1){
			total_need[1].cpu += vmlist.list[i].cpu;
			total_need[1].memory += vmlist.list[i].memory;	
		}
	}
	
	
	for(i = 0; i < pmlist.list_len; i++){

		double cpu = total_need[i].cpu / pmlist.list[i].cpu;
		double memory = total_need[i].memory / pmlist.list[i].memory;
		if(cpu - memory >= 0.000000){
			scarcity_order[i].cpu = 1;
			scarcity_order[i].memory = 2;
		}else{
			scarcity_order[i].cpu = 2;
			scarcity_order[i].memory = 1;
		}
	}

	return 0;
}


int init_sort_vmlist(const int *classfity_result, const LIST vmlist, const LIST pmlist, VMLIST **sort_vmlist){
	int i;

	int temp_1 = 0;
	int temp_2 = 0;
	*sort_vmlist = (VMLIST *)malloc(pmlist.list_len * sizeof(VMLIST));
	memset((*sort_vmlist), 0, pmlist.list_len * sizeof(VMLIST));
	
	for(i = 0; i < vmlist.list_len; i++){
		if(classfity_result[i] == 0){
			(*sort_vmlist)[classfity_result[i]].vmlist_len++;
		}else if(classfity_result[i] == 1){
			(*sort_vmlist)[classfity_result[i]].vmlist_len++;
		}
	}

	for(i = 0; i < pmlist.list_len; i++){
		int temp = (*sort_vmlist)[i].vmlist_len;
		(*sort_vmlist)[i].pm_id = i;
	//	init_sort_list_vmlist(temp, &(sort_vmlist[i]->vmlist_id));
		(*sort_vmlist)[i].vmlist_id = (int *)malloc(temp * sizeof(int));
			// memset(sort_vmlist[i]->vmlist_id, 0, temp);
	}
	
		
	for(i = 0; i < vmlist.list_len; i++){
		if(classfity_result[i] == 0){
			(*sort_vmlist)[classfity_result[i]].vmlist_id[temp_1] = i;
			temp_1 ++;
		}else if(classfity_result[i] == 1){
			(*sort_vmlist)[classfity_result[i]].vmlist_id[temp_2] = i;
			temp_2 ++;
		}
	}

	return 0;
}


int get_sorted_vmlist(const int *classfity_result, const SVM *scarcity_order, const LIST vmlist, const LIST pmlist, VMLIST *sort_vmlist){
	
	int i;
	for(i = 0; i < pmlist.list_len; i++){
		sort_vmlist_by_scarcity_order(scarcity_order, sort_vmlist[i].pm_id, vmlist, sort_vmlist[i].vmlist_id, sort_vmlist[i].vmlist_len);
	}
	
	return 0;
}

int sort_vmlist_by_scarcity_order( const SVM *scarcity_order, int pm_type, const LIST vmlist,int *sort_vmlist, int len){
	int i, j;

	if(scarcity_order[pm_type].cpu == 1){
		for(i = 0; i < len - 1; i++){
			int temp = sort_vmlist[i];
			int max_id = -1;
			for(j = i + 1; j < len; j++){
				if(vmlist.list[sort_vmlist[j]].cpu > vmlist.list[sort_vmlist[i]].cpu){
					max_id = j;
				}else if(vmlist.list[sort_vmlist[j]].cpu == vmlist.list[sort_vmlist[i]].cpu){
					if(vmlist.list[sort_vmlist[j]].memory > vmlist.list[sort_vmlist[i]].memory){
						max_id = j;
					}
				}
			}
			if(max_id != -1){
				sort_vmlist[i] = sort_vmlist[max_id];
				sort_vmlist[max_id] = temp;
			}
		}

	}else if(scarcity_order[pm_type].memory == 1){
		for(i = 0; i < len - 1; i++){
			int temp = sort_vmlist[i];
			int max_id = -1;
			for(j = i + 1; j < len; j++){
				if(vmlist.list[sort_vmlist[j]].memory > vmlist.list[sort_vmlist[i]].memory){
					max_id = j;
				}else if(vmlist.list[sort_vmlist[j]].memory == vmlist.list[sort_vmlist[i]].memory){
					if(vmlist.list[sort_vmlist[j]].cpu > vmlist.list[sort_vmlist[i]].cpu){
						max_id = j;
					}
				}
			}
			if(max_id != -1){
				sort_vmlist[i] = sort_vmlist[max_id];
				sort_vmlist[max_id] = temp;
			}
		}
	}
	return 0;
}



int place_vmlist(const LIST vmlist, const LIST pmlist, const VMLIST *sort_vmlist, PList *plist){
	int i, j;
	int flag = 0;
	PPList *p;

	
	for(i = 0; i < pmlist.list_len; i++){
		
		int temp_cpu;
		int temp_memory;
		plist[i].num = 0;
		plist[i].pm_id = pmlist.list[i].id;
		init_ppmlist(&(plist[i].plist), pmlist.list[i]);
		printf("第%d种类型物理机上放置的虚拟机为：\n    ", pmlist.list[i].id);
		for(j = 0; j < sort_vmlist[i].vmlist_len; j++){
			printf("%d ", sort_vmlist[i].vmlist_id[j]);
			p = plist[i].plist;
			while(p->next != NULL){
				temp_cpu = p->remain_cpu - vmlist.list[sort_vmlist[i].vmlist_id[j]].cpu;
				temp_memory = p->reman_memory - vmlist.list[sort_vmlist[i].vmlist_id[j]].memory;
				if((temp_cpu < 0) || (temp_memory < 0)){
					p = p->next;
				}else{
					p->remain_cpu = temp_cpu;
					p->reman_memory = temp_memory;
					p->len++;
					insert_pvmlist_from_tail(p->pvmlist, sort_vmlist[i].vmlist_id[j], vmlist);
					flag = 1;
					break;
				}
			}
			if(flag == 1){
				flag = 0;
				break;
			}
			
			temp_cpu = p->remain_cpu - vmlist.list[sort_vmlist[i].vmlist_id[j]].cpu;
			temp_memory = p->reman_memory - vmlist.list[sort_vmlist[i].vmlist_id[j]].memory;
			if((temp_cpu < 0) || (temp_memory < 0)){
				// p->remain_cpu = temp_cpu;
				// p->reman_memory = temp_memory;
				insert_ppmlist_from_tail(plist[i].plist, pmlist.list[i]);
				p = p->next;
			}else if((temp_cpu >= 0) && (temp_memory >= 0) && p->len > 0){
				insert_pvmlist_from_tail(p->pvmlist, sort_vmlist[i].vmlist_id[j], vmlist);
				p->remain_cpu -= vmlist.list[sort_vmlist[i].vmlist_id[j]].cpu;
				p->reman_memory -= vmlist.list[sort_vmlist[i].vmlist_id[j]].memory;
				p->len ++;
			}
			
			
			if(p->len == 0){
				plist[i].num ++;
				init_pvmlist(&(p->pvmlist), sort_vmlist[i].vmlist_id[j], vmlist);
				p->len ++;
				p->remain_cpu -= vmlist.list[sort_vmlist[i].vmlist_id[j]].cpu;
				p->reman_memory -= vmlist.list[sort_vmlist[i].vmlist_id[j]].memory;
			}
		}
		printf("\n\n");
	}
		
	return 0;
}

int print_placement(const LIST pmlist, PList *plist){
	int i;
	PPList *p;
	int total = 0;
	printf("================================================================================\n");
	for(i = 0; i < pmlist.list_len; i++){
		printf("第%d种类型物理机上放置的虚拟机序列：\n", pmlist.list[i].id);
		p = plist[i].plist;
		while(p != NULL){
			PVList *v;
			v = p->pvmlist;
			printf("    ");
			while(v != NULL){
				printf("%d ", v->vm_id);
				v = v->next;
			}
			printf("\n");
			p = p->next;
			
		}
		printf("\n需要%d台物理机\n\n", plist[i].num);
		printf("================================================================================\n");
		total += plist[i].num;
		printf("\n");
	}
	
	printf("共需要%d台物理机\n", total);
	
	return 0;
}
