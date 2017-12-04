/**
 * File : main.c
 * Auth : Eric Harlow
 * https://github.com/steshaw/gtk-examples/blob/master/ch04.button.edit.combo/listbox.c
 * Apapdted to GTK3 By Benjamin Collins
 *
 * Show the listbox widget in use
 **/

#include <gtk/gtk.h>

void add_list_item(GtkWidget *listbox, char *sText);

int main(int argc, char *argv[]) {

	GtkWidget *window;
	GtkWidget *listbox;

	gtk_init(&argc, &argv);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	listbox = gtk_list_box_new();
	gtk_list_box_set_selection_mode(GTK_LIST_BOX(listbox), GTK_SELECTION_BROWSE);

	gtk_widget_show(listbox);

	add_list_item(listbox, "This is a listbox");
	add_list_item(listbox, "Quite useful....");
	add_list_item(listbox, "When it needs to be.");
	add_list_item(listbox, "This list can be");
	add_list_item(listbox, "Quite long you know");

	gtk_container_add(GTK_CONTAINER(window), listbox);
	gtk_widget_show(window);

	gtk_main();
	return 0;

}

void add_list_item(GtkWidget *listbox, char *sText) {

	GtkWidget *row;
	GtkWidget *label;

	label = gtk_label_new(sText);

	row = gtk_list_box_row_new();
	gtk_container_add(GTK_CONTAINER(row), label);
	gtk_container_add(GTK_CONTAINER(listbox), row);
	gtk_widget_show_all(row);

}
