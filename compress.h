#ifndef COMPRESS_H
#define COMPRESS_H
#include "tree.h"
#include "hash_tree.h"
#include "hash_table.h"
#include <gtk/gtk.h>

int is_bit_i_set(unsigned char comp,int i);

int check_bit(tree *hufftree,unsigned char comp,int index);

unsigned char print_compress(tree *hufftree, unsigned char comp, unsigned int *indexgeral,unsigned int *indexcurrent,unsigned char byteprevious, FILE *file_out,unsigned int height);

void put_trash(FILE *file_out, unsigned int trash);

unsigned char compress(tree *hufftree, hash_tree *hashtree,char filename[], FILE *file_out, gdouble fraction,GtkWidget* progress);

#endif 