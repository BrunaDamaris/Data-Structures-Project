#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include "compress.h"
#include "tree.h"
#include "hash_tree.h"
#include "hash_table.h"

gdouble fr;
gdouble fractionfirst;

int is_bit_i_set(unsigned char compbyte,int i)
{
	unsigned char mask = 1<<i;
	return mask & compbyte;
}

int check_bit(tree *hufftree,unsigned char compbyte,int index)
{
	while(index>=0)
	{
		if(is_bit_i_set(compbyte,index))
		{
		//	printf("1");
			if(hufftree != NULL) hufftree = hufftree -> right;
		}
		else
		{
		//	printf("0");
			if(hufftree != NULL) hufftree = hufftree -> left;
		}
		index--;
	}
	if(hufftree == NULL) return 1;
	else return 0;
}

unsigned char print_compress(tree *hufftree, unsigned char compbyte, unsigned int *indexgeral,unsigned int *indexcurrent,unsigned char byteprevious, FILE *file_out)
{
	//printf("compress_7\n");
	unsigned char mask = 0;
	unsigned char byte = 0;
	unsigned int index = 7;
	unsigned int aux = 7;
	
	while(!(is_bit_i_set(compbyte,index)) && index > 0) index--;

	//printf("compress_8\n");
	index ++;
	//printf("compress_9\n");
	set_bit(mask,index);
	//printf("compress_10\n");
	while(!(check_bit(hufftree,compbyte,index))) index++;
	//printf("compress_11\n");
	index--;
	//printf("compress_12\n");
	aux = *indexgeral;
	//printf("compress_13\n");
	if(*indexcurrent > 0)
	{

		index = *indexcurrent;
	}
	//printf("compress_14\n");
	while(index >= 0 && aux >= 0)
	{
		if(is_bit_i_set(compbyte,index)) byte = set_bit(byte,aux);

		if(index > 0) index--;

		else{
			if(aux > 0) aux--;
			else aux = 7;
			*indexgeral = aux;
			*indexcurrent = index;
			return byte + byteprevious;
		}
		if(aux > 0) aux--;
		else
		{
			aux = 7;
			byte += byteprevious;
			fprintf(file_out,"%c",byte);
			*indexgeral = aux;
			*indexcurrent = index;
			byte = 0;
			byteprevious = 0;
		}
	}
}


static gboolean fill(gpointer dt)
{
	double now = 0.000000;
	GtkWidget* pro = dt;
	fr = fr+fractionfirst;
	if(fr < 1.000000)
	{
		now = 1.0-fr;
		fr += now;
		gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(pro),fr);
		return TRUE;
	}
	return FALSE;
}
// unsigned char compress(tree *hufftree, hash_tree *hashtree,char filename[], FILE *file_out)
// {
// 	int index;
// 	unsigned char key = 0;
// 	unsigned int *index_geral_freq,*index_current_freq;
// 	unsigned int value1,value2,trash;
// 	unsigned char filebyte,*first;
// 	unsigned char byte = 0,b;
// 	first = &b;
// 	*first=0;
// 	//printf("compress_1\n");
// 	index_geral_freq = &value1;
// 	index_current_freq = &value2;
// 	//printf("compress_2\n");
// 	FILE *file_in;
// 	file_in = fopen(filename, "rb");
// 	//printf("compress_3\n");
// 	//printf("\n");
// 	*index_geral_freq = 7;
// 	*index_current_freq = 0;
// 	//printf("compress_4\n");
// 	while(!feof(file_in))
// 	{
// 		if(fscanf(file_in,"%c",&filebyte) != EOF)
// 		{
// 			if(filebyte != 0xff && filebyte != 0x0a)
// 			{
// 				key = create_index(&filebyte);

// 				byte = print_compress(hufftree,hashtree->trees[key]->path_bits,index_geral_freq,index_current_freq,byte,file_out);
					
// 				if(*index_geral_freq == 7)
// 				{
// 					fprintf(file_out,"%c",byte);
// 					byte = 0;
// 				}
// 			}
// 		}
// 	}
// 	*index_geral_freq = *index_geral_freq - 1; 	/// pois na função print compress da return com o indice ainda no bit 
						   
// 	fprintf(file_out,"%c",byte);
// 	rewind(file_out);

// 	trash = 8 - *index_geral_freq;
// 	*first = trash << 5;
// 	fclose(file_in);
// 	return *first;
// }

unsigned char  compress(tree *hufftree, hash_tree *hashtree,char filename[], FILE *file_out, gdouble fraction,GtkWidget* progress)
{
	int index;
	unsigned char key = 0;
	unsigned int *index_geral_freq,*index_current_freq;
	unsigned int value1,value2,trash;
	unsigned char filebyte,*first;
	unsigned char byte = 0,b;
	first = &b;
	*first = 0;
	//printf("compress_1\n");
	index_geral_freq = &value1;
	index_current_freq = &value2;
	//printf("compress_2\n");
	FILE *file_in;
	file_in = fopen(filename, "rb");
	//printf("compress_3\n");
	//printf("\n");
	*index_geral_freq = 7;
	*index_current_freq = 0;
	double now = 0.000000;
	fractionfirst = fraction;
	fr = fraction;
	//printf("compress_4\n");
	gtk_progress_bar_set_text(GTK_PROGRESS_BAR(progress),"Compactando...");
	while(!feof(file_in))
	{
		if(fscanf(file_in,"%c",&filebyte) != EOF)
		{
			g_timeout_add(500,fill,GTK_PROGRESS_BAR(progress));

			if(filebyte != 0xff && filebyte != 0x0a)
			{
				key = create_index(&filebyte);

				byte = print_compress(hufftree,hashtree->trees[key]->path_bits,index_geral_freq,index_current_freq,byte,file_out);
					
				if(*index_geral_freq == 7)
				{
					fprintf(file_out,"%c",byte);
					byte = 0;
				}
			}
		}
	}
	*index_geral_freq = *index_geral_freq - 1; 	/// pois na função print compress da return com o indice ainda no bit 
						   
	fprintf(file_out,"%c",byte);
	rewind(file_out);

	trash = 8 - *index_geral_freq;
	*first = trash << 5;
	fclose(file_in);
	return *first; 
}