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

gdouble fraction_cont(double contbytes)
{
	gdouble fraction;
	double num;
	if(contbytes >= 10.0) num = 1000.0;
	if(contbytes >= 100.0) num = 10000.0;
	if(contbytes >= 1000.0) num = 100000.0;
	if(contbytes >= 10000.0) num = 1000000.0;
	fraction = num/contbytes;
	fraction = fraction/num;
	return fraction;
}

GtkWidget* progressbar(GtkWidget* progress)
{
	GtkWidget* window;
	GtkWidget* button;
	GtkWidget* boxv1;
	GdkRGBA color;
	GdkPixbuf* icon;

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size (GTK_WINDOW(window),50,50);
	gtk_window_set_title(GTK_WINDOW(window),"Compactar");
	gtk_container_set_border_width(GTK_CONTAINER(window),50);
	gtk_window_set_resizable(GTK_WINDOW(window),FALSE);

	boxv1 = gtk_box_new(FALSE,0);
	gtk_container_add(GTK_CONTAINER(window),boxv1);

	progress = gtk_progress_bar_new();
	gtk_progress_bar_set_text(GTK_PROGRESS_BAR(progress),"Compactando...");
	gtk_box_pack_start(GTK_BOX(boxv1),progress,FALSE,FALSE,0);

	// gdk_rgba_parse(&color,"turquoise");
 // 	gtk_widget_override_background_color(GTK_WIDGET(window),GTK_STATE_FLAG_NORMAL, &color);

 	icon = gdk_pixbuf_new_from_file ("./icon.png", NULL);
    gtk_window_set_icon (GTK_WINDOW(window),icon);

	gtk_widget_show_all(window);

	return progress;
}

void create_dialog_compress(GtkWindow* window,gpointer data)
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

void create_dialog_decompress(GtkWindow* window,gpointer data)
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
	    decompress(myfile);
	    g_free(myfile);
	}
	gtk_widget_destroy(dialog);
}

void compactar()
{
	GtkWidget* window;
	GtkWidget* button;
	GtkWidget* boxv1;
	GdkRGBA color;
	GdkPixbuf* icon;

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size (GTK_WINDOW(window), 200, 200);
	gtk_window_set_title(GTK_WINDOW(window),"Compactar");
	gtk_container_set_border_width(GTK_CONTAINER(window),200);
	gtk_window_set_resizable(GTK_WINDOW(window),FALSE);

	boxv1 = gtk_box_new(FALSE,0);
	gtk_container_add(GTK_CONTAINER(window),boxv1);

	button = gtk_button_new_with_label("Selecionar arquivo");
	g_signal_connect(G_OBJECT (button),"clicked",G_CALLBACK(create_dialog_compress),NULL);
	gtk_box_pack_start(GTK_BOX(boxv1),button,FALSE,FALSE,0);

	gdk_rgba_parse(&color,"turquoise");
 	gtk_widget_override_background_color(GTK_WIDGET(window),GTK_STATE_FLAG_NORMAL, &color);

 	icon = gdk_pixbuf_new_from_file ("./icon.png", NULL);
    gtk_window_set_icon (GTK_WINDOW(window),icon);

	gtk_widget_show_all(window);
}

void descompactar()
{
	GtkWidget* window;
	GtkWidget* button;
	GtkWidget* boxv1;
	GdkRGBA color;
	GdkPixbuf* icon;

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size (GTK_WINDOW(window), 200, 200);
	gtk_window_set_title(GTK_WINDOW(window),"Compactar");
	gtk_container_set_border_width(GTK_CONTAINER(window),200);
	gtk_window_set_resizable(GTK_WINDOW(window),FALSE);

	boxv1 = gtk_box_new(FALSE,0);
	gtk_container_add(GTK_CONTAINER(window),boxv1);

	button = gtk_button_new_with_label("Selecionar arquivo");
	g_signal_connect(G_OBJECT (button),"clicked",G_CALLBACK(create_dialog_decompress),NULL);
	gtk_box_pack_start(GTK_BOX(boxv1),button,FALSE,FALSE,0);

	gdk_rgba_parse(&color,"turquoise");
 	gtk_widget_override_background_color(GTK_WIDGET(window),GTK_STATE_FLAG_NORMAL, &color);

 	icon = gdk_pixbuf_new_from_file ("./icon.png", NULL);
    gtk_window_set_icon (GTK_WINDOW(window),icon);

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
	GdkPixbuf* icon;

	gtk_init(&argc,&argv);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	g_signal_connect(G_OBJECT(window),"destroy",G_CALLBACK(gtk_main_quit),NULL);
	gtk_window_set_default_size (GTK_WINDOW(window), 614,345);
	gtk_window_set_title(GTK_WINDOW(window),"HBP");
	gtk_container_set_border_width(GTK_CONTAINER(window),200);
	gtk_window_set_resizable(GTK_WINDOW(window),FALSE);

	boxv1 = gtk_box_new(FALSE,0);
	gtk_container_add(GTK_CONTAINER(window),boxv1);

	button1 = gtk_button_new_with_label("Compactar");
	g_signal_connect(G_OBJECT (button1),"clicked",G_CALLBACK(compactar),NULL);
	gtk_box_pack_start(GTK_BOX(boxv1),button1,FALSE,FALSE,0);
	
	button2 = gtk_button_new_with_label("Descompactar");
	g_signal_connect(G_OBJECT (button2),"clicked",G_CALLBACK(descompactar),NULL);
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

    icon = gdk_pixbuf_new_from_file("./icon.png", NULL);
    gtk_window_set_icon(GTK_WINDOW(window),icon);

	gtk_widget_show_all(window);
	gtk_main();
}