#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <png.h>

struct DashTextureHeader {
	int32_t data_type;
	uint32_t data_size;
	uint32_t unknown;
	uint32_t pallet_frame_buffer_x;
	uint32_t pallet_frame_buffer_y;
	uint32_t number_pallet_colors;
	uint32_t number_of_pallets;
	uint32_t image_frame_buffer_x;
	uint32_t image_frame_buffer_y;
	uint32_t width;
	uint32_t height;
};

uint16_t *clut;
uint8_t *body;

struct DashTextureHeader tim;
char *active_file;

static void open_dialog(GApplication *app, gpointer user_data);
static void read_bin_file(char *filename);
static void listbox_callback(GtkListBox *box, GtkListBoxRow *row, gpointer user_data);
void read_image(FILE *fp);

GtkWidget *listbox;

int main(int argc, char *argv[]) {

	GtkWidget *window;
	GtkWidget *header;
	GtkWidget *file_menu, *menu, *item;
	GtkWidget *v_outline, *h_outline;
	GtkWidget *label, *row;
	GtkWidget *separator;
	GtkWidget *image;
	GtkWidget *first, *radio;
	GtkWidget *frame;
	GSList *list;
	GtkWidget *scrolledwindow;
	int i;

	gtk_init(&argc, &argv);

	// Set up Window

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_set_default_size(GTK_WINDOW(window), 680, 300);
	gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	// Set Up header

	header = gtk_header_bar_new();
	gtk_header_bar_set_title(GTK_HEADER_BAR(header), "Dash Texture Viewer");
	gtk_header_bar_set_show_close_button(GTK_HEADER_BAR(header), TRUE);
	gtk_window_set_titlebar(GTK_WINDOW(window), header);

	// Set Up Menu

	menu = gtk_menu_button_new();
	file_menu = gtk_menu_new();
	
	item = gtk_menu_item_new_with_label ("Open");
	g_signal_connect(G_OBJECT(item), "activate", G_CALLBACK(open_dialog), (gpointer)window);
	gtk_menu_attach (GTK_MENU (file_menu), item, 0, 1, 0, 1);
	item = gtk_menu_item_new_with_label ("Export");
	gtk_menu_attach (GTK_MENU (file_menu), item, 0, 1, 1, 2);
	item = gtk_menu_item_new_with_label ("Export All");
	gtk_menu_attach (GTK_MENU (file_menu), item, 0, 1, 2, 3);
	item = gtk_menu_item_new_with_label ("About");
	gtk_menu_attach (GTK_MENU (file_menu), item, 0, 1, 3, 4);
	
	gtk_widget_show_all (file_menu);

	gtk_menu_button_set_popup(GTK_MENU_BUTTON(menu), file_menu);
	gtk_header_bar_pack_start(GTK_HEADER_BAR(header), menu);
	gtk_widget_set_margin_start(GTK_WIDGET(menu), 15);

	// Vertical Outline

	v_outline = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	h_outline = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

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
	frame = gtk_frame_new(NULL);
	scrolledwindow = gtk_scrolled_window_new(NULL, NULL);
	gtk_container_add(GTK_CONTAINER(frame), scrolledwindow);
	gtk_container_add(GTK_CONTAINER(scrolledwindow), listbox);
	g_signal_connect(listbox, "row-activated", G_CALLBACK(listbox_callback), NULL);

	gtk_widget_set_size_request(GTK_WIDGET(frame), 230, 256);
	gtk_box_pack_start(GTK_BOX(h_outline), frame, TRUE, TRUE, 0);

	image = gtk_image_new_from_file("img/placeholder.png");
	frame = gtk_aspect_frame_new(NULL, 0, 0, 1.0f, FALSE);
	gtk_widget_set_size_request(GTK_WIDGET(frame), 256, 256);
	gtk_widget_set_margin_start(GTK_WIDGET(frame), 28);
	gtk_container_add(GTK_CONTAINER(frame), image);

	gtk_box_pack_start(GTK_BOX(h_outline), frame, TRUE, TRUE, 0);

	v_outline = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_box_pack_start(GTK_BOX(h_outline), v_outline, TRUE, FALSE, 0);

	first = gtk_radio_button_new_with_label(NULL, "Pallet 001");
	gtk_box_pack_start(GTK_BOX(v_outline), first, FALSE, FALSE, 0);

	radio = gtk_radio_button_new_with_label(NULL, "Pallet 002");
	gtk_radio_button_join_group(GTK_RADIO_BUTTON(radio), GTK_RADIO_BUTTON(first));
	gtk_box_pack_start(GTK_BOX(v_outline), radio, FALSE, FALSE, 0);
	
	radio = gtk_radio_button_new_with_label(NULL, "Pallet 003");
	gtk_radio_button_join_group(GTK_RADIO_BUTTON(radio), GTK_RADIO_BUTTON(first));
	gtk_box_pack_start(GTK_BOX(v_outline), radio, FALSE, FALSE, 0);
	
	radio = gtk_radio_button_new_with_label(NULL, "Pallet 004");
	gtk_radio_button_join_group(GTK_RADIO_BUTTON(radio), GTK_RADIO_BUTTON(first));
	gtk_box_pack_start(GTK_BOX(v_outline), radio, FALSE, FALSE, 0);

	radio = gtk_radio_button_new_with_label(NULL, "Pallet 005");
	gtk_radio_button_join_group(GTK_RADIO_BUTTON(radio), GTK_RADIO_BUTTON(first));
	gtk_box_pack_start(GTK_BOX(v_outline), radio, FALSE, FALSE, 0);

	gtk_widget_show_all(window);
	gtk_main();

	return 0;

}

static void open_dialog(GApplication *app, gpointer user_data) {

	GtkWidget *dialog;
	gint res;

	free(active_file);

	dialog = gtk_file_chooser_dialog_new(
		"Open .BIN File",
		GTK_WINDOW(user_data),
		GTK_FILE_CHOOSER_ACTION_OPEN,
		"Cancel",
		GTK_RESPONSE_CANCEL,
		"Open",
		GTK_RESPONSE_ACCEPT,
		NULL
	);

	res = gtk_dialog_run (GTK_DIALOG (dialog));
	if (res == GTK_RESPONSE_ACCEPT) {
		char *filename;
		GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
		filename = gtk_file_chooser_get_filename (chooser);

		active_file = malloc(strlen(filename) + 10);
		strcpy(active_file, filename);

		read_bin_file(filename);
		g_free(filename);
	}
	gtk_widget_destroy (dialog);

}

static void read_bin_file(char *filename) {

	GtkWidget *row, *label;
	char *dot;

	FILE *fp;
	uint32_t type;
	uint32_t ofs, file_len;
	char asset_name[0x20];

	fp = fopen(filename, "r");
	if(fp == NULL) {
		g_print("Unable to open %s for reading\n", filename);
		return;
	}
	
	fseek(fp, 0, SEEK_END);
	file_len = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	while(ftell(fp) < file_len) {

		ofs = ftell(fp);
		fread(&type, sizeof(uint32_t), 1, fp);
		//g_print("Offset: 0x%x\n", ofs);

		fseek(fp, ofs + 0x40, SEEK_SET);
		fread(asset_name, 0x20, 1, fp);

		if(asset_name[0] != '.' || asset_name[1] != '.') {
			fseek(fp, ofs + 0x400, SEEK_SET);
			continue;
		}
		
		dot = strrchr(asset_name, '.');
		g_print("Extension: %s\n", dot);
		
		if(!strcmp(dot, ".TIM") && type == 1) {
			label = gtk_label_new(asset_name);
			gtk_widget_set_halign (GTK_WIDGET(label), GTK_ALIGN_START);
			row = gtk_list_box_row_new();
			gtk_container_add(GTK_CONTAINER(row), label);
			gtk_container_add(GTK_CONTAINER(listbox), row);
		}

		fseek(fp, ofs + 0x400, SEEK_SET);

	}
	
	gtk_widget_show_all (listbox);
	fclose(fp);

}

static void listbox_callback(GtkListBox *box, GtkListBoxRow *row, gpointer user_data) {

	struct DashTextureHeader header;

	g_print("Active file: %s\n", active_file);
	GList *children = gtk_container_get_children(GTK_CONTAINER(row));
	const gchar *text = gtk_label_get_text(children->data);

	FILE *fp;
	uint32_t data_type, flen, data_len;
	uint32_t ofs;
	uint32_t pallet_colors, nb_pallets, bpp;
	uint32_t width, height;
	uint16_t *clut, *image_body;
	uint32_t blockw, blockh, x, y, bx, by;
	uint8_t byte, *bitmap;
	char *slash;

	char asset_name[0x20];

	fp = fopen(active_file, "r");
	if(fp == NULL) {
		g_print("Unable to open %s for reading\n", active_file);
		return;
	}
	
	fseek(fp, 0, SEEK_END);
	flen = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	while(ftell(fp) < flen) {

		ofs = ftell(fp);

		fseek(fp, ofs + 0x40, SEEK_SET);
		fread(asset_name, 0x20, 1, fp);

		if(asset_name[0] != '.' || asset_name[1] != '.') {
			fseek(fp, ofs + 0x400, SEEK_SET);
			continue;
		}
		
		if(strcmp(asset_name, text)) {
			fseek(fp, ofs + 0x400, SEEK_SET);
			continue;

		}
		
		fseek(fp, ofs, SEEK_SET);
		g_print("Parsing: %s at offset: 0x%x\n", asset_name, ofs);
		break;
	}
	

	read_image(fp);

}



void read_image(FILE *fp) {

	FILE *wp;
	char *slash, *point, filename[0x20], outname[0x40];
	uint32_t ofs, data_type, data_length, i;
	uint32_t pallet_x, pallet_y, pallet_colors, nb_pallets, bx, by, x, y;
	uint32_t frame_x, frame_y, width, height, bytes_per_pixel;
	uint16_t *clut, *image_body;
	uint8_t byte, *bitmap, *c;

	int BLOCK_WIDTH = 128;
	int BLOCK_HEIGHT = 32;

	// Save start offset
	ofs = ftell(fp);
	printf("TIM Start: 0x%08x\n", ofs);

	// Read Data type and Length

	fread(&data_type, sizeof(uint32_t), 1, fp);
	fread(&data_length, sizeof(uint32_t), 1, fp);

	// Read number of pallets and colors
	
	fseek(fp, ofs + 0xC, SEEK_SET);
	fread(&pallet_x, sizeof(uint32_t), 1, fp);
	fread(&pallet_y, sizeof(uint32_t), 1, fp);
	
	printf("Pallet: %d, %d\n", pallet_x, pallet_y);

	fseek(fp, ofs + 0x14, SEEK_SET);
	fread(&pallet_colors, sizeof(uint32_t), 1, fp);
	fread(&nb_pallets, sizeof(uint32_t), 1, fp);

	// Read Width and height of texture

	fread(&frame_x, sizeof(uint32_t), 1, fp);
	fread(&frame_y, sizeof(uint32_t), 1, fp);

	fseek(fp, ofs + 0x24, SEEK_SET);
	fread(&width, sizeof(uint32_t), 1, fp);
	fread(&height, sizeof(uint32_t), 1, fp);

	printf("Framebuffer: %d %d\n", frame_x, frame_y);

	// Set number of bits per pixel

	switch(pallet_colors) {
		case 0x100:

			bytes_per_pixel = 8;
			width *= 2;

		break;
		case 0x10:
			
			bytes_per_pixel = 4;
			width *= 4;

		break;
		default:
			
			fprintf(stderr, "Unknown pallet color parameter; %d\n", pallet_colors);
			exit(1);

		break;
	}

	// Read filename

	fseek(fp, ofs + 0x40, SEEK_SET);
	fread(filename, 0x20, 1, fp);

	// Read Color Look up Table

	fseek(fp, ofs + 0x100, SEEK_SET);
	clut = malloc(pallet_colors * nb_pallets * sizeof(uint16_t));
	fread(clut, sizeof(uint16_t), pallet_colors * nb_pallets, fp);

	// Read and convert image body

	fseek(fp, ofs + 0x800, SEEK_SET);
	printf("Seeking 0x800 bytes\n");

	image_body = malloc(sizeof(uint16_t) * width * height);

	switch(bytes_per_pixel) {
		case 4:
			
			for(y = 0; y < height; y+= BLOCK_HEIGHT) {
				for(x = 0; x < width; x += BLOCK_WIDTH) {
					for(by = 0; by < BLOCK_HEIGHT; by++) {
						for(bx = 0; bx < BLOCK_WIDTH; bx += 2) {

							fread(&byte, sizeof(uint8_t), 1, fp);
							image_body[((y + by) * width) + (x + bx)] = clut[byte & 0xf];
							image_body[((y + by) * width) + (x + bx + 1)] = clut[byte >> 4];

						}
					}
				}
			}

		break;
		case 8:
			
			BLOCK_WIDTH = BLOCK_WIDTH / 2;

			for(y = 0; y < height; y += BLOCK_HEIGHT) {
				for(x = 0; x < width; x += BLOCK_WIDTH) {
					for(by = 0; by < BLOCK_HEIGHT; by++) {
						for(bx = 0; bx < BLOCK_WIDTH; bx++) {

							fread(&byte, sizeof(uint8_t), 1, fp);
							image_body[((y + by) * width) + (x + bx)] = clut[byte];

						}
					}
				}
			}

		break;
	}
	
	free(clut);

	bitmap = malloc(width * height * 4);
	c = bitmap;

	for(i = 0; i < width * height; i++) {

		*c++ = ((image_body[i] >> 0x00) & 0x1f) << 3;
		*c++ = ((image_body[i] >> 0x05) & 0x1f) << 3;
		*c++ = ((image_body[i] >> 0x0a) & 0x1f) << 3;
		
		if(image_body[i] == 0) {
			*c++ = 0;
		} else {
			*c++ = 0xFF;
		}
	}

	free(image_body);

	// Create output filename

	slash = strrchr(filename, '\\') + 1;
	sprintf(outname, "%s", slash);
	
	point = strrchr(outname, '.');
	point[1] = 'P';
	point[2] = 'N';
	point[3] = 'G';
	
	printf("Name: %s\n", outname);
	
	// Write png file

	png_structp png_ptr;
	png_infop info_ptr;
	png_byte **row_ptr;
	int pixel_size = 4;
	int depth = 8;

	wp = fopen(outname, "w");
	if(wp == NULL) {
		fprintf(stderr, "Could not open %s for writing\n", outname);
		exit(1);
	}
	
	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if(png_ptr == NULL) {
		fprintf(stderr, "Could not create png struct\n");
		exit(1);
	}

	info_ptr = png_create_info_struct(png_ptr);
	if(info_ptr == NULL) {
		fprintf(stderr, "Could not create info struct\n");
		exit(1);
	}

	if(setjmp(png_jmpbuf(png_ptr))) {
		fprintf(stderr, "Could not set jmpbuf\n");
		exit(1);
	}

	png_set_IHDR(
		png_ptr,
		info_ptr,
		width,
		height,
		depth,
		PNG_COLOR_TYPE_RGBA,
		PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_DEFAULT,
		PNG_FILTER_TYPE_DEFAULT
	);

	row_ptr = png_malloc(png_ptr, height * sizeof(png_byte*));

	printf("00\n");
	
	i = 0;
	for(y = 0; y < height; y++) {
	
		png_byte *row = png_malloc(png_ptr, width * pixel_size);
		row_ptr[y] = row;

		for(x = 0; x < width; x++) {
			
			*row++ = bitmap[i++];
			*row++ = bitmap[i++];
			*row++ = bitmap[i++];
			*row++ = bitmap[i++];

		}

	}

	printf("01\n");

	png_init_io(png_ptr, wp);
	png_set_rows(png_ptr, info_ptr, row_ptr);
	png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);

	printf("02\n");

	for(y = 0; y < height; y++) {
		png_free(png_ptr, row_ptr[y]);
	}

	printf("03\n");

	free(bitmap);
	png_free(png_ptr, row_ptr);
	fclose(wp);

}
