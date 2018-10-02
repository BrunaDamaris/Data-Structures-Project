#ifndef INTERFACE_H
#define INTERFACE_H
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <gtk/gtk.h>
#include "huffman_1_4.h"
#include "decompress.h"
#define MAX_SIZE 1000000

gdouble fraction_cont(double contbytes);
GtkWidget* progressbar(GtkWidget* progress);
void create_dialog_compress(GtkWindow* window,gpointer data);
void create_dialog_decompress(GtkWindow* window,gpointer data);
void compactar();
void descompactar();
void about();
int main(int argc, char *argv[]);

#endif