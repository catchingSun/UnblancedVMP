#ifndef __CLASSIFY_VM_BY_GA_

#include "load_sys_file.h"

#define __CLASSIFY_VM_BY_GA_

#define INIT_POPULATION_COUNT 150
#define SUB_POPULATION_COUNT 60
#define DIVIDE_COUNT 2
#define CROSSOVER_P 0.7
#define MUTATION_P 0.1
#define GENETIC_TIMES 100

#define GET_ARRAY_LEN(array,len) {len = (sizeof(array) / sizeof(int));}
#define GET_ARRAY_ROWS(array, len){len = ((sizeof(array) / sizeof(int)) / (sizeof(array[0]) / sizeof(int)));}
#define GET_ARRAY_COLS(array, len){len = sizeof(array[0]) / sizeof(int);}

#define MAX(a, b){(a > b ? a : b);}


typedef struct SEQUENCE{
	int *sequence;
	double fitness;
}SEQUENCE;

typedef struct GASEQUENCE{
	int rows;
	int cols;
	SEQUENCE *sequence;
}GASEQUENCE;


typedef struct FitnessArray{
	int side;
	double fitness;
	double select_p;
}*FitnessArray;

int genetic_algorithm(const int genetic_times, const int initially_list_len, const int sub_population_count, const LIST vmlist, const LIST pmlist, int *classfity_result);
int get_vmlist_len(const char * vmlist_path);
int init_random_sequence(GASEQUENCE *initially_vm_list, int initially_list_len, const LIST vmlist, const LIST pmlist);
int cross_process(GASEQUENCE *cross_sequence, const LIST vmlist, const LIST pmlist);
int variation_process(GASEQUENCE *variation_sequence, const int init_len, const LIST vmlist, const LIST pmlist);
int selection_seed_sequence(const GASEQUENCE sequence, GASEQUENCE *sub_sequence, FitnessArray *fitness_array, const sub_sequence_count);
int free_for_structure_array(GASEQUENCE *sequence);
int random_num(const int division_count);
int random_cross_variation_number(const int random_sequence_count);
int random_cross_variation_site(const int node_count);
double random_p();
int init_malloc_for_structure_array(GASEQUENCE *sequence, const int rows, const int cols);
double fitness_function(int *sequence, const int sequence_len, const LIST vmlist, const LIST pmlist);
void selectsort(FitnessArray *fitness_array, const int genetic_len);
void swap(FitnessArray *fitness_array1, FitnessArray *fitness_array2);
#endif



