#include <gtk/gtk.h>

void add_list_item(GtkWidget *listbox, char *filename);

int main(int argc, char *argv[]) {
	
	GtkWidget *window;
	GtkWidget *header;
	GtkWidget *open_button;
	GtkWidget *export_button;
	GtkWidget *hbox;
	GtkWidget *image;
	GtkWidget *listbox;

	gtk_init(&argc, &argv);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_set_default_size(GTK_WINDOW(window), 640, 480);
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	header = gtk_header_bar_new();
	gtk_header_bar_set_title(GTK_HEADER_BAR(header), "Dash Texture Viewer");
	gtk_header_bar_set_show_close_button(GTK_HEADER_BAR(header), TRUE);
	gtk_window_set_titlebar(GTK_WINDOW(window), header);
	
	open_button = gtk_file_chooser_button_new("Open", GTK_FILE_CHOOSER_ACTION_OPEN);
	gtk_header_bar_pack_start(GTK_HEADER_BAR(header), open_button);
	
	hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 20);

	export_button = gtk_button_new_with_label("Export");
	gtk_header_bar_pack_start(GTK_HEADER_BAR(header), export_button);
	gtk_container_add(GTK_CONTAINER(window), hbox);

	listbox = gtk_list_box_new();
	gtk_list_box_set_selection_mode(GTK_LIST_BOX(listbox), GTK_SELECTION_BROWSE);

	add_list_item(listbox, "Texture 000");
	add_list_item(listbox, "Texture 001");
	add_list_item(listbox, "Texture 002");
	add_list_item(listbox, "Texture 003");
	add_list_item(listbox, "Texture 004");
	add_list_item(listbox, "Texture 005");
	add_list_item(listbox, "Texture 006");
	add_list_item(listbox, "Texture 007");
	add_list_item(listbox, "Texture 008");
	add_list_item(listbox, "Texture 009");
	
	gtk_box_pack_start(GTK_BOX(hbox), listbox, TRUE, TRUE, 20);

	image = gtk_image_new_from_file("img/placeholder.png");
	gtk_box_pack_start(GTK_BOX(hbox), image, TRUE, TRUE, 0);

	gtk_widget_show_all(window);
	gtk_main();

	return 0;

}

void add_list_item(GtkWidget *listbox, char *filename) {

	GtkWidget *row;
	GtkWidget *label;

	label = gtk_label_new(filename);

	row = gtk_list_box_row_new();
	gtk_container_add(GTK_CONTAINER(row), label);
	gtk_container_add(GTK_CONTAINER(listbox), row);
	gtk_widget_show(row);

}
