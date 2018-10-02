#ifndef DECOMPRESS_H
#define DECOMPRESS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include "compress.h"
#include "tree.h"
#include "hash_tree.h"
#include "hash_table.h"
#include "interface.h"


void decompress(char filename[]);

#endif