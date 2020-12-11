/* 
INSTRUCTIONS:

All enum, structs and functions to be declared here.
If a definition is already declared, do not change it.
You may add extra definitions as required.

*/

#ifndef __DEF_H__
#define __DEF_H__
#include <stdlib.h>
#include <stdio.h>
// #include "policy.c"
// #include "workload.c"

//----------------------------------------------------------------
//----------------------------------------------------------------
/* ALL ENUMERATORS HERE */

// LOOP - Workload type for Loop
// RANDOM - Workload type for RANDOM
// LOCAL - 80-20 workload type

typedef enum enum_workloads{LOOP, RANDOM, LOCAL}WorkloadsT;

//----------------------------------------------------------------
//----------------------------------------------------------------
/* ALL STRUCTS HERE */

// A Queue Node (Queue is implemented using Doubly Linked List)
typedef struct QNode {
	struct QNode *prev, *next;
	unsigned page_num; // the page number stored in this QNode
	int approx_check;
} QNode;

// A Queue (A FIFO collection of Queue Nodes)
typedef struct Queue {
	unsigned count; // Number of filled frames
	unsigned total_cap; // total number of frames
	QNode *front, *rear;
} Queue;

// A hash (Collection of pointers to Queue Nodes)
typedef struct hash_table {
	int capacity; // how many pages can be there
	QNode** hash_array; // an array of queue nodes
} hash_table;

typedef struct Wnode{
	struct Wnode *next, *prev;
	int page_num;
}Wnode;

typedef struct waiting_queue{
	Wnode *front, *rear;
}waiting_queue;



struct workload
{
	// Defines type of workload
	WorkloadsT type;
	// The number of pages in workload
	int pages;
	// The total size of the workload
	int size;
	// The final work to run the policy
	int * work;
};

//----------------------------------------------------------------
//----------------------------------------------------------------
/* ALL FUNCTION DEFINITIONS HERE */

struct workload * generate_workload(WorkloadsT type, int pages, int size);
struct workload * generate_random(struct workload * w);
struct workload * generate_loop(struct workload * w);
struct workload * generate_local(struct workload * w);


QNode* new_QNode(unsigned page_number); // Creating a node for the linked list
Queue* create_Queue(int total_cap); // Creating a new Queue data structure. Input is the roral capacity(cache size) of the queue
hash_table* create_Htable(int capacity); // Hash table
int AreAllFramesFull(Queue* queue); //To check if all the Queues full
int isQueueEmpty(Queue* queue); //To check if a Queue if empty

// POLICIES------------------------------------------------------------------------

// LRU
void LRU_deQueue(Queue* queue); //Dequeue a LRU
void LRU_enqueue(Queue* queue, hash_table* hash, unsigned page_num); //Enquque a LRU
void ReferencePage(Queue* queue, hash_table* hash, unsigned page_num, int *hit); // Refering to the hash table
float policy_LRU(struct workload * w, int cache_size); //Main policy LRU function

// FIFO
void FIFO_dequeue(Queue* queue); //Dequeue a FIFO queue
void FIFO_Enqueue(Queue* queue, hash_table* hash, unsigned pageNumber, int *hit); //Enqueue a FIFO queue
float policy_FIFO(struct workload * w, int cache_size); //Main function of FIFO

// Random
float policy_RANDOM(struct workload * w, int cache_size);  //Main function of Random

// Approximate-LRU
void appLRU_deQueue(Queue* queue, hash_table* hash); //Dequeue
void appLRU_enqueue(Queue* queue, hash_table* hash, unsigned page_num,int* hit); //Enqueue
void approx_ref_page(Queue* queue, hash_table* hash, unsigned page_num, int* hit); //Reference page
float policy_LRUapprox(struct workload * w, int cache_size); //Main function

#endif /* __DEF_H__ */
