/*
INSTRUCTIONS:
This file must include only the main function
The main function should print out the various cases
and finally print out tables showing the performance
of various policies with the different workloads.
Vary the cache size.

You may include a function or 2 to ease the printing of tables.

*/

#include "definitions.h"
#include <stdio.h>
#include <stdlib.h>

int main(){
	
	struct workload *work;
	float hitrate_fifo;
	float hitrate_LRU;
	float hitrate_approxLRU; 
	float hitrate_rand;

	// All workload hve maximum cache size 100 and total pages are 10000
	work = generate_workload(RANDOM, 100,10000);
	// Adding files to csv
	FILE *file_ptr_loop = fopen("data_csv_rand.csv","w");
	// Increasing the Cache size
	for(int i=2;i<110;i++){
		hitrate_fifo = policy_FIFO(work,i);
		hitrate_LRU = policy_LRU(work,i);
		hitrate_approxLRU = policy_LRUapprox(work,i);
		hitrate_rand = policy_RANDOM(work,i);
		fprintf(file_ptr_loop,"%f,%f,%f,%f\n",hitrate_fifo,hitrate_LRU,hitrate_approxLRU,hitrate_rand);
	}

	free(work);
	fclose(file_ptr_loop);
	work = generate_workload(LOCAL, 100,10000);

	file_ptr_loop = fopen("data_csv_local.csv","w");
	for(int i=2;i<110;i++){
		hitrate_fifo = policy_FIFO(work,i);
		hitrate_LRU = policy_LRU(work,i);
		hitrate_approxLRU = policy_LRUapprox(work,i);
		hitrate_rand = policy_RANDOM(work,i);
		fprintf(file_ptr_loop,"%f,%f,%f,%f\n",hitrate_fifo,hitrate_LRU,hitrate_approxLRU,hitrate_rand);

	}
	free(work);
	fclose(file_ptr_loop);

	work = generate_workload(LOOP, 100,10000);

	file_ptr_loop = fopen("data_csv_loop.csv","w");
	for(int i=2;i<110;i++){
		hitrate_fifo = policy_FIFO(work,i);
		hitrate_LRU = policy_LRU(work,i);
		hitrate_approxLRU = policy_LRUapprox(work,i);
		hitrate_rand = policy_RANDOM(work,i);
		fprintf(file_ptr_loop,"%f,%f,%f,%f\n",hitrate_fifo,hitrate_LRU,hitrate_approxLRU,hitrate_rand);
	}
	free(work);
	fclose(file_ptr_loop);
}

