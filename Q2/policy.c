/* INSTRUCTIONS:

This file will contain all functions related with various policies.
Each policy must return the hit rate

*/

#include "definitions.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


// UTILITY FUNCTIONS ----------------------------------------------

//  Function to create a Queue node
QNode* new_QNode(unsigned page_number)
{
	QNode* temp = NULL;
	temp = (QNode*)malloc(sizeof(QNode));
	// Adding the page number of the node.
	temp->page_num = page_number;
	// Setting next and previous elements to NULL
	temp->prev = temp->next = NULL;

	temp->approx_check = 0; // A flag ONLY for Approx-LRU
	return temp;
}

//  Function to create a Queue
// NOTE : in this implementation of the Queue, elements are added from the front
//        and removed from the rear end.
Queue* create_Queue(int total_cap)
{
	Queue* queue = (Queue*)malloc(sizeof(Queue));
	// Default size -> 0
	queue->count = 0;
	//  Setting the front and rear to NULL.
	queue->front = NULL;
	queue->rear = NULL;
	//  Setting total capacity of the Queue(Given Cache Size)
	queue->total_cap = total_cap;
	return queue;
}

// Creating a Hash table to get th position of an element of a Queue
hash_table* create_Htable(int capacity)
{	
	hash_table* hash = NULL; 
	hash = (hash_table*)malloc(sizeof(hash_table));
	hash->capacity = capacity;
	hash->hash_array = (QNode**)malloc(hash->capacity * sizeof(QNode*));	int i;
	for (i = 0; i < hash->capacity; ++i)
		hash->hash_array[i] = NULL;
	return hash;
}

// Checking if Queus is Full
int AreAllFramesFull(Queue* queue)
{
	return queue->count == queue->total_cap;
}

// Checking if Queue is empty
int isQueueEmpty(Queue* queue)
{
	return queue->rear == NULL;
}

// MAIN FUNCTIONS-------------------------------

// FIFO 
void FIFO_dequeue(Queue* queue)
{
	if (isQueueEmpty(queue))
		return;

	// If Queue has only one element.
	if (queue->front == queue->rear)
		queue->front = NULL;

	// Removing a node from the rear end
	QNode* temp = queue->rear;
	queue->rear = queue->rear->prev; //Making the penultimate node as the rear node

	if (queue->rear)
		queue->rear->next = NULL;

	free(temp); 

	// decrementing the length of Queue.
	queue->count--;
}

// Adding new elements to the Queue
void FIFO_Enqueue(Queue* queue, hash_table* hash, unsigned pageNumber, int *hit)
{
	// Check if element if already there in the Queue using Hash Table.

	// If new element if introduced:
	if(!hash->hash_array[pageNumber]){
		// If Queue is full, then remove the element in the rear from the Queue and its mapping in Hash table.
		if (AreAllFramesFull(queue)) {
			// remove page from hash
			hash->hash_array[queue->rear->page_num] = NULL;
			// Dequeue
			FIFO_dequeue(queue);
		}

		// Making a new Node and adding to the front of the Queue
		QNode* temp = new_QNode(pageNumber);
		temp->next = queue->front;

		if (isQueueEmpty(queue))
			queue->rear = queue->front = temp;
		else
		{
			queue->front->prev = temp;
			queue->front = temp;
		}

		// Add page entry to hash table
		hash->hash_array[pageNumber] = temp;

		// increment the Queue size by 1
		queue->count++;
	}else{
		// If element is already there in the Hash table then Hit!!.
		// incrementing hit by 1
		*hit+=1;
		
	}
}
// Main function for FIFO
float policy_FIFO(struct workload * w, int cache_size)
{
	float hit_rate = 0;
	int hit = 0;
	// Initializing the Queue
	Queue *q = create_Queue(cache_size);
	// Initializing the Hash table
	hash_table* hash = create_Htable(w->pages);
	
	// Iterating over the workload.
	for(int i=0;i<w->size;i++){
		FIFO_Enqueue(q,hash, w->work[i],&hit );
	}
	// Calculating Hit rate
	hit_rate = ((float)hit/w->size);
	return hit_rate;
}


// LRU

void LRU_deQueue(Queue* queue)
{
	if (isQueueEmpty(queue))
		return;
	// If Queue has only one element.
	if (queue->front == queue->rear)
		queue->front = NULL;

	// Removing a node from the rear end
	QNode* temp = queue->rear;
	queue->rear = queue->rear->prev; //Making the penultimate node as the rear node

	if (queue->rear)
		queue->rear->next = NULL;
	free(temp);
	// decrementing the length of Queue.
	queue->count--;
}

// Adding new elements to the Queue
void LRU_enqueue(Queue* queue, hash_table* hash, unsigned page_num)
{	
	// Function is called when the element to be added is not present in the Hash
	// If Queue is full, then remove the element in the rear from the Queue and its mapping in Hash table.
	if (AreAllFramesFull(queue)) {
		// remove page from hash
		hash->hash_array[queue->rear->page_num] = NULL;
		// Dequeue
		LRU_deQueue(queue);
	}

	// Making a new Node and adding to the front of the Queue
	QNode* temp = new_QNode(page_num);
	temp->next = queue->front;
	if (isQueueEmpty(queue))
		queue->rear = queue->front = temp;
	else
	{
		queue->front->prev = temp;
		queue->front = temp;
	}
	// Add page entry to hash table
	hash->hash_array[page_num] = temp;
	// increment the Queue size by 1
	queue->count++;
}

void ReferencePage(Queue* queue, hash_table* hash, unsigned page_num, int* hit)
{
	// Checking if the given page number is there in Hash
	// if not then Enqueue the new element and dequeue the rear element from the Queue
	QNode* reqPage = hash->hash_array[page_num];
	if (reqPage == NULL)
		LRU_enqueue(queue, hash, page_num);

	// Else apply the policies of LRU
	// If element is not in the front of the Queue, Bring it in the front
	else if (reqPage != queue->front) {
		// Hit!! because element is in the Hash
		*hit+=1;
		reqPage->prev->next = reqPage->next;
		if (reqPage->next)
			reqPage->next->prev = reqPage->prev;

		if (reqPage == queue->rear) {
			queue->rear = reqPage->prev;
			queue->rear->next = NULL;
		}
		reqPage->next = queue->front;
		reqPage->prev = NULL;
		reqPage->next->prev = reqPage;
		queue->front = reqPage;
	}
	// If element is in the front. Increment Hit
	else if (reqPage == queue->front)
	{	
		*hit+=1;
	}
	
}

// Main LRU function
float policy_LRU(struct workload * w, int cache_size)
{
	float hit_rate = 0;
	int hit = 0;
	// Initializing the Queue
	Queue *q = create_Queue(cache_size);
	// Initializing the Hash table
	hash_table* hash = create_Htable(w->pages);

	// Iterating over the workload.
	for(int i=0;i<w->size;i++){
		ReferencePage(q,hash, w->work[i], &hit);
	}
	// Calculating Hit rate
	hit_rate = ((float)hit/w->size);
	return hit_rate;
}

// APPROXIMATE LRU

void appLRU_deQueue(Queue* queue, hash_table* hash)
{
	if (isQueueEmpty(queue))
		return;

	QNode* temp;
	// Rotaion of the Queue element until a element 
	// with app_check(bitref) 0 is in the rear of the Queue
	int app_check = 0;
	while(!app_check){
		temp = queue->rear;
		// If Rear has approx check flag 1 then send it to the front and make the flag 0
		if(queue->rear->approx_check == 1){
			queue->rear = temp->prev;
			queue->rear->next = NULL;

			temp->prev = NULL;

			temp->next = queue->front;
			queue->front->prev = temp;
			queue->front = temp;
			// Flag ->0 Reducing the bitref once it has got a second chance
			queue->front->approx_check = 0;

		}else{
			// If rear has Flag -> 0 
			app_check = 1;
		}
	}
	// Removing the rear after rotation of the Queue to get 
	// rear element with bitref 0.
	temp = queue->rear;
	queue->rear = temp->prev;
	queue->rear->next = NULL;
	temp->prev = NULL;
	// Removing from the Hash
	hash->hash_array[temp->page_num] = NULL;
	// Decreasing the Queue length of Queue
	queue->count -- ;
	free(temp);
}

void appLRU_enqueue(Queue* queue, hash_table* hash, unsigned page_num,int* hit)
{	
	// Calling the function only if element is not in the hash and thus not in the Queue.
	int full_check = 0;
	// If Queue is full, then remove the element in the rear from the Queue and its mapping in Hash table.
	if (AreAllFramesFull(queue)) {
		// DeQueue and removal from Hash Table in the following function
		appLRU_deQueue(queue, hash);
		full_check = 1;
	}
	// Adding the new node
	QNode* temp;
	temp = new_QNode(page_num);
	if(full_check==1) temp->approx_check=1;

	temp->next = queue->front;
	if (isQueueEmpty(queue))
		queue->rear = queue->front = temp;
	else
	{
		queue->front->prev = temp;
		queue->front = temp;
	}
	// Add page entry to hash table
	hash->hash_array[page_num] = temp;
	// increment the Queue size by 1
	queue->count++;

}

void approx_ref_page(Queue* queue, hash_table* hash, unsigned page_num, int* hit)
{
	// Checking if the given page number is there in Hash
	// if not then Enqueue the new element and dequeue the element which is in the rear
	// and has bitref flag 0 or else rotate until a rear of such kind is found
	QNode* reqPage = hash->hash_array[page_num];
	if (reqPage == NULL){
		appLRU_enqueue(queue, hash, page_num,hit);
	}else{
		*hit+=1;
		reqPage->approx_check = 1;
		}
	}

// Main Approximate LRU function
float policy_LRUapprox(struct workload * w, int cache_size)
{	
	float hit_rate = 0;
	int hit = 0;
	// Initializing the Queue
	Queue *q = create_Queue(cache_size);

	// Initializing the Hash table
	hash_table* hash = create_Htable(w->pages);

	// Iterating over the workload.
	for(int i=0;i<w->size;i++){
		approx_ref_page(q,hash, w->work[i], &hit);
	}

	// Calculating Hit rate
	hit_rate = ((float)hit/w->size);
	return hit_rate;
}


// RANDOM
// Implemented using Arrays and not Linked list
float policy_RANDOM(struct workload *w, int cache_size)
{

	int hit_count = 0;
	int *cache = (int *)calloc(cache_size, sizeof(int));

	// Initializing an empty queue (cache) and setting the default values to -1
	for (int i = 0; i < cache_size; i++)
		cache[i] = -1;

	// Iterating over the workload
	for (int i = 0; i < w->size; i++)
	{
		// For randomising
		int flag = 0;
		// Checking if Queue/Cache is full
		for (int j = 0; j < cache_size; j++)
		{
			// Searching if page specified is already present
			if (w->work[i] == cache[j])
			{
				// Increment Hit
				hit_count++;
				flag = 1;
				break;
			}
		}
		// If element not in the cache
		if (flag == 0)
		{
			// Randomly taking an index and replcing the element with the input element
			int new = rand() % cache_size;
			cache[new] = w->work[i];
		}
	}
	// Calculating Hit Rate
	float hit_rate = (float)hit_count / w->size;
	free(cache);
	return hit_rate;
}