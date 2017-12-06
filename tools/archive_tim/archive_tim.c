#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <png.h>

int read_archive(char *filename);
void read_image(FILE *fp);

int main(int argc, char *argv[]) {

	if(argc != 2) {
		fprintf(stderr, "Error. Usage: %s <filename.bin>\n", argv[0]);
	}
	
	return read_archive(argv[1]);

}

int read_archive(char *filename) {
	
	FILE *fp;
	uint32_t file_len, ofs;
	int32_t data_type;
	char data_name[0x20];

	// Open file

	fp = fopen(filename, "r");
	if(fp == NULL) {
		fprintf(stderr, "Could not open <%s> for reading\n", filename);
		return 1;
	}

	// Get file length

	fseek(fp, 0, SEEK_END);
	file_len = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	// Iterate over file

	while(ftell(fp) < file_len) {
		
		// Get offset and data_type

		ofs = ftell(fp);
		fread(&data_type, sizeof(int32_t), 1, fp);

		// End on terminate signal

		if(data_type == -1) {
			printf("End.\n");
			break;
		}
		
		// Seek to offset + 0x40 for filename

		fseek(fp, ofs + 0x40, SEEK_SET);
		fread(data_name, sizeof(char), 0x20, fp);
		
		// Detect if file location contains asset

		if(data_name[0] == '.' && data_name[1] == '.') {
			
			fseek(fp, ofs, SEEK_SET);

			switch(data_type) {
				case 1: // TIM IMAGE
					
					printf("Reading: %s\n", data_name);

					read_image(fp);
				break;
			}

		}

		// Seek to next location in file

		fseek(fp, ofs + 0x400, SEEK_SET);

	}

	// Close file pointer

	fclose(fp);

	// Return 

	return 0;
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
	sprintf(outname, "OUTPUT/%s", slash);
	
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
