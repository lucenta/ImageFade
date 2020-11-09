/* 
Author: Andrew Lucentini

Description: This program performs filtering on a ppm image using openMP. To run, type "make" then:
	
			./ppmfilt r <inputFilename>.ppm <outputFilename>.ppm

			where r is the radius of the filter window.
*/
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h> 
#include "ppmFile.h"

int main(int argc, char *argv[]){

	if (argc != 4){
		printf("Invalid usage\nCorrect usage is: ./hw4 r inputFilename.ppm outputFilename.ppm\n");
		exit(1);
	}

	int imgWidth, imgHeight, r;
	char *inputImg  = argv[2], *outputImg = argv[3]; /* Input and output file names */
	Image *input, *output;

	input = ImageRead(inputImg);
	imgWidth = ImageWidth(input);
	imgHeight = ImageHeight(input);
	output = ImageCreate(imgWidth, imgHeight); /* Create output image */ 
	r = atoi(argv[1]); 						   /* Get blur radius. */ 

	int x, y, i, j, 						/* Positional varibles used to denote pixel positions */
		horizontalStart, horizontalEnd,		/* Horizonal filter window position for a given pixel */
		verticalStart, verticalEnd, 		/* Vertical filter window position for a given pixel */
		rsum, gsum, bsum,					/* Sum of the red, green, and blue pixel values for a given filter window */
		numOfPixels;						/* Number of pixels in a given filter window */

	/* Start timing */
	double start_time = omp_get_wtime(); 
	printf("Start filtering using openMP\n"); 

	/* Use openMP parallel loop that will automatically split up the work between threads */
	# pragma omp parallel for private(x, y, i, j, horizontalStart, horizontalEnd, verticalStart, verticalEnd, rsum, gsum, bsum, numOfPixels)
	for (x = 0; x < imgWidth; x++){
		for (y = 0; y < imgHeight; y++){
		horizontalStart = (x <= r) ? 0 : x-r; 							/* Determine left starting position of filter window */
		horizontalEnd = ((imgWidth-x) <=r ) ? imgWidth-1  : x+r;		/* Determine right ending position of filter window */
		verticalStart = (y <= r) ? 0 : y-r;								/* Determine top starting position of filter window */
		verticalEnd = ((imgHeight-y) <=r ) ? imgHeight-1  : y+r;		/* Determine bottom ending position of filter window */
		numOfPixels = (horizontalEnd-horizontalStart+1)*(verticalEnd-verticalStart+1); /* Determine number of pixels in window */
		rsum = 0;
		gsum = 0;
		bsum = 0;
		/* Iterate through determined pixel window */
		for (i = horizontalStart; i <= horizontalEnd; i++){
			for (j = verticalStart; j <= verticalEnd; j++){
				/* Compute the sum of the red, green, and blue pixels respectively */
				rsum+=ImageGetPixel(input, i, j, 0);
				gsum+=ImageGetPixel(input, i, j, 1);
				bsum+=ImageGetPixel(input, i, j, 2);
			}
		}
		/* Set the average pixel value for each pixel */
		ImageSetPixel(output, x, y, 0, rsum/numOfPixels);
		ImageSetPixel(output, x, y, 1, gsum/numOfPixels);
		ImageSetPixel(output, x, y, 2, bsum/numOfPixels);

		}
	}
	/* Write the image to the output file */
	ImageWrite(output, outputImg);

	/* End timing */
	double time = omp_get_wtime() - start_time; 
	printf("Finished in %f\n", time);

	return 0;
}