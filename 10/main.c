#include <gtk/gtk.h>

int main(int argc, char *argv[]) {

	GtkWidget *window;
	GtkWidget *header;
	GtkWidget *v_outline, *h_outline;
	GtkWidget *label, *row;
	GtkWidget *listbox;
	GtkWidget *separator;
	GtkWidget *image;
	GtkWidget *radio;
	int i;

	gtk_init(&argc, &argv);

	// Set up Window

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_set_default_size(GTK_WINDOW(window), 640, 300);
	gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	// Set Up header

	header = gtk_header_bar_new();
	gtk_header_bar_set_title(GTK_HEADER_BAR(header), "Dash Texture Viewer");
	gtk_header_bar_set_show_close_button(GTK_HEADER_BAR(header), TRUE);
	gtk_window_set_titlebar(GTK_WINDOW(window), header);

	// Vertical Outline

	v_outline = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	h_outline = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 30);

	// Pack Vertical Outline
	
	label = gtk_label_new("Copyright 2017 Dashgl.com");
	gtk_widget_set_margin_bottom(GTK_WIDGET(label), 4);
	gtk_widget_set_margin_top(GTK_WIDGET(h_outline), 20);
	gtk_widget_set_margin_bottom(GTK_WIDGET(h_outline), 20);
	gtk_widget_set_margin_start(GTK_WIDGET(h_outline), 20);
	gtk_widget_set_margin_end(GTK_WIDGET(h_outline), 20);

	// gtk_box_pack_start(GTK_BOX(v_outline), h_outline, TRUE, TRUE, 0);
	// gtk_box_pack_start(GTK_BOX(v_outline), label, FALSE, FALSE, 0);

	gtk_container_add(GTK_CONTAINER(window), h_outline);

	// Pack Horizontal Outline

	listbox = gtk_list_box_new();
	gtk_list_box_set_selection_mode(GTK_LIST_BOX(listbox), GTK_SELECTION_BROWSE);
	gtk_box_pack_start(GTK_BOX(h_outline), listbox, TRUE, TRUE, 0);

	for(i = 0; i < 10; i++) {

		label = gtk_label_new("Texture 000");
		row = gtk_list_box_row_new();
		gtk_container_add(GTK_CONTAINER(row), label);
		gtk_container_add(GTK_CONTAINER(listbox), row);

	}

	separator = gtk_separator_new(GTK_ORIENTATION_VERTICAL);
	gtk_box_pack_start(GTK_BOX(h_outline), separator, FALSE, FALSE, 0);

	image = gtk_image_new_from_file("img/placeholder.png");
	gtk_box_pack_start(GTK_BOX(h_outline), image, TRUE, TRUE, 0);

	separator = gtk_separator_new(GTK_ORIENTATION_VERTICAL);
	gtk_box_pack_start(GTK_BOX(h_outline), separator, FALSE, FALSE, 0);

	v_outline = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_box_pack_start(GTK_BOX(h_outline), v_outline, TRUE, TRUE, 0);

	radio = gtk_radio_button_new_with_label(NULL, "Pallet 001");
	gtk_box_pack_start(GTK_BOX(v_outline), radio, FALSE, FALSE, 0);

	radio = gtk_radio_button_new_with_label(NULL, "Pallet 002");
	gtk_box_pack_start(GTK_BOX(v_outline), radio, FALSE, FALSE, 0);
	
	radio = gtk_radio_button_new_with_label(NULL, "Pallet 003");
	gtk_box_pack_start(GTK_BOX(v_outline), radio, FALSE, FALSE, 0);
	
	radio = gtk_radio_button_new_with_label(NULL, "Pallet 004");
	gtk_box_pack_start(GTK_BOX(v_outline), radio, FALSE, FALSE, 0);

	radio = gtk_radio_button_new_with_label(NULL, "Pallet 005");
	gtk_box_pack_start(GTK_BOX(v_outline), radio, FALSE, FALSE, 0);

	gtk_widget_show_all(window);
	gtk_main();

	return 0;

}

