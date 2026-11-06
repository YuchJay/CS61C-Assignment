/************************************************************************
**
** NAME:        gameoflife.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

// Determines what color the cell at the given row/col should be. This function allocates space for a new Color.
// Note that you will need to read the eight neighbors of the cell in question. The grid "wraps", so we treat the top row as adjacent to the bottom row
// and the left column as adjacent to the right column.
Color *evaluateOneCell(Image *image, int row, int col, uint32_t rule)
{
	Color pixel = image->image[row][col];
	int channels[3] = {pixel.R, pixel.G, pixel.B};
	// count[channel][bit]: live neighbors in that channel's bit plane
	int count[3][8] = {{0}};

	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			if (i == 0 && j == 0)
			{
				continue;
			}
			int neighborRow = (row + i + (int)image->rows) % (int)image->rows;
			int neighborCol = (col + j + (int)image->cols) % (int)image->cols;
			Color neighbor = image->image[neighborRow][neighborCol];
			int neighborChannels[3] = {neighbor.R, neighbor.G, neighbor.B};
			for (int ch = 0; ch < 3; ch++)
			{
				for (int bit = 0; bit < 8; bit++)
				{
					count[ch][bit] += (neighborChannels[ch] >> bit) & 1;
				}
			}
		}
	}

	Color *newPixel = malloc(sizeof(Color));
	int result[3];
	for (int ch = 0; ch < 3; ch++)
	{
		result[ch] = 0;
		for (int bit = 0; bit < 8; bit++)
		{
			int alive = (channels[ch] >> bit) & 1;
			// bits 0-8: birth rule; bits 9-17: survival rule
			int next = (rule >> (count[ch][bit] + (alive ? 9 : 0))) & 1;
			result[ch] |= next << bit;
		}
	}

	newPixel->R = result[0];
	newPixel->G = result[1];
	newPixel->B = result[2];
	return newPixel;
}

// The main body of Life; given an image and a rule, computes one iteration of the Game of Life.
// You should be able to copy most of this from steganography.c
Image *life(Image *image, uint32_t rule)
{
	Image *newImage = malloc(sizeof(Image));
	newImage->rows = image->rows;
	newImage->cols = image->cols;
	newImage->image = malloc(sizeof(Color *) * newImage->rows);

	for (int i = 0; i < newImage->rows; i++)
	{
		newImage->image[i] = malloc(sizeof(Color) * newImage->cols);
		for (int j = 0; j < newImage->cols; j++)
		{
			Color *pixel = evaluateOneCell(image, i, j, rule);
			newImage->image[i][j] = *pixel;
			free(pixel);
		}
	}

	return newImage;
}

/*
Loads a .ppm from a file, computes the next iteration of the game of life, then prints to stdout the new image.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a .ppm.
argv[2] should contain a hexadecimal number (such as 0x1808). Note that this will be a string.
You may find the function strtol useful for this conversion.
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!

You may find it useful to copy the code from steganography.c, to start.
*/
int main(int argc, char **argv)
{
	if (argc != 3)
	{
		fprintf(stderr, "usage: ./gameOfLife filename rule\nfilename is an ASCII PPM file (type P3) with maximum value 255.\nrule is a hex number beginning with 0x; Life is 0x1808.\n");
		exit(-1);
	}

	char *endptr;
	long parsed = strtol(argv[2], &endptr, 16);
	if (argv[2][0] == '\0' || *endptr != '\0' || parsed < 0 || parsed > UINT32_MAX)
	{
		fprintf(stderr, "Error: Invalid rule.\n");
		exit(-1);
	}

	Image *image = readData(argv[1]);
	if (image == NULL)
	{
		fprintf(stderr, "Error: Could not read image data.\n");
		exit(-1);
	}

	Image *newImage = life(image, (uint32_t)parsed);
	if (newImage == NULL)
	{
		freeImage(image);
		exit(-1);
	}

	writeData(newImage);

	freeImage(image);
	freeImage(newImage);

	return 0;
}
