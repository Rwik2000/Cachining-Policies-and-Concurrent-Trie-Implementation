// A file to check and make csv data for Reader-Writer locking
// on the custom data
#define _NO_HOH_LOCK_TRIE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include "trie.c"


trie_t trie;

void* insert_task(void *tnum){
    int t = *((int *) tnum);
    char key[10];
    int val;
    char ins_file_name[100];
    sprintf(ins_file_name, "./insert_test/%d.txt",t+1);
    FILE* ins_file = fopen(ins_file_name,"r");
    while (1)
    {   
        fscanf(ins_file,"%s",key);
        if (key[0]=='-') break;
        insert(trie,key,0);
    }
    fclose(ins_file);
    return NULL;
}

void* find_task(void *tnum){
    int t = *((int *) tnum);
    char key[10];
    char find_test_name[100];
    sprintf(find_test_name,"./find_test/%d.txt",t+1);
    FILE* find_test = fopen(find_test_name,"r");
    while (1)
    {
        int f_ret = 0;
        fscanf(find_test,"%s",key);
        if (key[0]=='-') break;
        find(trie,key,&f_ret);
    }
    fclose(find_test);
    return NULL;
}

int main(){
    int narr[300];
    int num_ins;
    int num_find;
    for(int i =0;i<300;i++)       narr[i] = i;

    // Writer intensive
    FILE *file_ptr_loop = fopen("plot/rw_lock/writer_intensive.csv","w");
    for(int j=5;j<301;j++){
        trie = init_trie();
        // 80% write(insertion)
        // 20% read (finding)
        num_ins= 4*j/5;
        num_find = j/5;
        clock_t t;
        t = clock();
        pthread_t ins_threads[num_ins]; 
        pthread_t find_threads[num_find]; 
        // Making concurrent threads

        for(int i =0; i<num_ins; i++)        pthread_create(&ins_threads[i], NULL, insert_task, (void *)&narr[i]);    
        for(int i =0; i<num_ins; i++)        pthread_join(ins_threads[i], NULL);
        for(int i =0; i<num_find;i++)        pthread_create(&find_threads[i], NULL, find_task, (void *)&narr[i]);    
        for(int i =0; i<num_find;i++)        pthread_join(find_threads[i], NULL);

        t = clock() - t;
        double time_taken = ((double)t)/CLOCKS_PER_SEC;
        fprintf(file_ptr_loop,"%d,%f\n",j,time_taken);
        delete_trie(trie);
        printf("%d\n",j);
    }

    // Reader intensive
    file_ptr_loop = fopen("plot/rw_lock/reader_intensive.csv","w");
    for(int j=5;j<301;j++){
        trie = init_trie();
        // 20% write(insertion)
        // 80% read (finding)
        num_ins = j/5;
        num_find = 4*j/5;
        clock_t t;
        t = clock();
        pthread_t ins_threads[num_ins]; 
        pthread_t find_threads[num_find]; 
        // Making concurrent threads
        for(int i =0; i<num_ins; i++)        pthread_create(&ins_threads[i], NULL, insert_task, (void *)&narr[i]);    
        for(int i =0; i<num_ins; i++)        pthread_join(ins_threads[i], NULL);
        for(int i =0; i<num_find;i++)        pthread_create(&find_threads[i], NULL, find_task, (void *)&narr[i]);    
        for(int i =0; i<num_find;i++)        pthread_join(find_threads[i], NULL);

        t = clock() - t;
        double time_taken = ((double)t)/CLOCKS_PER_SEC;
        fprintf(file_ptr_loop,"%d,%f\n",j,time_taken);
        // printf("Time taken :  %f seconds to execute \n", time_taken); 
        delete_trie(trie);
        printf("%d\n",j);

    }

    // Equal intensive
    file_ptr_loop = fopen("plot/rw_lock/equal_intensive.csv","w");
    for(int j=5;j<301;j++){
        trie = init_trie();
        // 50% write(insertion)
        // 50% read (finding)
        num_ins = j/2;
        num_find = j/2;
        clock_t t;
        t = clock();
        // Making concurrent threads
        pthread_t ins_threads[num_ins]; 
        pthread_t find_threads[num_find]; 
        for(int i =0; i<num_ins; i++)        pthread_create(&ins_threads[i], NULL, insert_task, (void *)&narr[i]);    
        for(int i =0; i<num_ins; i++)        pthread_join(ins_threads[i], NULL);
        for(int i =0; i<num_find;i++)        pthread_create(&find_threads[i], NULL, find_task, (void *)&narr[i]);    
        for(int i =0; i<num_find;i++)        pthread_join(find_threads[i], NULL);

        t = clock() - t;
        double time_taken = ((double)t)/CLOCKS_PER_SEC;
        fprintf(file_ptr_loop,"%d,%f\n",j,time_taken);
        // printf("Time taken :  %f seconds to execute \n", time_taken); 
        delete_trie(trie);
        printf("%d\n",j);

    }

    printf("DONE!\n");
    exit(0); 
}