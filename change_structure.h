#ifndef CHANGE_STRUCTURE_H
#define CHANGE_STRUCTURE_H
#include <stdio.h>
#include <stdlib.h>
#include "heap.h"
#include "hash_table.h"
#include "tree.h"
#include "hash_tree.h"



void hashtoheap(heap *heap,hash_table *hashtable);//passa os elementos que estão na hash para a heap,(ou ao menos tenta)
void treetohash(tree *hufftree, hash_tree *hashtree, unsigned char compbyte, unsigned int *size);

#endif