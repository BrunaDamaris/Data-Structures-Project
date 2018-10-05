#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include "hash_table.h"
#include "heap.h"
#include "tree.h"	
#include "hash_table.h"
#include "compress.h"
#include "change_structure.h"
#include "interface.h"
#include "decompress.h"
#define MAX_SIZE 1000000


void start(char filename[])
{
	int i;
	unsigned char filebyte,byte = 0;
	hash_table *hash = create_hash_table();
	heap *heap = create_heap();
	hash_tree* hash2 = create_hash_tree();
	tree *hufftree = NULL;
	unsigned int truesize, index, *size,trash,header=0;
	FILE *file_in;
	FILE *file_out;
	char filename_out[MAX_SIZE];
	double contbytes;
	gdouble fraction;
	GtkWidget* progress;
	size = malloc(sizeof(unsigned int));
	*size = 0;
  int flag1 = 0,flag2 = 0;

	file_in = fopen(filename, "rb");
	while(fscanf(file_in,"%c", &filebyte) != EOF)
	{
		contbytes += 1.0;
		//ler valores está funcionando
		//if(value != 0xff && value != 0xa && value != 0x9) 
		if(filebyte != 0xff && filebyte != 0x0a)
		{
      if(filebyte == 42)
      {
        flag1 = 1;
      }
      if(filebyte == 92)
      {
        flag2 = 1;
      }
			put(hash, &filebyte); // insere o elemento na hash
		} // insere o elemento na hash
		//inserir na hash está funcionando
	}
	fclose(file_in);
	fraction = fraction_cont(contbytes);
	
	//print_hash(hash);
 	// transfere a hash para a heap
  	hashtoheap(heap,hash); // envia os elementos da hash para a heap
  	//ordena a heap
  	truesize = heap->size;
  	i=1;
  	while(i <= heap->size)
  	{
  		printf("%c = %Ld\n", *((unsigned char*)heap->tree[i]->byte), heap->tree[i]->freq);
  		i++;
  	}
  	//parei aqui
	heapsort(heap); // ordena a heap
	heap->size = truesize;
  	//cria a arvore
  	hufftree = build_hufftree(hufftree, heap);
 	//printf("passe4\n");
  	//imprimindo
  	//print_in_order(hufftree);
  	//tree para hash
  	treetohash(hufftree,hash2,0,size,0);
  	//barra de progresso
  	print_hash_tree(hash2);
  	progress = progressbar(progress);
  	//cria um arquivo novo que possuíra o arquivo comprimido
  	index = 0;
  	while(filename[index] != '\0')
  	{
  	 filename_out[index] = filename[index];
  	 index++;
  	}
  	index=0;
  	while(filename_out[index] != '\0') index++;
  	index--;
  	filename_out[index+2] = '\0';
  	filename_out[index+1] = 'f';
 	filename_out[index] = 'f';
 	filename_out[index-1] = 'u';
 	filename_out[index-2] = 'h';
 	filename_out[index-3] = '.';
  	
  	file_out = fopen(filename_out,"wb");
	
  	//printa dois bits vazios
	fprintf(file_out,"%c",byte);
	fprintf(file_out,"%c",byte);
 	//printa a arvore
 	print_in_order(hufftree,file_out,flag1,flag2);
	//manda comprimir
  	trash = compress(hufftree,hash2,filename,file_out,fraction,progress);
  	//printa o header
  	//trash = trash << 8;
  	header = trash + (*size);
  	fprintf(file_out,"%c",trash);
  	fprintf(file_out,"%c",*size);
  	//printf("%d---%d\n",trash,*size);
  	//printf("%d\n",header);
  	//fprintf(file_out,"%c",header);
  	fclose(file_out);
  	
}
