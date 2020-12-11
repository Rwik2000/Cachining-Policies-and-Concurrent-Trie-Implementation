/*
INSTRUCTIONS:

This file will contain all the functions related to the generation of workload

*/

#include "definitions.h"
#include "stdlib.h"
#include <time.h>
// Note: The work must be generated for the returned pointer
struct workload * generate_workload(WorkloadsT type, int pages, int size)
{   
    struct workload * req_work = (struct workload*)malloc(sizeof(struct workload));
    req_work->type = type;
    req_work->pages = pages;
    req_work->size = size;
    req_work->work=malloc(sizeof(int)*req_work->size + 1);
    if(type == LOOP){
        generate_loop(req_work);
    }else if (type == RANDOM){
        generate_random(req_work);
    }else if(type == LOCAL){
        generate_local(req_work);
    }    
    return req_work;
}
// Random workload
struct workload * generate_random(struct workload *w)
{
    srand(time(NULL));
    for(int i=0;i<w->size;i++){
        w->work[i] = rand()%w->pages; 
    }
    return w;
}
// loop worload
struct workload * generate_loop(struct workload * w)
{
    int count=0;
    for(int i=0;i<w->size;i++){
        w->work[i] = count;
        if(count == w->pages - 1){
            count = 0;
        }else{
            count+=1;
        }        
    }
    return w;
}
// 80-20
struct workload * generate_local(struct workload * w)
{

    w->work=malloc(sizeof(int)*w->size+1);
    int *list=w->work;
    
    int i;
    int j;
    int size_upper=(w->size*80)/100;
    int page_lower=w->pages*20/100;
    srand(time(NULL));
    // Adding elements to the array
    for(i=0;i<size_upper;i++){
        
        list[i]=rand()%page_lower;
    }
    for(j=size_upper;j<w->size;j++){
        list[j]=rand()%(w->pages-page_lower)+page_lower;
    }
    int l;
    // Randomising the list
    srand(time(NULL));
    for(l=w->size-1;l>0;l--){
        int m=rand()%(l+1);
        int temp=list[m];
        list[m]=list[l];
        list[l]=temp;
    }

    return w;
}