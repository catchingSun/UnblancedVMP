/*****************************************************************************
Copyright: 2018, XUST LeiMly.
File name: classify_vm_by_ga.c 
Description: 1.初始化系统参数；2. 初始化虚拟机资源队列；3.采用遗传算法确定分配在两类物理机中的虚拟机
Author: LeiMly 
Version: V1.0 
Date: 2018.4.20
History: 
*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "classify_vm_by_ga.h"

int genetic_algorithm(const int genetic_times, const int initially_list_len, const int sub_population_count, const LIST vmlist, const LIST pmlist, int *classfity_result){
	int count = 0;
	int i;
	GASEQUENCE sub_sequence;
	GASEQUENCE new_sub_sequence;
	FitnessArray *fitness_array;
	
	int sub_seq_len = sub_population_count + (int)(CROSSOVER_P * sub_population_count) * (int)(CROSSOVER_P * sub_population_count - 1) + (int)(MUTATION_P * sub_population_count);
	init_malloc_for_structure_array(&sub_sequence, sub_seq_len, vmlist.list_len);
	init_malloc_for_structure_array(&new_sub_sequence, sub_population_count, vmlist.list_len);
	while(count < genetic_times){
		if(count == 0){
			GASEQUENCE initially_vm_list;
			int ga_len = initially_list_len + (int)(CROSSOVER_P * initially_list_len) * (int)(CROSSOVER_P * initially_list_len - 1) + (int)(MUTATION_P * initially_list_len);
			
			init_malloc_for_structure_array(&initially_vm_list, ga_len, vmlist.list_len);
			init_random_sequence(&initially_vm_list, initially_list_len, vmlist, pmlist);
			
			cross_process(&initially_vm_list, vmlist, pmlist);
			//printf("====init_cross_process=======\n");
			variation_process(&initially_vm_list, initially_list_len, vmlist, pmlist);
			//printf("====init_variation_process=======\n");
			fitness_array = (FitnessArray *)malloc(ga_len * sizeof(struct FitnessArray));
			for(i = 0; i < ga_len; i++){
				fitness_array[i] = (FitnessArray)malloc(sizeof(struct FitnessArray));
		
			}
			//printf("====init_variation_process=======\n");
			selection_seed_sequence(initially_vm_list, &sub_sequence, fitness_array, sub_population_count);
			//printf("====init_selection_seed_sequence=======\n");
			free(fitness_array);
			//free_for_structure_array(&initially_vm_list);
					
		}else{
			printf("***********************************%d*******************************************\n", count);
			
			cross_process(&sub_sequence, vmlist, pmlist);
			printf("====cross_process=======\n");

			variation_process(&sub_sequence, sub_population_count, vmlist, pmlist);
		//	printf("====variation_process=======\n");

			fitness_array = (FitnessArray *)malloc(sub_seq_len * sizeof(struct FitnessArray));
			for(i = 0; i < sub_seq_len; i++){
				fitness_array[i] = (FitnessArray)malloc(sizeof(struct FitnessArray));
		
			}
			
			selection_seed_sequence(sub_sequence, &new_sub_sequence, fitness_array, sub_population_count);
			
			free(fitness_array);
		//	printf("====selection_seed_sequence=======\n");
			for(i = 0; i < sub_sequence.rows; i++){
				memset(sub_sequence.sequence[i].sequence, 0, sub_sequence.cols * sizeof(int));
				sub_sequence.sequence[i].fitness = 0;
			}
			
			sub_sequence.cols = new_sub_sequence.cols;
			sub_sequence.rows = new_sub_sequence.rows;
			
			
			for(i = 0; i < new_sub_sequence.rows; i++){
				memcpy(sub_sequence.sequence[i].sequence, new_sub_sequence.sequence[i].sequence, new_sub_sequence.cols * sizeof(int));
				sub_sequence.sequence[i].fitness = new_sub_sequence.sequence[i].fitness;
			}
			
			memcpy(classfity_result, new_sub_sequence.sequence[0].sequence, new_sub_sequence.cols * sizeof(int));
			for(i = 0; i < new_sub_sequence.cols; i++){
				printf("%d ", new_sub_sequence.sequence[0].sequence[i]);
			}
			printf("\n");
			printf("%f\n", new_sub_sequence.sequence[0].fitness);

		}
		
		count++;
	}
	//free_for_structure_array(&new_sub_sequence);
	//free_for_structure_array(&sub_sequence);
	return 0;
}

int init_malloc_for_structure_array(GASEQUENCE *sequence, const int rows, const int cols){
	int i = 0;
	sequence->rows = 0;
	sequence->cols = cols;
	sequence->sequence = (SEQUENCE *)malloc(rows * sizeof(SEQUENCE));
	for(i = 0; i < rows; i++){
		sequence->sequence[i].sequence = (int *)malloc(cols * sizeof(int));
	}
	return 0;
}

int free_for_structure_array(GASEQUENCE *sequence){
	int i = 0;

	for(i = 0; i < sequence->rows; i++){
	
		free(sequence->sequence[i].sequence);
		sequence->sequence[i].sequence = NULL;
	
	}
	free(sequence->sequence);
	sequence->sequence = NULL;
	return 0;
}

int init_random_sequence(GASEQUENCE *initially_vm_list, int initially_list_len, const LIST vmlist, const LIST pmlist){
	int i, j;
	srand((unsigned)time(NULL));
	for(i = 0; i < initially_list_len; i++){
		for(j = 0; j < vmlist.list_len; j++){
			initially_vm_list->sequence[i].sequence[j] = random_num(pmlist.list_len);
		}
		initially_vm_list->sequence[i].fitness = fitness_function(initially_vm_list->sequence[i].sequence, vmlist.list_len, vmlist, pmlist);
	}
	initially_vm_list->rows = initially_list_len;
	initially_vm_list->cols = vmlist.list_len;
	return 0;
}

int cross_process(GASEQUENCE *cross_sequence, const LIST vmlist, const LIST pmlist){
	int i = 0, j = 0, k = 0;
	int cross_sequence_cols = cross_sequence->cols;
	int cross_sequence_rows = cross_sequence->rows;
	int *control_isrepeat;
	int cross_count = (int)(CROSSOVER_P * cross_sequence_rows);
	int *cross_array;
	int cross = 0;
	control_isrepeat = (int *)malloc(cross_sequence_rows * sizeof(int));
	
	cross_array = (int *)malloc(cross_count * sizeof(int));
	cross_sequence->rows = cross_sequence_rows + cross_count * (cross_count - 1);
	
	for(i = 0; i < cross_count; i++){
		int temp = random_cross_variation_number(cross_sequence_rows);
		while(control_isrepeat[temp] == 1){
			
			temp = random_cross_variation_number(cross_sequence_rows);
		}
		
		cross_array[i] = temp;
		control_isrepeat[temp] = 1;
	}

	
	cross = cross_sequence_rows;
	
	for(i = 0; i < cross_count - 1; i ++){
		for(j = i + 1; j < cross_count; j ++){
			int cross_site = random_cross_variation_site(cross_sequence_cols);
			for(k = 0; k < cross_sequence_cols; k++){
				if(k >= cross_site){
			
					cross_sequence->sequence[cross].sequence[k] = cross_sequence->sequence[cross_array[j]].sequence[k];
					cross_sequence->sequence[cross + 1].sequence[k] = cross_sequence->sequence[cross_array[i]].sequence[k];
				}else{
					cross_sequence->sequence[cross].sequence[k] = cross_sequence->sequence[cross_array[i]].sequence[k];
					cross_sequence->sequence[cross + 1].sequence[k] = cross_sequence->sequence[cross_array[j]].sequence[k];
				}
				
			}
		
			cross_sequence->sequence[cross].fitness = fitness_function(cross_sequence->sequence[cross].sequence, vmlist.list_len, vmlist, pmlist);
			cross_sequence->sequence[cross + 1].fitness = fitness_function(cross_sequence->sequence[cross + 1].sequence, vmlist.list_len, vmlist, pmlist);
			cross += 2;
		}
		
	}
	
	//free(control_isrepeat);
	//free(cross_array);
	return 0;
}

int variation_process(GASEQUENCE *variation_sequence, const int init_len, const LIST vmlist, const LIST pmlist){
	
	int i = 0, j = 0;


	int variation_site = 0;
	int variation_value = 0;
	
	int variation_sequence_cols = variation_sequence->cols;
	int variation_sequence_rows = variation_sequence->rows;
	
	int variation_count = (int)(MUTATION_P * init_len);
	int variation = 0;
	int *variation_array;
	int *control_isrepeat;
	variation_array = (int *)malloc(variation_count * sizeof(int));
	control_isrepeat = (int *)malloc(variation_sequence_rows * sizeof(int));

	variation_sequence->rows = variation_sequence_rows + variation_count;
	memset(variation_array, 0, variation_count * sizeof(int));
	memset(control_isrepeat, 0, init_len * sizeof(int)); //variation_sequence_rows

	for(i = 0; i < variation_count; i++){

		int temp = random_cross_variation_number(init_len);

		while(control_isrepeat[temp] == 1){
			temp = random_cross_variation_number(init_len);
		}
		variation_array[i] = temp;
		control_isrepeat[temp] = 1;
	}
	

	variation = variation_sequence_rows;
	
	for(i = variation; i < (variation + variation_count); i++){
		
		memcpy(variation_sequence->sequence[i].sequence, variation_sequence->sequence[variation_array[i - variation]].sequence, variation_sequence->cols * sizeof(int));
		
		variation_site = random_cross_variation_site(variation_sequence_cols);
		variation_value = random_num(pmlist.list_len);
		
		while(variation_value == variation_sequence->sequence[i].sequence[variation_site]){
			variation_value = random_num(pmlist.list_len);
		}
		variation_sequence->sequence[i].sequence[variation_site] = variation_value;
		variation_sequence->sequence[i].fitness = fitness_function(variation_sequence->sequence[i].sequence, vmlist.list_len, vmlist, pmlist);
	
	}
	//free(control_isrepeat);
	//free(variation_array);
	return 0;
}

double fitness_function(int *sequence, const int sequence_len, const LIST vmlist, const LIST pmlist){
	int i = 0, j = 0;
	double n = 0;
	int temp = 0;
	TVM *total_need;
	total_need = (TVM *)malloc(pmlist.list_len * sizeof(TVM));
	memset(total_need, 0, pmlist.list_len * sizeof(TVM));
	for(i = 0; i < sequence_len; i++){
		if(sequence[i] == 0){
			total_need[0].cpu += vmlist.list[i].cpu;
			total_need[0].memory += vmlist.list[i].memory;	
		}else if(sequence[i] == 1){
			total_need[1].cpu += vmlist.list[i].cpu;
			total_need[1].memory += vmlist.list[i].memory;	
		}
	}
	for(i = 0; i < pmlist.list_len; i ++){
		double c = (double)total_need[i].cpu / pmlist.list[i].cpu;
		double m = (double)total_need[i].memory / pmlist.list[i].memory;
		if(c - m >= 0.0){
			temp = c / m;
			n = n + c + temp;
		
		}else {
			temp = m / c;
			n = n + m + temp;
		}
	
	}
	/*for(i = 0; i < sequence_len; i++){
		printf("%d ", sequence[i]);
	}
	printf("  %d", n);
	printf("\n");
	*/
	/*if(n > 0){
		temp = 1.0 / n;
	}else{
		temp = 0.0;
	}*/

/*	if(n - (int)n > 0.0000001){
		return (int)n + 1;
	}else{
	
		return n;
	}
	*/
	return n;
}

int calculation_select_p(const GASEQUENCE sequence, FitnessArray *fitness_array){
	double fitness_sum = 0;
	int i;
	
	for(i = 0; i < sequence.rows; i++){
		
		fitness_array[i]->fitness = 1.0 / sequence.sequence[i].fitness;
		fitness_sum += fitness_array[i]->fitness;
		fitness_array[i]->side = i;
		
	}
	
	selectsort(fitness_array, sequence.rows);
	
	
	for(i = 0; i < sequence.rows; i++){
		fitness_array[i]->select_p = fitness_array[i]->fitness / fitness_sum;
	//	printf("%f , %f\n", fitness_array[i]->fitness, fitness_array[i]->select_p);
	}
	
	return 0;
}


int selection_seed_sequence(const GASEQUENCE sequence, GASEQUENCE *sub_sequence, FitnessArray *fitness_array, const sub_sequence_count){
	int i;
	double random;
	double *interval;
	int *control_isrepreat;
	double sum = 0;
	int sub_seq = 0;
	int seq_len = sequence.rows;
	
	
	
	interval = (double *)malloc((seq_len + 1) * sizeof(double));
	control_isrepreat = (int *)malloc(seq_len * sizeof(int));	
	calculation_select_p(sequence, fitness_array);
	
	sub_sequence->rows = sub_sequence_count;
	sub_sequence->cols = sequence.cols;

	for(i = 1; i <= seq_len; i++){
		sum += fitness_array[i - 1]->select_p;
		interval[i] = sum;
	}
	memset(control_isrepreat, 0, seq_len * sizeof(int));
	memset(interval, 0, (seq_len + 1) * sizeof(int));
	
	memcpy(sub_sequence->sequence[sub_seq].sequence, sequence.sequence[fitness_array[seq_len - 1]->side].sequence, sequence.cols * sizeof(sequence));
	sub_sequence->sequence[sub_seq].fitness = sequence.sequence[fitness_array[seq_len - 1]->side].fitness;
	control_isrepreat[fitness_array[seq_len - 1]->side] = 1;
	sub_seq ++;
	while(1){
	
		random = random_p();
		for(i = 0; i < seq_len; i ++){
			if((random - interval[i]) >= 0.0 && (interval[i + 1] - random) >= 0.0){
				if(control_isrepreat[fitness_array[i]->side] == 0){
					memcpy(sub_sequence->sequence[sub_seq].sequence, sequence.sequence[fitness_array[i]->side].sequence, sequence.cols * sizeof(sequence));
					sub_sequence->sequence[sub_seq].fitness = sequence.sequence[fitness_array[i]->side].fitness;
					control_isrepreat[fitness_array[i]->side] = 1;
					sub_seq ++;
					if(sub_seq >= sub_sequence_count){
						return 0;
					}
				}else{
					break;
				}
			}
		}
		
	}

	return 0;
}

void selectsort(FitnessArray *fitness_array, const int genetic_len){
    int i, j, Maxdex;
    
    for (i = 0; i < genetic_len - 1; i++)
    {
        Maxdex = i;
        for (j = i + 1; j < genetic_len; j++)
        {	
            if ((fitness_array[Maxdex]->fitness - fitness_array[j]->fitness) >= 0.000001){
            	
            	Maxdex = j;
            }
                
        }
        swap(&fitness_array[i], &fitness_array[Maxdex]);
    }
}

void swap(FitnessArray *fitness_array1, FitnessArray *fitness_array2){

	FitnessArray temp;
	temp = (FitnessArray)malloc(sizeof(struct FitnessArray));
	
	temp->fitness = (*fitness_array1)->fitness;
	temp->side = (*fitness_array1)->side;
	
	(*fitness_array1)->fitness = (*fitness_array2)->fitness;
	(*fitness_array1)->side = (*fitness_array2)->side;
	
	(*fitness_array2)->fitness = temp->fitness;
	(*fitness_array2)->side = temp->side;
	free(temp);
}

int random_num(const int division_count){
	int vm_class;

	vm_class = rand() % division_count;
	return vm_class;
}


double random_p(){
	double random;
	random = rand() / (double)(RAND_MAX);
	return random;
}

int random_cross_variation_number(const int random_sequence_count){
	int cv_num;

	cv_num = rand() % random_sequence_count;
	return cv_num;
}

int random_cross_variation_site(const int node_count){
	int cv_site;

	cv_site = rand() % node_count;
	return cv_site;
}


