#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "ppm.h"


// Image from:
// http://7-themes.com/6971875-funny-flowers-pictures.html

typedef struct {
    float red,green,blue,trash; // TODO: this must probably be changed for a SIMD version.
} AccuratePixelSIMD;

typedef struct {
     int x, y;
     AccuratePixelSIMD *data; // TODO: this might require some changes for a SIMD version too.
} AccurateImage;

typedef float v4f __attribute__((vector_size (16)));

// Convert ppm to high precision format.
AccurateImage *convertImageToNewFormat(PPMImage *image) {
	// Make a copy
	AccurateImage *imageAccurate;
	imageAccurate = (AccurateImage *)malloc(sizeof(AccurateImage));
	imageAccurate->data = (AccuratePixelSIMD*)malloc(image->x * image->y * sizeof(AccuratePixelSIMD));
	for(int i = 0; i < image->x * image->y; i++) {
		imageAccurate->data[i].red   = (float) image->data[i].red;
		imageAccurate->data[i].green = (float) image->data[i].green;
		imageAccurate->data[i].blue  = (float) image->data[i].blue;
		imageAccurate->data[i].trash = (float) 0; // prove aa fjerne denne senere
	}
	//printf("number of pixels: %i\n", image->x * image->y);
	imageAccurate->x = image->x;
	imageAccurate->y = image->y;
	
	return imageAccurate;
}

AccurateImage *createEmptyImage(PPMImage *image){
	AccurateImage *imageAccurate;
	imageAccurate = (AccurateImage *)malloc(sizeof(AccurateImage));
	imageAccurate->data = (AccuratePixelSIMD*)malloc(image->x * image->y * sizeof(AccuratePixelSIMD));
	imageAccurate->x = image->x;
	imageAccurate->y = image->y;
	
	return imageAccurate;
}

// free memory of an AccurateImage
void freeImage(AccurateImage *image){
	free(image->data);
	free(image);
}

// Perform the new idea:
void performNewIdeaIteration(AccurateImage *imageOut, AccurateImage *imageIn,int size) {
	int countIncluded = 0;
	int offsetOfThePixel=0;
	//float sum_red = 0;
	//float sum_blue = 0;
	//float sum_green =0;
	v4f sum;
	int numberOfValuesInEachRow = imageIn->x;

	//TODO:  kanskje heller se på __m128 for å representere 4 floats enn å lage v4f

	v4f *imageArray = (v4f*) imageIn->data; // lager imageIn til en vector array.
	printf("imageIn->data[0] rgb is (%f, %f, %f) and trash is %f\n",imageIn->data[0].red, imageIn->data[0].green, imageIn->data[0].blue, imageIn->data[0].trash);
	printf("imageArray[0] 	rgb is  (%f, %f, %f) and trash is %f\n\n", imageArray[0][0], imageArray[0][1], imageArray[0][2], imageArray[0][3]);
	//printf("%i\n", imageArray[0][1]);
	//v4f *line_buffer;
	//posix_memalign(&line_buffer, 32, imageIn->x*sizeof(v4f));
	
	// line buffer that will save the sum of some pixel in the column
	v4f *line_buffer = (AccuratePixelSIMD*) malloc(imageIn->x*sizeof(AccuratePixelSIMD));
	//AccuratePixelSIMD *line_buffer = (AccuratePixelSIMD*) malloc(imageIn->x*sizeof(AccuratePixelSIMD));
	memset(line_buffer,0,imageIn->x*sizeof(AccuratePixelSIMD));
	
	//v4f *line_buffer; 
	//posix_memalign(&line_buffer, 32, imageIn->x*sizeof(16)); // v4f er 16 tror jeg

	// Iterate over each line of pixelx.
	for(int senterY = 0; senterY < imageIn->y; senterY++) {
		// first and last line considered  by the computation of the pixel in the line senterY
		int starty = senterY-size;
		int endy = senterY+size;

		if (starty <=0){
			starty = 0;
			if(senterY == 0){
				// for all pixel in the first line, we sum all pixel of the column (until the line endy)
				// we save the result in the array line_buffer
				for(int line_y=starty; line_y < endy; line_y++){
					for(int i=0; i<imageIn->x; i++){
						line_buffer[i] += imageArray[numberOfValuesInEachRow*line_y+i];
						//line_buffer[i].blue+=imageIn->data[numberOfValuesInEachRow*line_y+i].blue;
						//line_buffer[i].red+=imageIn->data[numberOfValuesInEachRow*line_y+i].red;
						//line_buffer[i].green+=imageIn->data[numberOfValuesInEachRow*line_y+i].green;
					}
				}
			}
			for(int i=0; i<imageIn->x; i++){
				// add the next pixel of the next line in the column x
				line_buffer[i] += imageArray[numberOfValuesInEachRow*endy+i];
				//line_buffer[i].blue+=imageIn->data[numberOfValuesInEachRow*endy+i].blue;
				//line_buffer[i].red+=imageIn->data[numberOfValuesInEachRow*endy+i].red;
				//line_buffer[i].green+=imageIn->data[numberOfValuesInEachRow*endy+i].green;
			}

		}

		else if (endy >= imageIn->y ){
			// for the last lines, we just need to subtract the first added line
			endy = imageIn->y-1;
			for(int i=0; i<imageIn->x; i++){
				line_buffer[i] -= imageArray[numberOfValuesInEachRow*(starty-1)+1];
				//line_buffer[i].blue-=imageIn->data[numberOfValuesInEachRow*(starty-1)+i].blue;
				//line_buffer[i].red-=imageIn->data[numberOfValuesInEachRow*(starty-1)+i].red;
				//line_buffer[i].green-=imageIn->data[numberOfValuesInEachRow*(starty-1)+i].green;
			}	
		}else{
			// general case
			// add the next line and remove the first added line
			for(int i=0; i<imageIn->x; i++){
				line_buffer[i] += imageArray[numberOfValuesInEachRow*endy+i] - imageArray[numberOfValuesInEachRow*(starty-1)+i];
				//line_buffer[i].blue+=imageIn->data[numberOfValuesInEachRow*endy+i].blue-imageIn->data[numberOfValuesInEachRow*(starty-1)+i].blue;
				//line_buffer[i].red+=imageIn->data[numberOfValuesInEachRow*endy+i].red-imageIn->data[numberOfValuesInEachRow*(starty-1)+i].red;
				//line_buffer[i].green+=imageIn->data[numberOfValuesInEachRow*endy+i].green-imageIn->data[numberOfValuesInEachRow*(starty-1)+i].green;
			}	
		}

		//sum_green =0;
		//sum_red = 0;
		//sum_blue = 0;
		sum *= 0; // resets the vector.
		for(int senterX = 0; senterX < imageIn->x; senterX++) {
			// in this loop, we do exactly the same thing as before but only with the array line_buffer

			int startx = senterX-size;
			int endx = senterX+size;

			if (startx <=0){
				startx = 0;
				if(senterX==0){
					for (int x=startx; x < endx; x++){
						sum += line_buffer[x];
						//sum_red += line_buffer[x].red;
						//sum_green += line_buffer[x].green;
						//sum_blue += line_buffer[x].blue;
					}
				}
				sum += line_buffer[endx];
				//sum_red +=line_buffer[endx].red;
				//sum_green +=line_buffer[endx].green;
				//sum_blue +=line_buffer[endx].blue;
			}else if (endx >= imageIn->x){
				endx = imageIn->x-1;
				sum -= line_buffer[startx-1];
				//sum_red -=line_buffer[startx-1].red;
				//sum_green -=line_buffer[startx-1].green;
				//sum_blue -=line_buffer[startx-1].blue;

			}else{
				sum += line_buffer[endx] - line_buffer[startx-1];
				//sum_red += (line_buffer[endx].red-line_buffer[startx-1].red);
				//sum_green += (line_buffer[endx].green-line_buffer[startx-1].green);
				//sum_blue += (line_buffer[endx].blue-line_buffer[startx-1].blue);
			}			

			// we save each pixel in the output image
			offsetOfThePixel = (numberOfValuesInEachRow * senterY + senterX);
			countIncluded=(endx-startx+1)*(endy-starty+1);

			imageOut->data[offsetOfThePixel].red = sum[0]/countIncluded;
			imageOut->data[offsetOfThePixel].green = sum[1]/countIncluded;
			imageOut->data[offsetOfThePixel].blue = sum[2]/countIncluded;
			//imageOut->data[offsetOfThePixel].red = sum_red/countIncluded;
			//imageOut->data[offsetOfThePixel].green = sum_green/countIncluded;
			//imageOut->data[offsetOfThePixel].blue = sum_blue/countIncluded;
		}
	
	}

	// free memory
	free(line_buffer);	
}

// Perform the final step, and save it as a ppm in imageOut
void performNewIdeaFinalization(AccurateImage *imageInSmall, AccurateImage *imageInLarge, PPMImage *imageOut) {

	
	imageOut->x = imageInSmall->x;
	imageOut->y = imageInSmall->y;
	
	for(int i = 0; i < imageInSmall->x * imageInSmall->y; i++) {
		float value = (imageInLarge->data[i].red - imageInSmall->data[i].red);

		if(value > 255)
			imageOut->data[i].red = 255;
		else if (value < -1) {
			value = 257.0+value;
			if(value > 255)
				imageOut->data[i].red = 255;
			else
				imageOut->data[i].red = floor(value);
		} else if (value > -1 && value < 0) {
			imageOut->data[i].red = 0;
		}  else {
			imageOut->data[i].red = floor(value);
		}
		
		value = (imageInLarge->data[i].green - imageInSmall->data[i].green);
		if(value > 255)
			imageOut->data[i].green = 255;
		else if (value < -1) {
			value = 257.0+value;
			if(value > 255)
				imageOut->data[i].green = 255;
			else
				imageOut->data[i].green = floor(value);
		} else if (value > -1 && value < 0) {
			imageOut->data[i].green = 0;
		} else {
			imageOut->data[i].green = floor(value);
		}
		
		value = (imageInLarge->data[i].blue - imageInSmall->data[i].blue);
		if(value > 255)
			imageOut->data[i].blue = 255;
		else if (value < -1) {
			value = 257.0+value;
			if(value > 255)
				imageOut->data[i].blue = 255;
			else
				imageOut->data[i].blue = floor(value);
		} else if (value > -1 && value < 0) {
			imageOut->data[i].blue = 0;
		} else {
			imageOut->data[i].blue = floor(value);
		}
	}
	
}


int main(int argc, char** argv) {

/*
	v4f a;
	a[0] = 0;
	a[1] = 1;
	a[2] = 2;
	a[3] = 3;
	v4f b = {0,2,3,10};
	b +=1;
	printf("%f,%f,%f,%f\n", b[0], b[1], b[2], b[3]);
*/

	PPMImage *image;
        
	if(argc > 1) {
		image = readPPM("flower.ppm");
	} else {
		image = readStreamPPM(stdin);
	}

	AccurateImage *imageUnchanged = convertImageToNewFormat(image); // save the unchanged image from input image
	AccurateImage *imageBuffer = createEmptyImage(image);
	AccurateImage *imageSmall = createEmptyImage(image);
	AccurateImage *imageBig = createEmptyImage(image);
	
	PPMImage *imageOut;
	imageOut = (PPMImage *)malloc(sizeof(PPMImage));
	imageOut->data = (PPMPixel*)malloc(image->x * image->y * sizeof(PPMPixel));

	// Process the tiny case:
	performNewIdeaIteration(imageSmall, imageUnchanged, 2);
	performNewIdeaIteration(imageBuffer, imageSmall, 2);
	performNewIdeaIteration(imageSmall, imageBuffer, 2);
	performNewIdeaIteration(imageBuffer, imageSmall, 2);
	performNewIdeaIteration(imageSmall, imageBuffer, 2);
	
	// Process the small case:
	performNewIdeaIteration(imageBig, imageUnchanged,3);
	performNewIdeaIteration(imageBuffer, imageBig,3);
	performNewIdeaIteration(imageBig, imageBuffer,3);
	performNewIdeaIteration(imageBuffer, imageBig,3);
	performNewIdeaIteration(imageBig, imageBuffer,3);
	
	// save tiny case result
	performNewIdeaFinalization(imageSmall,  imageBig, imageOut);
	if(argc > 1) {
		writePPM("flower_tiny.ppm", imageOut);
	} else {
		writeStreamPPM(stdout, imageOut);
	}

	
	// Process the medium case:
	performNewIdeaIteration(imageSmall, imageUnchanged, 5);
	performNewIdeaIteration(imageBuffer, imageSmall, 5);
	performNewIdeaIteration(imageSmall, imageBuffer, 5);
	performNewIdeaIteration(imageBuffer, imageSmall, 5);
	performNewIdeaIteration(imageSmall, imageBuffer, 5);
	
	// save small case
	performNewIdeaFinalization(imageBig,  imageSmall,imageOut);
	if(argc > 1) {
		writePPM("flower_small.ppm", imageOut);
	} else {
		writeStreamPPM(stdout, imageOut);
	}

	// process the large case
	performNewIdeaIteration(imageBig, imageUnchanged, 8);
	performNewIdeaIteration(imageBuffer, imageBig, 8);
	performNewIdeaIteration(imageBig, imageBuffer, 8);
	performNewIdeaIteration(imageBuffer, imageBig, 8);
	performNewIdeaIteration(imageBig, imageBuffer, 8);

	// save the medium case
	performNewIdeaFinalization(imageSmall,  imageBig, imageOut);
	if(argc > 1) {
		writePPM("flower_medium.ppm", imageOut);
	} else {
		writeStreamPPM(stdout, imageOut);
	}
	
	// free all memory structures
	freeImage(imageUnchanged);
	freeImage(imageBuffer);
	freeImage(imageSmall);
	freeImage(imageBig);
	free(imageOut->data);
	free(imageOut);
	free(image->data);
	free(image);
	
	return 0;
}

