#ifndef __LOAD_SYS_CONFIG_


#define __LOAD_SYS_CONFIG_


#define VMPCONFIG_FILE_PATH "uvmp.ini"

typedef struct VM{
	int id;
	int cpu;
	int memory;
}VM;

typedef struct TVM{
	int cpu;
	int memory;
}TVM;


typedef struct LIST{
	int list_len;
	VM *list;
}LIST;


void (*init_pvm_list_fp)(const char *list_path, LIST *list);
int init_pvm_list(const char *list_path, LIST *list);
int init_uvmp(char *vmlist_file_path, char *pmlist_file_path, int *divide_count);
int load_file(char *vmlist_file_path, char *pmlist_file_path, int *divide_count, LIST *vmlist, LIST *pmlist);
#endif
