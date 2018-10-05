#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "heap.h"


tree* create_empty_tree(long long unsigned freq, void* byte)
{
	tree *new_tree = (tree*) malloc(sizeof(tree));
	new_tree->freq = freq;
	new_tree->byte = malloc(sizeof(unsigned char));
	*((unsigned char*)new_tree->byte) = *((unsigned char*)byte);
	new_tree->right = NULL;
	new_tree->left = NULL;
	return new_tree;
}

tree* create_huff_tree(tree *left, tree *right)
{
	tree *new_tree = (tree*) malloc(sizeof(tree));
	new_tree->left = left;
	new_tree->right = right;
	new_tree->freq = left->freq + right->freq;
	new_tree->byte = malloc(sizeof(unsigned char));
	*((unsigned char*)new_tree->byte) = 42;
	return new_tree;
}

int is_empty(tree *hufftree)
{
	if(hufftree == NULL)return 1;
	else return 0;
}

int is_it_a_leaf(tree* node)
{
	if((node->left) == NULL && (node->right) == NULL) 
	{
		printf("Node: %c\n",*((unsigned char*) node->byte));
		return 1;
	}
	return 0;
}

tree* dequeuehp(heap *heap)
{
    tree *item = heap->tree[heap->size];
    heap->size--;
    heapfy(heap,heap->begin);
    return item;
    
}

void print_in_order(tree *hufftree, FILE *file_out,int flag1,int flag2)
{
	
	if(!is_empty(hufftree))
	{
		// if(flag1 && flag2)
		// {
		// 	//printf("nope\n");
		// 	if(hufftree->left == NULL && hufftree->right == NULL) fprintf(file_out,"%c",*(unsigned char*)hufftree->byte);
		// 	else fprintf(file_out,"\\\\");
		// 	print_in_order(hufftree->left,file_out,flag1,flag2);
		// 	print_in_order(hufftree->right,file_out,flag1,flag2);
		// }
		// else if(flag1)
		// {
		// 	//printf("here\n");
		// 	if(hufftree->left == NULL && hufftree->right == NULL) fprintf(file_out,"%c",*(unsigned char*)hufftree->byte);
		// 	else fprintf(file_out,"\\");
		// 	print_in_order(hufftree->left,file_out,flag1,flag2);
		// 	print_in_order(hufftree->right,file_out,flag1,flag2);
		// }
		// else
		// {
		// 	//printf("wtf\n");
			if(hufftree->left == NULL && hufftree->right == NULL) fprintf(file_out,"%c",*(unsigned char*)hufftree->byte);
			else fprintf(file_out,"*");
			print_in_order(hufftree->left,file_out,flag1,flag2);
			print_in_order(hufftree->right,file_out,flag1,flag2);
		//}
	}
	
}

tree* build_hufftree(tree *hufftree, heap *heap)
{
	tree *auxtree = NULL;
	tree *auxtree2 = NULL;
	int truesize;
	while(heap->begin < heap->size)
    {
  		auxtree = NULL;
  		auxtree2 = NULL;

       
	    auxtree = dequeuehp(heap);
	    truesize = heap->size;
	    heapsort(heap);
	    heap->size = truesize;
	        
	    auxtree2 = dequeuehp(heap);
	    truesize = heap->size;
	    heapsort(heap);
	    heap->size = truesize;
	    //printf("here: %c,%c\n",(*(unsigned char*)auxtree->byte),(*(unsigned char*)auxtree2->byte));

	    if(((*(unsigned char*)auxtree2->byte) == 42 && (*(unsigned char*)auxtree->byte) != 42 && (auxtree->freq == auxtree2->freq)))
		{
			hufftree = create_huff_tree(auxtree2, auxtree);
		}
		else
		{
			hufftree = create_huff_tree(auxtree, auxtree2);
		}
		heap->tree[++heap->size] = hufftree;
	    truesize = heap->size;
	    heapsort(heap);
	    heap->size = truesize;
    }
   
    return hufftree;
}

unsigned char set_bit(unsigned char c, unsigned int i)
{
	unsigned char mask = 1 << i;
	return mask | c;
}

unsigned char set_xor_bit(unsigned char c, unsigned int i)
{
	unsigned char mask = 1 << i;
	return mask ^ c;
}