#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "trie.h"
#include <pthread.h>

// utility to convert character to corresponding index
#define CHAR_TO_INDEX(c) ((int)c - (int)'a') 
#define ARRAY_SIZE(a) sizeof(a)/sizeof(a[0])

trie_node_t init_trie_node(void);
trie_t init_trie(void){
    // Write your code here
    trie_t pNode; 
	pNode = (_trie_t*)malloc(sizeof(_trie_t)); 

	// initialize init single mutex lock
	#ifdef _S_LOCK_TRIE
	pthread_mutex_init(&pNode->sing_mut_lock ,NULL);
	#endif

	// initialize rw lock
	#ifndef _S_LOCK_TRIE
	pthread_rwlock_init(&pNode->rw_lock ,NULL);
	#endif
	// initializing the head
	pNode->head = init_trie_node(); 

	return pNode; 
} 
trie_node_t init_trie_node(void){
    // Write your code here 
	// Initializing a node
	trie_node_t trie_node = (_trie_node_t*)malloc(sizeof(_trie_node_t)); 
	if (trie_node)
	{ 
		int i; 
		trie_node->is_end=false;

		for (i = 0; i < 26; i++) 
			trie_node->children[i] = NULL; 
	} 
	// Initializing locks for HOH mechanism
	#ifndef _NO_HOH_LOCK_TRIE
    pthread_mutex_init(&trie_node->node_lock, NULL);
    #endif

	return trie_node; 
} 

void insert(trie_t trie, char* key, int value){
	int length = strlen(key);
    int index;
    int iter;

	// acquire ingle mutex lock
	#ifdef _S_LOCK_TRIE
	pthread_mutex_lock(&trie->sing_mut_lock);
	#endif
	// acquire rw lock
	#ifndef _S_LOCK_TRIE
	pthread_rwlock_wrlock(&trie->rw_lock);
	#endif
    trie_node_t x = trie->head;
	
	// acquire Locks for hoh and setting a temporary lock
	#ifndef _NO_HOH_LOCK_TRIE
    pthread_mutex_t *temp_lock=NULL;
    pthread_mutex_lock(&x->node_lock);
    #endif

    for (iter = 0; iter<length; iter++){
        index = CHAR_TO_INDEX(key[iter]);
        if(!x->children[index]){
            x->children[index] = init_trie_node(); 
			// acquire Locks for hoh
			#ifndef _NO_HOH_LOCK_TRIE
            pthread_mutex_init(&x->children[index]->node_lock,NULL);
            #endif
        }
		#ifndef _NO_HOH_LOCK_TRIE
		if(temp_lock!=NULL){
			// release Locks for hoh
			pthread_mutex_unlock(temp_lock);
		}
		temp_lock = &x->node_lock;
		#endif
        x = x->children[index];
		#ifndef _NO_HOH_LOCK_TRIE
		// acquire Locks for hoh
		pthread_mutex_lock(&x->node_lock);
		#endif

    }
    // Write your code here
	x->value = value;
    x->is_end = true;

	#ifndef _NO_HOH_LOCK_TRIE
	// release Locks for hoh
	pthread_mutex_unlock(temp_lock);
	pthread_mutex_unlock(&x->node_lock);
    #endif
	// Releasing Single Mutex Lock
	#ifdef _S_LOCK_TRIE
	pthread_mutex_unlock(&trie->sing_mut_lock);
	#endif

	// rw
	#ifndef _S_LOCK_TRIE
	pthread_rwlock_unlock(&trie->rw_lock);
	#endif

}

int find(trie_t trie,char* key,int* val_ptr){
    int level; 
	int length = strlen(key); 
	int index; 
	trie_node_t x = trie->head; 

	// init single mutex lock
	#ifdef _S_LOCK_TRIE
	pthread_mutex_lock(&trie->sing_mut_lock);
	#endif
	// rw
	#ifndef _S_LOCK_TRIE
	pthread_rwlock_rdlock(&trie->rw_lock);
	#endif
	// hoh
	#ifndef _NO_HOH_LOCK_TRIE
    pthread_mutex_t *temp_lock=NULL;
	pthread_mutex_lock(&x->node_lock);
    #endif

	for (level = 0; level < length; level++) 
	{ 			
		index = CHAR_TO_INDEX(key[level]); 
		if (!x->children[index]){

			// Releasing Single mutex lock
			#ifdef _S_LOCK_TRIE
			pthread_mutex_unlock(&trie->sing_mut_lock);
			#endif 
			// RW lock
			#ifndef _S_LOCK_TRIE
			pthread_rwlock_unlock(&trie->rw_lock);
			#endif
			// hoh
			#ifndef _NO_HOH_LOCK_TRIE
			if(temp_lock!=NULL) pthread_mutex_unlock(temp_lock);
			pthread_mutex_unlock(&x->node_lock);
			#endif

			return -1; 
		}
		// hoh
		#ifndef _NO_HOH_LOCK_TRIE
		if(temp_lock!=NULL){
			pthread_mutex_unlock(temp_lock);
		}
		temp_lock = &x->node_lock;
		#endif
		x =x->children[index]; 
		// hoh
		#ifndef _NO_HOH_LOCK_TRIE
		pthread_mutex_lock(&x->node_lock);
		#endif
	} 

	if(x->is_end){
		*val_ptr = x->value;

		// Releasing Single mutex lock
		#ifdef _S_LOCK_TRIE
		pthread_mutex_unlock(&trie->sing_mut_lock);
		#endif 
		// rw
		#ifndef _S_LOCK_TRIE
		pthread_rwlock_unlock(&trie->rw_lock);
		#endif
		// hoh
		#ifndef _NO_HOH_LOCK_TRIE
		if(temp_lock!=NULL) pthread_mutex_unlock(temp_lock);
		pthread_mutex_unlock(&x->node_lock);
		#endif
		return 0;
	}
	// hoh
	#ifndef _NO_HOH_LOCK_TRIE
	pthread_mutex_unlock(temp_lock);
	pthread_mutex_unlock(&x->node_lock);
	#endif
	// Single
	#ifdef _S_LOCK_TRIE
	pthread_mutex_unlock(&trie->sing_mut_lock);
	#endif 
	// rw
	#ifndef _S_LOCK_TRIE
	pthread_rwlock_unlock(&trie->rw_lock);
	#endif

    return -1;
} 

void delete_kv(trie_t trie, char* key){
    int level; 
	int length = strlen(key); 
	int index; 
	// single
	#ifdef _S_LOCK_TRIE
	pthread_mutex_lock(&trie->sing_mut_lock);
	#endif
	// rw
	#ifndef _S_LOCK_TRIE
	pthread_rwlock_wrlock(&trie->rw_lock);
	#endif
	trie_node_t x = trie->head; 
	// hoh
	#ifndef _NO_HOH_LOCK_TRIE
	pthread_mutex_t *temp_lock=NULL;
	pthread_mutex_lock(&x->node_lock);
    #endif

	int del_check = 1;
	for (level = 0; level < length; level++) 
	{ 	
		
		index = CHAR_TO_INDEX(key[level]); 
		if (!x->children[index]){
			del_check = 0;
			break;
		}
		// hoh
		#ifndef _NO_HOH_LOCK_TRIE
		if(temp_lock!=NULL){
			pthread_mutex_unlock(temp_lock);
		}
		temp_lock= &x->node_lock;
		#endif
		x =x->children[index]; 
		// hoh
		#ifndef _NO_HOH_LOCK_TRIE
		pthread_mutex_lock(&x->node_lock);
		#endif 
	} 

	if(x != NULL && x->is_end && del_check){
		x->is_end = false;
	}
	// hoh
	#ifndef _NO_HOH_LOCK_TRIE
	pthread_mutex_unlock(temp_lock);
	pthread_mutex_unlock(&x->node_lock);
	#endif
	// single
	#ifdef _S_LOCK_TRIE
	pthread_mutex_unlock(&trie->sing_mut_lock);
	#endif
	// rw
	#ifndef _S_LOCK_TRIE
	pthread_rwlock_unlock(&trie->rw_lock);
	#endif

}

// A recursive function to traverse to the trie data structure and finding the words
// with givn prefix. Essentially a DFS.
void recurse_prefix(trie_node_t t, char ***list, char* word, int *list_count){

	char **temp_list = *list;
	trie_node_t temp_node = t;
	if(temp_node->is_end){
		char *store = malloc(strlen(word)*sizeof(char)+1);

		strcpy(store,word);
		temp_list[*list_count] = store;
		*list_count = *list_count+1;
	}
	
	for(int i = 0; i < 26; i++){
		if(temp_node->children[i]){

			char *temp = malloc((strlen(word)+1)*sizeof(char) + 1);
			strcpy(temp, word);
			char new_char = i+(int)'a';
			strncat(temp, &new_char,1);
			recurse_prefix(temp_node->children[i], &temp_list, temp, list_count);
			free(temp);
		}
	}


}

char** keys_with_prefix(trie_t trie, char* prefix){
    // Write your code here
	// single
	#ifdef _S_LOCK_TRIE
	pthread_mutex_lock(&trie->sing_mut_lock);
	#endif
	// rw
	#ifndef _S_LOCK_TRIE
	pthread_rwlock_rdlock(&trie->rw_lock);
	#endif

    char** list = malloc(sizeof(char*)*1000);
	int length = strlen(prefix);
	int list_count = 0;
	int index;
	trie_node_t root = trie->head;
	for(int i=0; i<length; i++){
		index = CHAR_TO_INDEX(prefix[i]);
			if(root->children[index] == NULL){
				list[0] = NULL;
				// Releasing Single Mutex Lock
				#ifdef _S_LOCK_TRIE
				pthread_mutex_unlock(&trie->sing_mut_lock);
				#endif

				#ifndef _S_LOCK_TRIE
				pthread_rwlock_unlock(&trie->rw_lock);
				#endif
				return list;
			}		
			root = root->children[index];
	}
	recurse_prefix(root,&list,prefix,&list_count);
	list[list_count]= NULL;

	// Releasing Single Mutex Lock
	#ifdef _S_LOCK_TRIE
	pthread_mutex_unlock(&trie->sing_mut_lock);
	#endif
	
	#ifndef _S_LOCK_TRIE
	pthread_rwlock_unlock(&trie->rw_lock);
	#endif
	
	return list;
}
// Recursive function to delete the trie data structure
void recurse_delete(trie_node_t node){
	trie_node_t temp = node;
	for(int i=0;i<26;i++){
		if(temp->children[i]){
			recurse_delete(temp->children[i]);
			temp->children[i] = NULL;
		}
	}
	free(temp);
}
void delete_trie(trie_t trie){
	// single
	#ifdef _S_LOCK_TRIE
	pthread_mutex_lock(&trie->sing_mut_lock);
	#endif
	// rw
	#ifndef _S_LOCK_TRIE
	pthread_rwlock_wrlock(&trie->rw_lock);
	#endif

	trie_node_t node = trie->head;
	recurse_delete(node);
	trie->head = NULL;
	// Releasing Single Mutex Lock
	#ifdef _S_LOCK_TRIE
	pthread_mutex_unlock(&trie->sing_mut_lock);
	#endif
	// rw
	#ifndef _S_LOCK_TRIE
	pthread_rwlock_unlock(&trie->rw_lock);
	#endif
	free(trie);

    // Write your code here
}