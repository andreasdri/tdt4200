#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "ppm.h"

// Image from:
// http://7-themes.com/6971875-funny-flowers-pictures.html

typedef struct {
     double red,green,blue;
} AccuratePixel;

typedef struct {
     int x, y;
     AccuratePixel *data;
} AccurateImage;

// Convert ppm to high precision format.
AccurateImage *convertImageToNewFormat(PPMImage *image) {
	// Make a copy
	AccurateImage *imageAccurate;
	imageAccurate = (AccurateImage *)malloc(sizeof(AccurateImage));
	imageAccurate->data = (AccuratePixel*)malloc(image->x * image->y * sizeof(AccuratePixel));
	for(int i = 0; i < image->x * image->y; i++) {
		imageAccurate->data[i].red   = (double) image->data[i].red;
		imageAccurate->data[i].green = (double) image->data[i].green;
		imageAccurate->data[i].blue  = (double) image->data[i].blue;
	}
	imageAccurate->x = image->x;
	imageAccurate->y = image->y;
	
	return imageAccurate;
}

// Perform the new idea:
void performNewIdeaIteration(AccurateImage *imageOut, AccurateImage *imageIn, int size) {
	
	// Iterate over each pixel
	for(int senterX = 0; senterX < imageIn->x; senterX++) {
	
		for(int senterY = 0; senterY < imageIn->y; senterY++) {
			
			// For each pixel we compute the magic number
			double sumRed = 0, sumGreen = 0, sumBlue = 0;
			int countIncluded = 0;
			for(int x = -size; x <= size; x++) {
			
				for(int y = -size; y <= size; y++) {
					int currentX = senterX + x;
					int currentY = senterY + y;
					
					// Check if we are outside the bounds
					if(currentX < 0 || currentX >= imageIn->x)
						continue;
					//if(currentX >= imageIn->x)
					//	continue;
					
					if(currentY < 0 || currentY >= imageIn->y)
						continue;
					//if(currentY >= imageIn->y)
					//	continue;
					
					// Now we can begin
					int numberOfValuesInEachRow = imageIn->x; 
					int offsetOfThePixel = (numberOfValuesInEachRow * currentY + currentX);
					sumRed += imageIn->data[offsetOfThePixel].red;
					sumGreen += imageIn->data[offsetOfThePixel].green;
					sumBlue += imageIn->data[offsetOfThePixel].blue;
					
					// Keep track of how many values we have included  (ON THAT ROW?)
					countIncluded++;
				}
			
			}
			
			// Now we compute the final value
			// setter verdien direkte, ikke noe mellomlargring.

			// double valueRed = sumRed / countIncluded;
			// double valueGreen = sumGreen / countIncluded;
			// double valueBlue = sumBlue / countIncluded;
			// printf("count included %i\n", countIncluded);
			
			
			// Update the output image
			int offsetOfThePixel = (imageOut->x * senterY + senterX);
			imageOut->data[offsetOfThePixel].red = sumRed / countIncluded;
			imageOut->data[offsetOfThePixel].green = sumGreen / countIncluded;
			imageOut->data[offsetOfThePixel].blue = sumBlue / countIncluded;
		}
	
	}
	
}


// Perform the final step, and return it as ppm.
PPMImage * performNewIdeaFinalization(AccurateImage *imageInSmall, AccurateImage *imageInLarge) {
	PPMImage *imageOut;
	imageOut = (PPMImage *)malloc(sizeof(PPMImage));
	imageOut->data = (PPMPixel*)malloc(imageInSmall->x * imageInSmall->y * sizeof(PPMPixel));
	
	imageOut->x = imageInSmall->x;
	imageOut->y = imageInSmall->y;
	
	for(int i = 0; i < imageInSmall->x * imageInSmall->y; i++) {

		// kan sikkert ta noe med abs value her?  -> bare abs gir error pixler
		double value = (imageInLarge->data[i].red - imageInSmall->data[i].red);
		//printf("value is: %f\n", value);
		
		// aner ikke om sjekkingen på stedene her har julpet noe..
		if (value > -1.0 && value < 1.0) {
			imageOut->data[i].red = 0;
		} else if(value > 255)
			imageOut->data[i].red = 255;
		else if (value < -1.0) {
			value = 257.0+value;
			if(value > 255)
				imageOut->data[i].red = 255;
			else
				imageOut->data[i].red = floor(value);
		} else {
			imageOut->data[i].red = floor(value);
		}
		
		value = (imageInLarge->data[i].green - imageInSmall->data[i].green);
		if (value > -1.0 && value < 1.0) {
			imageOut->data[i].green = 0;
		} else if(value > 255)
			imageOut->data[i].green = 255;
		else if (value < -1.0) {
			value = 257.0+value;
			if(value > 255)
				imageOut->data[i].green = 255;
			else
				imageOut->data[i].green = floor(value);
		} else {
			imageOut->data[i].green = floor(value);
		}
		
		value = (imageInLarge->data[i].blue - imageInSmall->data[i].blue);
		if (value > -1.0 && value < 1.0) {
			imageOut->data[i].blue = 0;
		} else if(value > 255)
			imageOut->data[i].blue = 255;
		else if (value < -1.0) {
			value = 257.0+value;
			if(value > 255)
				imageOut->data[i].blue = 255;
			else
				imageOut->data[i].blue = floor(value);
		} else {
			imageOut->data[i].blue = floor(value);
		}
	}
	
	return imageOut;
}


int main(int argc, char** argv) {
	clock_t start = clock(), diff;
	PPMImage *image;
	
	if(argc > 1) {
		image = readPPM("flower.ppm");
	} else {
		image = readStreamPPM(stdin);
	}
	
	AccurateImage *imageOriginal = convertImageToNewFormat(image);
	AccurateImage *image_x = convertImageToNewFormat(image);
	AccurateImage *image_y = convertImageToNewFormat(image);
	AccurateImage *imageTemp = convertImageToNewFormat(image);
	
	// Process the tiny case: (y)
		performNewIdeaIteration(image_x, imageOriginal, 2);
		performNewIdeaIteration(imageTemp, image_x, 2);
		performNewIdeaIteration(image_x, imageTemp, 2);
		performNewIdeaIteration(imageTemp, image_x, 2);
		performNewIdeaIteration(image_x, imageTemp, 2);
	
	
	// Process the small case: 
		performNewIdeaIteration(image_y, imageOriginal, 3);
		performNewIdeaIteration(imageTemp, image_y, 3);
		performNewIdeaIteration(image_y, imageTemp, 3);
		performNewIdeaIteration(imageTemp, image_y, 3);
		performNewIdeaIteration(image_y, imageTemp, 3);

	PPMImage *final_tiny = performNewIdeaFinalization(image_x,  image_y); //here x is larger than y
	
	
	// Process the medium case:
		performNewIdeaIteration(image_x, imageOriginal, 5);
		performNewIdeaIteration(imageTemp, image_x, 5);
		performNewIdeaIteration(image_x, imageTemp, 5);
		performNewIdeaIteration(imageTemp, image_x, 5);
		performNewIdeaIteration(image_x, imageTemp, 5);
	
	PPMImage *final_small = performNewIdeaFinalization(image_y,  image_x); //here y is larger than x


	// Process the large case
		performNewIdeaIteration(image_y, imageOriginal, 8);
		performNewIdeaIteration(imageTemp, image_y, 8);
		performNewIdeaIteration(image_y, imageTemp, 8);
		performNewIdeaIteration(imageTemp, image_y, 8);
		performNewIdeaIteration(image_y, imageTemp, 8);
	
	PPMImage *final_medium = performNewIdeaFinalization(image_x,  image_y);
	


	if(argc > 1) {
		writePPM("flower_tiny.ppm", final_tiny);
		writePPM("flower_small.ppm", final_small);
		writePPM("flower_medium.ppm", final_medium);
	} else {
		writeStreamPPM(stdout, final_tiny);
		writeStreamPPM(stdout, final_small);
		writeStreamPPM(stdout, final_medium);
	}
	
	// messure the time taken
	diff = clock() - start;
	int msec = diff * 1000 / CLOCKS_PER_SEC;
	printf("Time taken %d seconds %d milliseconds \n", msec/1000, msec%1000);
	// --- done measuring time
	
	return 0;
}
