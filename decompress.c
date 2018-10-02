#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include "compress.h"
#include "tree.h"
#include "hash_tree.h"
#include "hash_table.h"
#include "interface.h"



void decompress(char filename[])
{
	FILE* file_in;
	file_in = fopen(filename,"r");
	unsigned int newbyte;
	unsigned int trash,size ,header;

	header = fgetc(file_in);
	printf("%d\n",newbyte);
	trash = header >> 13;
	size = size<< 3;
    size = size>> 3;

	newbyte = fgetc(file_in);
	size = newbyte;
	//size = (newbyte << 5) + (fscanf(file_in,"%d",&newbyte));
	printf("%d\n",trash);
	printf("%d\n",size);

	while(!feof(file_in))
	{
		newbyte = fgetc(file_in);
	}
}