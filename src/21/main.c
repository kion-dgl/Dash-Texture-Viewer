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

void convert_rgba5551(uint16_t *src, uint8_t *dst, uint16_t width, uint16_t height);
void export_tim(uint8_t *bitmap, int width, int height);

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
	uint32_t ofs, file_len;
	char asset_name[0x20];

	fp = fopen(active_file, "r");
	if(fp == NULL) {
		g_print("Unable to open %s for reading\n", active_file);
		return;
	}
	
	fseek(fp, 0, SEEK_END);
	file_len = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	while(ftell(fp) < file_len) {

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
	
	// Parse File
	fread(&header, 1, sizeof(header), fp);

	// Debug Header

	g_print("\nData type: %d, Data length: %d\n", 
		header.data_type, 
		header.data_size
	);

	g_print("Pallet framebuffer coords: x(%d) y(%d)\n", 
		header.pallet_frame_buffer_x, 
		header.pallet_frame_buffer_y
	);

	g_print("Pallet colors: %d, Number of Pallets: %d\n",
		header.number_pallet_colors,
		header.number_of_pallets
	);

	g_print("Frame buffer coords: x(%d) y(%d)\n",
		header.image_frame_buffer_x,
		header.image_frame_buffer_y
	);

	g_print("Image width(%d) and height(%d)\n",
		header.width,
		header.height
	);

	if(clut) {
		free(clut);
	}

	clut = malloc(
		header.number_pallet_colors * 
		header.number_of_pallets *
		sizeof(uint16_t)
	);
	fseek(fp, ofs + 0x100, SEEK_SET);
	fread(
		clut, 
		header.number_pallet_colors * 
		header.number_of_pallets *
		sizeof(uint16_t),
		1,
		fp
	);

	if(body) {
		free(body);
	}

	body = malloc(header.width * header.height);
	fseek(fp, ofs + 0x800, SEEK_SET);
	fread(body, header.width * header.height, 1, fp);
	
	int i, byte, x, y, bx, by, blockw, blockh;
	
	uint16_t *image_body;
	uint8_t *bitmap;

	blockw = 128;
	blockh = 32;

	switch(header.number_pallet_colors) {
		case 0x10:
			
			header.width *= 4;
			image_body = malloc(sizeof(uint16_t) * header.width * header.height);
			bitmap = malloc(header.width * header.height * 4);

			i = 0;
			for(y = 0; y < header.height; y+= blockh) {
				for(x = 0; x < header.width; x += blockw) {
					for(by = 0; by < blockh; by++) {
						for(bx = 0; bx < blockw; bx += 2) {
							byte = body[i];
							image_body[((y + by) * header.width) + (x + bx)] = clut[byte & 0xf];
							image_body[((y + by) * header.width) + (x + bx + 1)] = clut[byte >> 4];
							i++;
						}
					}
				}
			}

			convert_rgba5551(image_body, bitmap, header.width, header.height);
			free(image_body);

		break;
		case 0x100:
			
			blockw /= 2;
			header.width *= 2;
			image_body = malloc(sizeof(uint16_t) * header.width * header.height);
			bitmap = malloc(header.width * header.height * 4);

			i = 0;
			for(y = 0; y < header.height; y+= blockh) {
				for(x = 0; x < header.width; x += blockw) {
					for(by = 0; by < blockh; by++) {
						for(bx = 0; bx < blockw; bx ++) {
							byte = body[i];
							image_body[((y + by) * header.width) + (x + bx)] = clut[byte];
							i++;
						}
					}
				}
			}

			convert_rgba5551(image_body, bitmap, header.width, header.height);
			free(image_body);
		
		break;

	}
	
	export_tim(bitmap, header.width, header.height);
	free(bitmap);
	fclose(fp);
	
}


void convert_rgba5551(uint16_t *src, uint8_t *dst, uint16_t width, uint16_t height) {

    int i;
    uint8_t *c = dst;

    for(i = 0; i < width * height; i++) {
        *c++ = ((src[i] >> 0x00) & 0x1f) << 3;
        *c++ = ((src[i] >> 0x05) & 0x1f) << 3;
        *c++ = ((src[i] >> 0x0a) & 0x1f) << 3;
        if(src[i] == 0) {
            *c++ = 0;
        } else if(src[i] >> 0x0f) {
            *c++ = 0xff;
        } else {
            *c++ = 0x80;
        }
    }

}

void export_tim(uint8_t *bitmap, int width, int height) {

    FILE *fp;
    png_structp png_ptr;
    png_infop info_ptr;
    size_t x, y;
    png_byte **row_ptr;
    uint16_t color;

    int i;
    int pixel_size = 4;
    int depth = 8;

    fp = fopen("out.png", "wb");
    if(fp == NULL) {
        fprintf(stderr, "Could not open out.png for writing\n");
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

    printf("Before row\n");

    row_ptr = png_malloc(png_ptr, height * sizeof(png_byte*));

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

    printf("After row \n");

    png_init_io(png_ptr, fp);
    png_set_rows(png_ptr, info_ptr, row_ptr);
    png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);

    for(y = 0; y < height; y++) {
        png_free(png_ptr, row_ptr[y]);
    }

    png_free(png_ptr, row_ptr);

}

