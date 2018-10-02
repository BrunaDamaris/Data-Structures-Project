#include <stdio.h>
#include <stdlib.h>
#include "change_structure.h"
#include "heap.h"
#include "hash_table.h"
#include "tree.h"
#include "hash_tree.h"



void hashtoheap(heap *heap,hash_table *hashtable)
{

	int i;
	//print_hash(hash);
	for(i = 0;i < 256;i++)
	{
	       if(hashtable->table[i]!=NULL) 
	       {
	       		//printf("ok1\n");
			enqueue(heap,hashtable->table[i]->freqbyte,hashtable->table[i]->byte);//enfileira na heap primeiro a frequencia depois o valor
	       }
	}
}

void treetohash(tree *hufftree, hash_tree *hashtree, unsigned char compbyte, unsigned int *size)
{
	if(hufftree == NULL) return;
	else if(hufftree->left == NULL && hufftree->right == NULL)
	{
		put_from_tree(hashtree,hufftree->byte,compbyte);
		*size = *size + 1;
		return;
	}
	else
	{
		compbyte = compbyte << 1;
		treetohash(hufftree->left,hashtree,compbyte,size);
		*size = *size + 1;
		compbyte = set_bit(compbyte,0);
		treetohash(hufftree->right,hashtree,compbyte,size);
		*size = *size + 1;
		compbyte = compbyte >> 1;
	}
	*size = *size - 1;
	return;
}