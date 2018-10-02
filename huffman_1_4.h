#ifndef HUFFMAN_1_4_H
#define HUFFMAN_1_4_H
#include <stdio.h>
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

void start(char filename[]);
gdouble fraction_cont(double contbytes);

#endif