
/*
 * Copyright 2002-2010 Guillaume Cottenceau.
 *
 * This software may be freely redistributed under the terms
 * of the X11 license.
 *
 */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#define PNG_DEBUG 3
#include <png.h>


void set_pngrow_pointers(int x, int y, png_bytep* rows, unsigned long** bits)
{
 	register volatile int i, j, i2;
 	png_byte* row;

	i2 = y;
 	for (i=0;i<y;i++)
 	{
		i2 --;
     		row = rows[i];
     		for (j=0;j<x;j++)
     		{
		        row[j*3+2] = (bits[i2][j] & 0xff);
		        row[j*3+1] = (bits[i2][j] & 0xff00) >> 0x8;
		        row[j*3]   = (bits[i2][j] & 0xff0000) >> 0x10;
			/*
			printf("%d(%d-%d): %02x %02x %02x <-- bits[%d][%d]: %08x\n",
				i, 3*j, 3*j+2, 
				(int)(row[j*3] & 0xff), 
				(int)(row[j*3+1] & 0xff), 
				(int)(row[j*3+2] & 0xff), 
				i, j, bits[i][j]);
			*/
     		}
 	}
}


void set_pngrows_array(int x, int y, png_bytep* rows, unsigned long** bits, int dim)
{
 register volatile int i, j, r, i2;
 png_byte* row;

 i2 = y;
 if (dim == 4) /* RGBA */
 {
 	for (i=0;i<y;i++)
 	{
	    	i2 --;
     		row = rows[i];
     		for (j=0;j<x;j++)
     		{
	 		r 		=  (row[j<<2]     & 0xff) << 0x10;
	 		r 		+= (row[(j<<2)+1] & 0xff) << 0x8;
	 		r 		+=  row[(j<<2)+2] & 0xff;
	 		bits[i2][j] 	= r;
     		}
 	}
 }
 else if (dim == 3) /* RGB */
 {
 	for (i=0;i<y;i++)
 	{
	    	i2 --;
     		row = rows[i];
     		for (j=0;j<x;j++)
     		{
	 		r 		=  (row[3*j]   & 0xff) << 0x10;
	 		r 		+= (row[3*j+1] & 0xff) << 0x8;
	 		r 		+=  row[3*j+2] & 0xff;
	 		bits[i2][j] 	= r;
/*			printf("RGB: bits[%d][%d] <-- %06x (from: %d(%d-%d))\n", i, j, r, i, 3*j, 3*j+2);*/
     		}
 	}
 }
 else if (dim == 1) /* GREY */
 {
 	for (i=0;i<y;i++)
 	{
	    	i2 --;
     		row = rows[i];
     		for (j=0;j<x;j++)
     		{
	 		r 		= row[j] & 0xff;
	 		bits[i2][j] 	= (r << 0x10) + (r << 0x8) + r;
/*			printf("RGB: bits[%d][%d] <-- %06x (from: %02x %d(%d))\n", i, j, bits[i][j], r, i, j);*/
     		}
 	}
 }


 /*
        for (y=0; y<height; y++) {
                png_byte* row = row_pointers[y];
                for (x=0; x<width; x++) {
                        png_byte* ptr = &(row[x*4]);
                        printf("Pixel at position [ %d - %d ] has RGBA values: %d - %d - %d - %d\n",
                               x, y, ptr[0], ptr[1], ptr[2], ptr[3]);

                        // set red value to 0 and green value to the blue one
                        ptr[0] = 0;
                        ptr[1] = ptr[2];
                }
        }
 */
}

int read_png_file(unsigned long*** bits, int* x, int* y, char* file_name)
{
	int i, width, height, dim;
	png_byte ct, bit_depth;

	png_structp png_ptr;
	png_infop info_ptr;
	int number_of_passes;
	png_bytep* row_pointers;

        png_byte header[8]; 

        /* open file and test for it being a png */
        FILE *fp = fopen(file_name, "rb");
        if (!fp)
	{
                printf("pngio: File %s could not be opened for reading", file_name);
		return 1;
	}

        fread(header, 1, 8, fp);
        if (png_sig_cmp(header, 0, 8))
	{
                printf("pngio: File %s is not recognized as a PNG file", file_name);
		return 2;
	}


        /* initialize stuff */
        png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

        if (!png_ptr)
	{
                printf("pngio: png_create_read_struct failed");
		return 3;
	}

        info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr)
	{
                printf("pngio: png_create_info_struct failed");
		return 4;
	}

        if (setjmp(png_jmpbuf(png_ptr)))
	{
                printf("pngio: Error during init_io");
		return 5;
	}

        png_init_io(png_ptr, fp);
        png_set_sig_bytes(png_ptr, 8);

        png_read_info(png_ptr, info_ptr);

        width  = png_get_image_width(png_ptr, info_ptr);
        height = png_get_image_height(png_ptr, info_ptr);
        ct     = png_get_color_type(png_ptr, info_ptr);
        bit_depth = png_get_bit_depth(png_ptr, info_ptr);

        number_of_passes = png_set_interlace_handling(png_ptr);
        png_read_update_info(png_ptr, info_ptr);
/*	printf("bit_depth: %d, color_type: %d, num_of_passes: %d\n", bit_depth, ct, number_of_passes);*/

        /* read file */
        if (setjmp(png_jmpbuf(png_ptr)))
	{
                printf("pngio: Error during read_image");
		return 6;
	}

        if (ct == PNG_COLOR_TYPE_RGB)
	{
	    dim = 3;
	}
	else if (ct == PNG_COLOR_TYPE_RGBA)
	{
	    dim = 4;
	}
	else if (ct == PNG_COLOR_TYPE_GRAY)
	{
	    dim = 1;
	}
	else
	{
	    printf("pngio: unsupported color type: %d\n", ct);
	    return 7;
	}


        row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
        for (i=0;i<height;i++)
	{
/*                row_pointers[i] = (png_byte*)malloc(png_get_rowbytes(png_ptr,info_ptr));*/
                row_pointers[i] = (png_byte*)malloc(sizeof(png_byte) * width * dim);
	}

        png_read_image(png_ptr, row_pointers);
        fclose(fp);

	*x = width;
	*y = height;
    
	*bits = (unsigned long**)calloc(*y, sizeof(unsigned long*));
	for (i=0;i<height;i++) (*bits)[i] = (unsigned long*)calloc(*x, sizeof(unsigned long));

	set_pngrows_array(*x, *y, row_pointers, *bits, dim);

        for (i=0;i<height;i++) free(row_pointers[i]);
        free(row_pointers);

	return 0;
}


/*int save_jpeg_file(unsigned long** tab, int x, int y, char* filename)*/
int write_png_file(unsigned long** bits, int x, int y, char* file_name)
{
	png_structp png_ptr;
	png_infop info_ptr;
	FILE* fp;
	int i, width, height, dim;
	png_byte color_type, bit_depth;
	png_bytep* row_pointers;

        /* create file */
        fp = fopen(file_name, "wb");
        if (!fp)
	{
                printf("pngio: File %s could not be opened for writing", file_name);
		return 1;
	}

        /* initialize stuff */
        png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

        if (!png_ptr)
	{
                printf("pngio: png_create_write_struct failed");
		return 2;
	}

        info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr)
	{
                printf("pngio: png_create_info_struct failed");
		return 3;
	}

        if (setjmp(png_jmpbuf(png_ptr)))
	{
                printf("pngio: Error during init_io");
		return 4;
	}

        png_init_io(png_ptr, fp);

        /* write header */
        if (setjmp(png_jmpbuf(png_ptr)))
	{
                printf("pngio: Error during writing header");
		return 5;
	}

	width  = x;
	height = y;
	bit_depth = 8;
	color_type = PNG_COLOR_TYPE_RGB;
	dim = 3;

        png_set_IHDR(png_ptr, info_ptr, width, height,
                     bit_depth, color_type, PNG_INTERLACE_NONE,
                     PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

        png_write_info(png_ptr, info_ptr);

        /* write bytes */
        if (setjmp(png_jmpbuf(png_ptr)))
	{
                printf("pngio: Error during writing bytes");
		return 6;
	}

        row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
        for (i=0;i<height;i++)
	{
                row_pointers[i] = (png_byte*)malloc(sizeof(png_byte) * (width + 4) * dim);
	}

	set_pngrow_pointers(x, y, row_pointers, bits);

        png_write_image(png_ptr, row_pointers);

        /* end write */
        if (setjmp(png_jmpbuf(png_ptr)))
	{
                printf("pngio: Error during end of write");
		return 7;
	}

        png_write_end(png_ptr, NULL);

        /* cleanup heap allocation */
        for (i=0;i<height;i++) free(row_pointers[i]);
        free(row_pointers);

        fclose(fp);

	return 0;
}

/*
void process_file(void)
{
        if (png_get_color_type(png_ptr, info_ptr) == PNG_COLOR_TYPE_RGB)
                printf("[process_file] input file is PNG_COLOR_TYPE_RGB but must be PNG_COLOR_TYPE_RGBA "
                       "(lacks the alpha channel)");

        if (png_get_color_type(png_ptr, info_ptr) != PNG_COLOR_TYPE_RGBA)
                printf("[process_file] color_type of input file must be PNG_COLOR_TYPE_RGBA (%d) (is %d)",
                       PNG_COLOR_TYPE_RGBA, png_get_color_type(png_ptr, info_ptr));

        for (y=0; y<height; y++) {
                png_byte* row = row_pointers[y];
                for (x=0; x<width; x++) {
                        png_byte* ptr = &(row[x*4]);
                        printf("Pixel at position [ %d - %d ] has RGBA values: %d - %d - %d - %d\n",
                               x, y, ptr[0], ptr[1], ptr[2], ptr[3]);

                        ptr[0] = 0;
                        ptr[1] = ptr[2];
                }
        }
}
*/


int main(int lb, char** par)
{
	int x, y;
	unsigned long** bits;

	if (lb < 3)
	{
	    printf("uasge: %s infile outfile\n", par[0]);
	    return 1;
	}

        if (read_png_file(&bits, &x, &y, par[1]) != 0)
	{
	    printf("read_png_file: error\n");
	    return 2;
	}

        if (write_png_file(bits, x, y, par[2]) != 0)
	{
	    printf("write_png_file: error\n");
	    return 3;
	}

        return 0;
}

