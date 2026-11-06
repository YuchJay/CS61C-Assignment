/************************************************************************
**
** NAME:        imageloader.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**              Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-15
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "imageloader.h"

// Opens a .ppm P3 image file, and constructs an Image object.
// You may find the function fscanf useful.
// Make sure that you close the file with fclose before returning.
Image *readData(char *filename)
{
	// Initialize variables.
	FILE *fp = fopen(filename, "r");
	if (fp == NULL)
	{
		return NULL;
	}
	int width, height, maxColor;
	char buffer[20];
	int num1, num2, num3;

	// Read the header and basic information.
	Image *ppm = malloc(sizeof(Image));
	if (ppm == NULL)
	{
		fclose(fp);
		return NULL;
	}
	if (fscanf(fp, "%19s", buffer) != 1 || strcmp(buffer, "P3") != 0 ||
		fscanf(fp, "%d %d", &width, &height) != 2 ||
		fscanf(fp, "%d", &maxColor) != 1 ||
		width <= 0 || height <= 0)
	{
		free(ppm);
		fclose(fp);
		return NULL;
	}

	// Assign the values.
	ppm->rows = height;
	ppm->cols = width;
	ppm->image = malloc(sizeof(Color *) * height);
	if (ppm->image == NULL)
	{
		free(ppm);
		fclose(fp);
		return NULL;
	}

	// Read the pixel data.
	for (int i = 0; i < height; i++)
	{
		Color *row = malloc(sizeof(Color) * width);
		if (row == NULL)
		{
			for (int k = 0; k < i; k++)
			{
				free(ppm->image[k]);
			}
			free(ppm->image);
			free(ppm);
			fclose(fp);
			return NULL;
		}
		ppm->image[i] = row;
		for (int j = 0; j < width; j++)
		{
			fscanf(fp, " %d %d %d", &num1, &num2, &num3);
			row[j].R = num1;
			row[j].G = num2;
			row[j].B = num3;
		}
	}

	fclose(fp);
	return ppm;
}

// Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the image's data.
void writeData(Image *image)
{
	// Print the header and basic information.
	printf("P3\n");
	printf("%d %d\n", image->cols, image->rows);
	printf("255\n");

	// Print the pixel data.
	for (int i = 0; i < image->rows; i++)
	{
		for (int j = 0; j < image->cols; j++)
		{
			printf("%3d %3d %3d", image->image[i][j].R, image->image[i][j].G, image->image[i][j].B);
			if (j < image->cols - 1)
			{
				printf("   ");
			}
		}
		printf("\n");
	}

	return;
}

// Frees an image
void freeImage(Image *image)
{
	for (int i = 0; i < image->rows; i++)
	{
		free(image->image[i]);
	}
	free(image->image);
	free(image);

	return;
}