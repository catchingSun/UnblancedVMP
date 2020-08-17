#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "load_sys_file.h"

int load_file(char *vmlist_file_path, char *pmlist_file_path, int *divide_count, LIST *vmlist, LIST *pmlist){
	init_uvmp(vmlist_file_path, pmlist_file_path, divide_count);
	init_pvm_list_fp = init_pvm_list;
	(*init_pvm_list_fp)(vmlist_file_path, vmlist);
	(*init_pvm_list_fp)(pmlist_file_path, pmlist);
	return 0;
}
	
int init_uvmp(char *vmlist_file_path, char *pmlist_file_path, int *divide_count){
	FILE *vmlist_fp;
	char *delim = "=";
	vmlist_fp = fopen(VMPCONFIG_FILE_PATH, "r");
	
	while(!feof(vmlist_fp)){
		char *uvmp_conf[2];
		char str[50];
		fscanf(vmlist_fp, "%s", str);
		uvmp_conf[0] = strtok(str, delim);
		uvmp_conf[1] = strtok(NULL, delim);
	
		if(strcmp(uvmp_conf[0], "divide_count") == 0){
			*divide_count = atoi(uvmp_conf[1]);
		
		}else if(strcmp(uvmp_conf[0], "vmlist_file_path") == 0){
			strcpy(vmlist_file_path, uvmp_conf[1]);

		}else if(strcmp(uvmp_conf[0], "pmlist_file_path") == 0){
			strcpy(pmlist_file_path, uvmp_conf[1]);

		}
	}
	fclose(vmlist_fp);
	return 0;
}

int init_pvm_list(const char *list_path, LIST *list){
	int i;
	FILE *list_fp;
	list_fp = fopen(list_path, "r");
	fscanf(list_fp, "%d", &list->list_len);
	list->list = (VM *)malloc(sizeof(VM) * list->list_len);
	for(i = 0; i < list->list_len; i++){
		fscanf(list_fp, "%d %d %d", &(list->list[i].id), &(list->list[i].cpu), &(list->list[i].memory));
	}
	fclose(list_fp);
	return 0;
}