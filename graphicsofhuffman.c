#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <gtk/gtk.h>
//#include "hash_table.h"
#define MAX_SIZE 10000

GtkWidget* progress;
gdouble fraction;

typedef struct HEAP heap;
typedef struct hash_table hash_table;
typedef struct hashtree hash_tree;
typedef struct treeelement tree_element;
typedef struct ELEMENT element;
typedef struct TREE tree;

struct TREE
{
	unsigned char data;//item
	unsigned int freq;//frequencia
	tree *left;
	tree *right;
};

struct HEAP 
{
	int begin;
    int size;// tamanho
    tree *tree[257];
};

struct ELEMENT
{
	unsigned char key;//elementos do arquivo lido
	unsigned int freq;//valores de frequencias
};

struct hash_table
{
	element *table[257];//array de ponteiros para cada estrutura element
};

struct treeelement
{
	unsigned char key;//elemento que vem da arvore
	unsigned char path_numofbits;//caminho na arvore
};

struct hashtree
{
	tree_element *trees[257];//array de ponteiros para cada estrutura tree_element
};

//hash table functions
hash_table* create_hash_table();

unsigned int create_index(unsigned char key);

void put(hash_table *hashtable, unsigned char key);

int get(hash_table *hashtable,unsigned char key);

void remove_ht(hash_table *hashtable, unsigned char key);

void print_hash(hash_table *hashtable);

//end of hash functions


//heap functions
heap* create_heap();//cria a heap

void enqueue(heap *heap, int element,unsigned char data);//coloca no array as frequencias e seus respectivos elementos

void build_heap(heap *heap);//constroi a heap

int get_parent_i(heap* heap,int i);//obtém o indice do pai

int get_left_i(heap* heap,int i);//obtém o indice da esquerda

int get_right_i(heap* heap,int i);//obtém o indice da direita

void heapfy(heap* heap,int i);//da min heapify para que a cabeça do array seja a menor frequencia sempre

void heapsort(heap* heap);//ordena as frequencias

//end of heap functions


//tree functions
tree* create_empty_tree(int freq, int data);

tree* create_huff_tree(tree *left, tree *right);

int is_empty(tree *hufftree);

tree* dequeuehp(heap* heap);

void print_in_order(tree *hufftree);

tree* build_hufftree(tree *hufftree, heap *heap);

unsigned char set_bit(unsigned char c, unsigned int i);

unsigned char set_xor_bit(unsigned char c, unsigned int i);
//end of tree functions

//hash from tree functions
hash_tree* create_hash_tree();

void put_in_tree(hash_tree *hashtree,unsigned char key,unsigned char comp);
//end of hash from tree functions

//compress functions
int is_bit_i_set(unsigned char comp,int i);

int check_bit(tree *hufftree,unsigned char comp,int index);

unsigned char print_compress(tree *hufftree, unsigned char comp, unsigned int *indexgeral,unsigned int *indexcurrent,unsigned char byteprevious);

void compress(tree *hufftree, hash_tree *hashtree,char filename[]);
//end of compress functions

//end of hash from tree functions

//change structure functions
void hashtoheap(heap *heap,hash_table *hashtable);//passa os elementos que estão na hash para a heap,(ou ao menos tenta)

void treetohash(tree *hufftree, hash_tree *hashtree, unsigned char comp);
//end of change structure functions

//gtk
void create_dialog(GtkWindow* window,gpointer data);
//main
void start();
//gtk
void progressbar();

void create_dialog(GtkWindow* window,gpointer data);

void compactar();





void start(char* filename)
{
	unsigned char value;
	hash_table *hash = create_hash_table();
	heap *heap = create_heap();
	hash_tree* hash2 = create_hash_tree();
	tree *hufftree = NULL;
	int truesize;
	FILE *file_in;
	FILE *file_out;
	char letter;
	double contbytes,num = 100.0;
	


	printf("Digite o nome do seu arquivo:\n");
	//scanf(" %[^\n]s", filename);
	

	file_in = fopen(filename, "r");
	file_out = fopen("out.txt", "w+");
	while(!feof(file_in))
	{
		value = fgetc(file_in);
		contbytes += 1.0;
		if(value != 0xff && value != 0xa && value != 0x9) put(hash, value); // insere o elemento na hash
	}
	if(contbytes >= 10.0) num = 1000.0;
	if(contbytes >= 100.0) num = 10000.0;
	if(contbytes >= 1000.0) num = 100000.0;
	if(contbytes >= 10000.0) num = 1000000.0;
	fclose(file_in);
	fraction = num/contbytes;
	fraction = fraction/num;
	//print_hash(hash);

 	// transfere a hash para a heap
  	hashtoheap(heap,hash); // envia os elementos da hash para a heap
  	//ordena a heap

  	truesize = heap->size;
	heapsort(heap); // ordena a heap
	heap->size = truesize;

  	//cria a arvore
  	hufftree = build_hufftree(hufftree, heap);
  	//imprimindo
  	print_in_order(hufftree);

  	//tree para hash
  	treetohash(hufftree,hash2,0);
  	//progresso
  	progressbar();
	//manda comprimir
  	compress(hufftree,hash2,filename);

}






///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////// hash functions ////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
hash_table* create_hash_table()
{
	int i;
	hash_table *new_hash_table = (hash_table*) malloc(sizeof(hash_table));
	for(i = 0;i < 257; i++)
	{
		new_hash_table->table[i] = NULL;
	}
	return new_hash_table;
}

unsigned int create_index(unsigned char key)
{
	unsigned int convert = key;
	return convert % 257;
}

void put(hash_table *hashtable, unsigned char key)
{
	unsigned int  index;
	index = create_index(key); // cria chave baseada no caracter
	if(hashtable->table[index] != NULL) // caso esse espaço na hash seja diferente de nulo 
	{																													 // a frequencia é aumentada
		hashtable->table[index]->freq++;
		return;
	}
	else if(hashtable->table[index]==NULL) // se for igual a nulo ele cria um novo nó
	{
		//index = create_index(index+1); // falta
		element *new_element = (element*) malloc(sizeof(element));
		new_element->key = key;
		new_element->freq = 1;
		hashtable->table[index] = new_element;
	}
}


int get(hash_table *hashtable,unsigned char key)
{
	unsigned char  index;
	index = create_index(key);
	while(hashtable->table[index]!=NULL)
	{
		if(hashtable->table[index]->key==key)
		{
			return hashtable->table[index]->freq;
		}
		index = create_index(index+1);
	}
	return 0;
}

void remove_ht(hash_table *hashtable, unsigned char key)
{
	unsigned char index;
	index = create_index(key);
	while(hashtable->table[index]!=NULL)
	{
		if(hashtable->table[index]->key==key)
		{
			free(hashtable->table[index]);
			hashtable->table[index]->key = -1;
		}
		index = create_index(index+1);
	}
}

void print_hash(hash_table *hashtable)
{
	int index;
	for(index=0;index < 257;index++)
	{
		if(hashtable->table[index]!=NULL) printf("%x = %d\n", hashtable->table[index]->key, hashtable->table[index]->freq);
	}
	printf("\n");
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////// end of hash functions ////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////// heap functions ////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
heap* create_heap()
{
    heap* new_heap = (heap*) malloc(sizeof(heap));
    new_heap->begin = 1;
    new_heap->size = 0;
    return new_heap;
}

void enqueue(heap *heap,int freq,unsigned char data)
{
	tree *new_tree = create_empty_tree(freq, data);
	heap->tree[++heap->size] = new_tree;
}

void build_heap(heap *heap)
{
    int i;
    for(i = (heap->size)/2;i;i--)
    {
        heapfy(heap,i);
    }
}

int get_parent_i(heap* heap,int i)
{
    int result = i/2;
    return result;
}
int get_left_i(heap* heap,int i)
{
    int result = 2*i;
    return result;
}
int get_right_i(heap* heap,int i)
{
    int result = 2*i + 1;
    return result;
}
void heapfy(heap* heap,int i)
{
    int smallest,left_index,right_index;
    tree *auxz = NULL;
  	left_index = get_left_i(heap,i); // encontra o filho esquerdo
    right_index = get_right_i(heap,i); // encontra o filho direito
    
  	if((left_index <= heap->size) &&  (heap->tree[left_index]->freq < heap->tree[i]->freq)) // Verifica se o filho esquerdo é valdio
    {																																						// e se sua frequencia é menor que a  
        smallest = left_index;																									// do pai
    }
    else
    {
        smallest = i;                                                         // assume que o menor é o pai
    }
    if((right_index <= heap->size) && (heap->tree[right_index]->freq < heap->tree[smallest]->freq)) // verifica se o direito é menor 
    {																																										// que o menor
        smallest = right_index;
    }
    if((heap->tree[i]->freq != heap->tree[smallest]->freq))                               // se a frequencia do indice 
    {																																					// não for a menor ele realiza a troca
        
    	auxz = heap->tree[i];
        heap->tree[i] = heap->tree[smallest];
        heap->tree[smallest] = auxz;
        heapfy(heap,smallest);
    }
}
void heapsort(heap* heap)
{
	tree *hufftree = NULL;
	tree *left = NULL;
	tree *right = NULL;
	tree *aux = NULL;
	build_heap(heap); // constroi a heap
     
  	int current,last,current2;
    for(current = heap->size;current >= heap->begin;current--)
    {
     //   printf("%d\n", heap->tree[heap->begin]->freq);
        aux = heap->tree[current];
        heap->tree[current] = heap->tree[heap->begin];
        heap->tree[heap->begin] = aux;
        heap->size--;
        heapfy(heap,heap->begin);
    } 
 
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////// end of heap functions ////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////// tree functions //////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
tree* create_empty_tree(int freq, int data)
{
	tree *new_tree = (tree*) malloc(sizeof(tree));
	new_tree->freq = freq;
	new_tree->data = data;
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
	new_tree->data = 19;
	return new_tree;
}

int is_empty(tree *hufftree)
{
	if(hufftree == NULL)return 1;
	else return 0;
}

tree* dequeuehp(heap *heap)
{
        tree *item = heap->tree[heap->size];
        heap->size--;
        heapfy(heap,heap->begin);
        return item;
    
}

void print_in_order(tree *hufftree)
{
	
	if(!is_empty(hufftree))
	{
		if(hufftree->data !=19)printf("[%c]", hufftree->data);
		else printf("*");
		print_in_order(hufftree->left);
		print_in_order(hufftree->right);
	}
	
}

tree* build_hufftree(tree *hufftree, heap *heap)
{
	tree *aux = NULL;
	tree *aux2 = NULL;
	int truesize;
	while(heap->begin < heap->size)
    {

  		aux = NULL;
  		aux2 = NULL;

       
        aux = dequeuehp(heap);
        truesize = heap->size;
        heapsort(heap);
        heap->size = truesize;
        
        aux2 = dequeuehp(heap);
        truesize = heap->size;
        heapsort(heap);
        heap->size = truesize;
		
		if(aux2->data == 19 && aux->data != 19)
		{
			hufftree = create_huff_tree(aux2, aux);
		}
		else
		{
			hufftree = create_huff_tree(aux, aux2);
		}
        //htf = create_huff_tree(left, right);
      	//  printf("raiz%d\n", htf->freq);
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
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////// end of tree functions //////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////// hash from tree functions //////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
hash_tree* create_hash_tree()
{	
	int i;
	hash_tree *new_hash_tree = (hash_tree*) malloc(sizeof(hash_tree));
	for(i = 0;i < 257;++i)
	{
		new_hash_tree->trees[i] = NULL;
	}
	return new_hash_tree;

}
void put_from_tree(hash_tree *hashtree,unsigned char key,unsigned char comp)
{
	unsigned int  index;
	index = create_index(key); // cria chave baseada no caracter
	tree_element *new_element = (tree_element*) malloc(sizeof(tree_element));
	new_element->key = key;
	new_element->path_numofbits = comp;
	hashtree->trees[index] = new_element;	
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////// end of hash from tree functions //////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////// compress functions //////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int is_bit_i_set(unsigned char comp,int i)
{
	unsigned char mask = 1<<i;
	return mask & comp;
}

int check_bit(tree *hufftree,unsigned char comp,int index)
{
	while(index>=0)
	{
		if(is_bit_i_set(comp,index))
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

unsigned char print_compress(tree *hufftree, unsigned char comp, unsigned int *indexgeral,unsigned int *indexcurrent,unsigned char byteprevious)
{
	unsigned char mask = 0;
	unsigned char byte = 0;
	unsigned int index = 7;
	unsigned int aux = 7;

	while(!(is_bit_i_set(comp,index)) && index > 0) index--;

	
	index ++;

	set_bit(mask,index);

	while(!(check_bit(hufftree,comp,index))) index++;
	
	index--;
	
	aux = *indexgeral;
	if(*indexcurrent > 0)
	{
		index = *indexcurrent;
	}
	
	while(index >= 0 && aux >= 0)
	{
		if(is_bit_i_set(comp,index)) byte = set_bit(byte,aux);

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
			printf("...%d...", byte);
			*indexgeral = aux;
			*indexcurrent = index;
			byte = 0;
			byteprevious = 0;
		}
	}
}

void compress(tree *hufftree, hash_tree *hashtree,char filename[])
{
	int index;
	unsigned int key;
	unsigned int *index_geral_freq,*index_current_freq;
	unsigned int value1,value2;
	char value;
	unsigned char byte=0;
	index_geral_freq = &value1;
	index_current_freq = &value2;
	FILE *file_in;
	file_in = fopen(filename, "rb");
	printf("\n");
	*index_geral_freq = 7;
	*index_current_freq = 0;
	double now = 0.000000;
	double fractionfirst = fraction;
	while(!feof(file_in))
	{
		value = fgetc(file_in);
		key = create_index(value);
		gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress),fraction);
		fraction = fraction+fractionfirst;
		if(key !=255 && key !=10)
		{
			byte = print_compress(hufftree,hashtree->trees[key]->path_numofbits,index_geral_freq,index_current_freq,byte);			
		}
		if(*index_geral_freq == 7)
		{
			printf("...%d...", byte);
			byte = 0;
		}
	}
	printf("...%d...\n", byte);
	fraction = fraction-fractionfirst;
	if(fraction < 1.000000)
	{
		now = 1.0-fraction;
		fraction += now;
		gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress),fraction);
	}
	//gtk_widget_destroy(progress);
	fclose(file_in);
	printf("\n");
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////// end of compress functions //////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////// change structure functions //////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////



void hashtoheap(heap *heap,hash_table *hashtable)
{
	int i;
	//print_hash(hash);
	for(i = 0;i < 257;i++)
	{
       if(hashtable->table[i]!=NULL) 
       {
		    enqueue(heap,hashtable->table[i]->freq,hashtable->table[i]->key);//enfileira na heap primeiro a frequencia depois o valor
       }
	}
}

void treetohash(tree *hufftree, hash_tree *hashtree, unsigned char comp)
{
	if(hufftree == NULL)
	{
		return;
	}
	else if(hufftree->data != 19)
	{
		//printf("comp : %d data : %d\n", comp, htf->data);
		put_from_tree(hashtree,hufftree->data,comp);
		//comp ^= 1;// comp--
		//comp = comp<<1;
		return;
	}
	else
	{
		//printf("%d %x\n", comp, htf->data);
		comp = comp<<1;
		treetohash(hufftree->left,hashtree,comp);
		//printf("%d %x\n", comp, htf->data);
		//comp = comp>>1;
		//comp = comp>>1;
		comp = set_bit(comp,0);
		treetohash(hufftree->right,hashtree,comp);
		// comp = set_xor_bit(comp,0);
		//comp ^= 1;
		comp = comp>>1;
	}
	return;

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////// change structure functions //////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void progressbar()
{
	GtkWidget* window;
	GtkWidget* button;
	GtkWidget* boxv1;
	GtkWidget *label;

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size (GTK_WINDOW(window),50,50);
	gtk_window_set_title(GTK_WINDOW(window),"Compactando");
	gtk_container_set_border_width(GTK_CONTAINER(window),50);

	boxv1 = gtk_box_new(FALSE,0);
	gtk_container_add(GTK_CONTAINER(window),boxv1);

	progress = gtk_progress_bar_new();
	gtk_box_pack_start(GTK_BOX(boxv1),progress,FALSE,FALSE,0);

	label = gtk_label_new("Compactando...");
	gtk_box_pack_start(GTK_BOX(boxv1),label,FALSE,FALSE,0);

	gtk_widget_show_all(window);
}

void create_dialog(GtkWindow* window,gpointer data)
{
	GtkWidget* dialog;
	GtkWindow* parent_window;
	GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
	gint res;
	GtkFileChooser *chooser;
	char* myfile;

	dialog = gtk_file_chooser_dialog_new("Selecione um arquivo",GTK_WINDOW(window),action,("_Cancel"),GTK_RESPONSE_CANCEL,("_Open"),GTK_RESPONSE_ACCEPT,NULL);

	res = gtk_dialog_run(GTK_DIALOG(dialog));

	if(res == GTK_RESPONSE_ACCEPT)
	{
	    GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
	    myfile = gtk_file_chooser_get_filename(chooser);
	    puts(myfile);
	    start(myfile);
	    g_free(myfile);
	}
	gtk_widget_destroy(dialog);
}

void compactar()
{
	GtkWidget* window;
	GtkWidget* button;
	GtkWidget* boxv1;

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size (GTK_WINDOW(window), 200, 200);
	gtk_window_set_title(GTK_WINDOW(window),"Compactar");
	gtk_container_set_border_width(GTK_CONTAINER(window),200);

	boxv1 = gtk_box_new(FALSE,0);
	gtk_container_add(GTK_CONTAINER(window),boxv1);

	button = gtk_button_new_with_label("Selecionar arquivo");
	g_signal_connect(G_OBJECT (button),"clicked",G_CALLBACK(create_dialog),NULL);
	gtk_box_pack_start(GTK_BOX(boxv1),button,FALSE,FALSE,0);

	gtk_widget_show_all(window);
}

void about()
{
	GtkWidget* dialog;
	GdkPixbuf* logo;
	const gchar *name = "HBT";
	const gchar *version = "1.4";
	const gchar *copyright = "2018 Crypto Industries";
	const gchar *comments = "Deu trabalho sim!";
	const gchar *license = "Licenciado por : Não sei";
	const gchar *website = "https://developer.gnome.org/gtk3/stable/GtkAboutDialog.html#gtk-about-dialog-new";
	const gchar *author[4];
	const gchar *author1 = "Bruna Damaris";
	const gchar *author2 = "Heitor Almeida";
	const gchar *author3 = "Paulo Cavalcante";
	const gchar *author4 = NULL;
	author[0] = author1;
	author[1] = author2;
	author[2] = author3;
	author[3] = author4;
	const gchar **authors = author;
	
	logo = gdk_pixbuf_new_from_file ("./logo.png", NULL);

	dialog = gtk_about_dialog_new();


	gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(dialog),version);

	gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(dialog),copyright);

	gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(dialog),comments);

	gtk_about_dialog_set_program_name(GTK_ABOUT_DIALOG(dialog),name);

	gtk_about_dialog_set_license(GTK_ABOUT_DIALOG(dialog),license);

	gtk_about_dialog_set_website(GTK_ABOUT_DIALOG(dialog),website);

	gtk_about_dialog_set_authors(GTK_ABOUT_DIALOG(dialog),authors);

	gtk_about_dialog_set_logo(GTK_ABOUT_DIALOG(dialog),logo);

	gtk_dialog_run(GTK_DIALOG(dialog));

	gtk_widget_destroy(dialog);
}

int main(int argc, char *argv[])
{
	GtkWidget* window;
	GtkWidget* button1;
	GtkWidget* button2;
	GtkWidget* button3;
	GtkWidget* boxv1;
	GdkRGBA color;
	GdkRGBA color1;

	gtk_init(&argc,&argv);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	g_signal_connect(G_OBJECT(window),"destroy",G_CALLBACK(gtk_main_quit),NULL);
	gtk_window_set_default_size (GTK_WINDOW(window), 614,345);
	gtk_window_set_title(GTK_WINDOW(window),"HBP");
	gtk_container_set_border_width(GTK_CONTAINER(window),200);

	boxv1 = gtk_box_new(FALSE,0);
	gtk_container_add(GTK_CONTAINER(window),boxv1);

	button1 = gtk_button_new_with_label("Compactar");
	g_signal_connect(G_OBJECT (button1),"clicked",G_CALLBACK(compactar),NULL);
	gtk_box_pack_start(GTK_BOX(boxv1),button1,FALSE,FALSE,0);
	
	button2 = gtk_button_new_with_label("Descompactar");
	g_signal_connect(G_OBJECT (button2),"clicked",G_CALLBACK(compactar),NULL);
	gtk_box_pack_start(GTK_BOX(boxv1),button2,FALSE,FALSE,0);
	
	button3 = gtk_button_new_with_label("Informações");
	g_signal_connect(G_OBJECT (button3),"clicked",G_CALLBACK(about),NULL);
	gtk_box_pack_start(GTK_BOX(boxv1),button3,FALSE,FALSE,0);

	// gdk_color_parse ("turquoise", &color);
	// gtk_widget_modify_bg(GTK_WIDGET(window), GTK_STATE_NORMAL, &color);
	gdk_rgba_parse(&color,"turquoise");
 	gtk_widget_override_background_color(GTK_WIDGET(window),GTK_STATE_FLAG_NORMAL, &color);
 	gdk_rgba_parse(&color1,"blue");

    if(GTK_IS_BIN(button1)) 
    {
        GtkWidget *children = gtk_bin_get_child(GTK_BIN(button1));
        gtk_widget_override_color(GTK_WIDGET(children),GTK_STATE_FLAG_NORMAL, &color1);
    }

    if(GTK_IS_BIN(button2))
    {
        GtkWidget *children = gtk_bin_get_child(GTK_BIN(button2));
        gtk_widget_override_color(GTK_WIDGET(children),GTK_STATE_FLAG_NORMAL, &color1);
    }

    if(GTK_IS_BIN(button3))
    {
        GtkWidget *children = gtk_bin_get_child(GTK_BIN(button3));
        gtk_widget_override_color(GTK_WIDGET(children),GTK_STATE_FLAG_NORMAL, &color1);
    }

	gtk_widget_show_all(window);
	gtk_main();
}	
