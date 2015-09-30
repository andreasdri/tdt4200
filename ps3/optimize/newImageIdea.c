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
void performNewIdeaIteration(AccurateImage *imageOut, AccurateImage *imageIn, int colourType, int size) {
	
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
					if(currentX < 0)
						continue;
					if(currentX >= imageIn->x)
						continue;
					if(currentY < 0)
						continue;
					if(currentY >= imageIn->y)
						continue;
					
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
			int numberOfValuesInEachRow = imageOut->x; // R, G and B
			int offsetOfThePixel = (numberOfValuesInEachRow * senterY + senterX);
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
		printf("value is: %f\n", value);
		if(value > 255)
			imageOut->data[i].red = 255;
		else if (value < -1.0) {
			value = 257.0+value;
			if(value > 255)
				imageOut->data[i].red = 255;
			else
				imageOut->data[i].red = floor(value);
		} else if (value > -1.0 && value < 0.0) {
			imageOut->data[i].red = 0;
		} else {
			imageOut->data[i].red = floor(value);
		}
		
		value = (imageInLarge->data[i].green - imageInSmall->data[i].green);
		if(value > 255)
			imageOut->data[i].green = 255;
		else if (value < -1.0) {
			value = 257.0+value;
			if(value > 255)
				imageOut->data[i].green = 255;
			else
				imageOut->data[i].green = floor(value);
		} else if (value > -1.0 && value < 0.0) {
			imageOut->data[i].green = 0;
		} else {
			imageOut->data[i].green = floor(value);
		}
		
		value = (imageInLarge->data[i].blue - imageInSmall->data[i].blue);
		if(value > 255)
			imageOut->data[i].blue = 255;
		else if (value < -1.0) {
			value = 257.0+value;
			if(value > 255)
				imageOut->data[i].blue = 255;
			else
				imageOut->data[i].blue = floor(value);
		} else if (value > -1.0 && value < 0.0) {
			imageOut->data[i].blue = 0;
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
	AccurateImage *imageAccurate1_tiny = convertImageToNewFormat(image); // hva er egentlig forskjellen på Accurate1 og 2?
	AccurateImage *imageAccurate2_tiny = convertImageToNewFormat(image);
	
	int colour = 0; // don't make any difference what it is... just need it because it needs an parameter.
	// Process the tiny case:
	//for(int colour = 0; colour < 3; colour++) {
	
	//why does it do everything 5 times?...??? TODO: experement with this... removing some of them gives a lot of pixel errors
	// samples the points around 5 times for each picture?
		int size = 2; 
		performNewIdeaIteration(imageAccurate2_tiny, imageAccurate1_tiny, colour, size);
		performNewIdeaIteration(imageAccurate1_tiny, imageAccurate2_tiny, colour, size);
		performNewIdeaIteration(imageAccurate2_tiny, imageAccurate1_tiny, colour, size);
		performNewIdeaIteration(imageAccurate1_tiny, imageAccurate2_tiny, colour, size);
		performNewIdeaIteration(imageAccurate2_tiny, imageAccurate1_tiny, colour, size);
	//}
	
	
	AccurateImage *imageAccurate1_small = convertImageToNewFormat(image);
	AccurateImage *imageAccurate2_small = convertImageToNewFormat(image);
	
	// Process the small case:
	//for(int colour = 0; colour < 3; colour++) {
		size = 3;
		performNewIdeaIteration(imageAccurate2_small, imageAccurate1_small, colour, size);
		performNewIdeaIteration(imageAccurate1_small, imageAccurate2_small, colour, size);
		performNewIdeaIteration(imageAccurate2_small, imageAccurate1_small, colour, size);
		performNewIdeaIteration(imageAccurate1_small, imageAccurate2_small, colour, size);
		performNewIdeaIteration(imageAccurate2_small, imageAccurate1_small, colour, size);
	//}
	
	AccurateImage *imageAccurate1_medium = convertImageToNewFormat(image);
	AccurateImage *imageAccurate2_medium = convertImageToNewFormat(image);
	
	// Process the medium case:
	//for(int colour = 0; colour < 3; colour++) {
		size = 5;
		performNewIdeaIteration(imageAccurate2_medium, imageAccurate1_medium, colour, size);
		performNewIdeaIteration(imageAccurate1_medium, imageAccurate2_medium, colour, size);
		performNewIdeaIteration(imageAccurate2_medium, imageAccurate1_medium, colour, size);
		performNewIdeaIteration(imageAccurate1_medium, imageAccurate2_medium, colour, size);
		performNewIdeaIteration(imageAccurate2_medium, imageAccurate1_medium, colour, size);
	//}
	
	AccurateImage *imageAccurate1_large = convertImageToNewFormat(image);
	AccurateImage *imageAccurate2_large = convertImageToNewFormat(image);
	
	// Do each color channel
	//for(int colour = 0; colour < 3; colour++) {
		size = 8;
		performNewIdeaIteration(imageAccurate2_large, imageAccurate1_large, colour, size);
		performNewIdeaIteration(imageAccurate1_large, imageAccurate2_large, colour, size);
		performNewIdeaIteration(imageAccurate2_large, imageAccurate1_large, colour, size);
		performNewIdeaIteration(imageAccurate1_large, imageAccurate2_large, colour, size);
		performNewIdeaIteration(imageAccurate2_large, imageAccurate1_large, colour, size);
	//}
	
	// Save the images.
	PPMImage *final_tiny = performNewIdeaFinalization(imageAccurate2_tiny,  imageAccurate2_small);
	PPMImage *final_small = performNewIdeaFinalization(imageAccurate2_small,  imageAccurate2_medium);
	PPMImage *final_medium = performNewIdeaFinalization(imageAccurate2_medium,  imageAccurate2_large);
	
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

