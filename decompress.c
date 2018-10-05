#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include "compress.h"
#include "tree.h"
#include "hash_tree.h"
#include "hash_table.h"
#include "interface.h"
typedef struct decompress_tree decompress_tree;

struct decompress_tree
{
	unsigned char byte;
	decompress_tree *left;
	decompress_tree *right;
};

decompress_tree* create_empty_decompress_tree()
{
	return NULL;
}

decompress_tree* create_decompress_tree(unsigned char byte, decompress_tree *left, decompress_tree *right)
{
	decompress_tree *new_decompress_tree = (decompress_tree*) malloc(sizeof(decompress_tree));
	new_decompress_tree->byte = byte;
	new_decompress_tree->left =	left;
	new_decompress_tree->right = right;
	return new_decompress_tree;
}

decompress_tree* add(FILE *file_in,unsigned char byte)
{
	decompress_tree* dt = create_empty_decompress_tree(byte,NULL,NULL);
	// if(dt == NULL)
	// {
	// 	dt = create_decompress_tree(byte, NULL, NULL);
	// 	dt->left = add(dt, byte, file_in);
	// 	dt->right = add(dt, byte, file_in);
	// }
	if(byte == '\\')
    {
        dt -> byte = fgetc(file_in);
    }
	if(byte == '*')
	{
		printf("%c\n",byte);
		// dt = create_decompress_tree('*', NULL, NULL);	
		dt->left = add( file_in,fgetc(file_in));
		dt->right = add( file_in,fgetc(file_in));
	}
	// else if(byte != '*')
	// {
	// 	dt = create_decompress_tree(byte, NULL, NULL);
	// 	return dt;
	// }
	return dt;
}

decompress_tree* go(FILE *file_in)
{   
    decompress_tree *dt = NULL;
    dt = add(file_in, fgetc(file_in));
    return dt;
}

void print_in_order_dt(decompress_tree *dt, FILE *file_out)
{
	if(dt != NULL)
	{
		fprintf(file_out,"%c",dt->byte);
		print_in_order_dt(dt->left, file_out);
		print_in_order_dt(dt->right, file_out);
	}
}
void print_pre_order(decompress_tree *dt)
{
	if(dt != NULL)
	{
		printf("%c",dt->byte);
		print_pre_order(dt->left);
		print_pre_order(dt->right);
	}
}

decompress_tree* search_in_tree(decompress_tree *dt,unsigned char byte,FILE* file_out)
{
	int index = 7;
	decompress_tree* auxtree = dt;
	while(index >= 0)
	{
		if(is_it_a_leaf(dt)) 
		{
			fprintf(file_out, "%c\n", dt->byte);
		}
		if(is_bit_i_set(byte,index)) auxtree = dt->right;
		else auxtree = dt -> left;
		index--;
	}
	// if(dt != NULL) printf("%c\n",dt->byte);
	// if(dt->byte == byte)
	// {
	// 	if(is_it_a_leaf(dt)) fprintf(file_out,"%c",dt->byte);
	// 	return dt;
	// }
	// else if(dt->byte > byte) return search_in_tree(dt->left,byte,file_out);
	// else return search_in_tree(dt->right,byte,file_out);
}
void decompress(char filename[])
{
	FILE* file_in;
	FILE *file_out;
	file_in = fopen(filename,"rb");
	file_out = fopen("out.txt","wb");

	decompress_tree *dt= create_empty_decompress_tree();
	decompress_tree *root= create_empty_decompress_tree();
	decompress_tree *result = create_empty_decompress_tree();
	unsigned char byte,trash, first, second;
	unsigned int size ,header;

	first = fgetc(file_in);
	second = fgetc(file_in);
	trash  = first >> 5;
	size = second;

	int cont = 0;

	//printf("%d,%d\n",trash,size);
	
	dt = go(file_in);
	
	while(size > trash)
	{
		// printf("%d\n",cont++);
		byte = fgetc(file_in);
		search_in_tree(dt,byte,file_out);
		size--;
	}
	//print_in_order_dt(dt, file_out);
	print_pre_order(dt);
	printf("\n");
}