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
	void *aux = NULL;
	hash_table *hash = create_hash_table();
	heap *heap = create_heap();
	hash_tree* hash2 = create_hash_tree();
	tree *hufftree = NULL;
	unsigned int truesize, index, *size=0,trash,header=0;
	FILE *file_in;
	FILE *file_out;
	char letter;
	char filename_out[MAX_SIZE];
	double contbytes;
	gdouble fraction;
	GtkWidget* progress;
	size = malloc(sizeof(unsigned int));
	*size = 0;

	//printf("Digite o nome do seu arquivo:\n");
	//scanf(" %[^\n]s", filename);
	

	file_in = fopen(filename, "rb");
	while(!feof(file_in))
	{
		filebyte = fgetc(file_in);
		contbytes += 1.0;
		//ler valores está funcionando
		//if(value != 0xff && value != 0xa && value != 0x9) 
		if(filebyte != 0xff && filebyte != 0x0a)
		{
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
  	//parei aqui
	heapsort(heap); // ordena a heap
	heap->size = truesize;
  	//cria a arvore
  	hufftree = build_hufftree(hufftree, heap);
 	//printf("passe4\n");
  	//imprimindo
  	//print_in_order(hufftree);
  	//tree para hash
  	treetohash(hufftree,hash2,0,size);
  	//barra de progresso
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
 	print_in_order(hufftree,file_out);
	//manda comprimir
  	trash = compress(hufftree,hash2,filename,file_out,fraction,progress);
  	//printa o header
  	trash = trash << 8;
  	header = trash + (*size);
  	printf("%d\n",header);
  	fprintf(file_out,"%c",header);
  	fclose(file_out);
  	
}
