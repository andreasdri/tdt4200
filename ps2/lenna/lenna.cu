#include <iostream>
#include <stdio.h>
#include "lodepng.h"


__global__
void invert_bit_kernel(unsigned char* img) {
  int i = blockIdx.x * blockDim.x + threadIdx.x;
  unsigned char a = ~img[i];
  img[i] = a;
}

int main( int argc, char ** argv){

  size_t pngsize;
  unsigned char *png;
  const char * filename = "lenna512x512_inv.png";
  /* Read in the image */
  lodepng_load_file(&png, &pngsize, filename);

  unsigned char *image;
  unsigned int width, height;
  /* Decode it into a RGB 8-bit per channel vector */
  unsigned int error = lodepng_decode24(&image, &width, &height, png, pngsize);

  /* Check if read and decode of .png went well */
  if(error != 0){
      std::cout << "error " << error << ": " << lodepng_error_text(error) << std::endl;
  }

  unsigned char *cudaImage;
  unsigned int size = height * width * 3;

  /* Allocate and copy stuff to device */
  cudaMalloc((void**)&cudaImage, size);
  cudaMemcpy(cudaImage, image, size, cudaMemcpyHostToDevice);

  /* Maximum number of threads for the its-015 GPUs */
  int nThreads = 1024;
  int nBlocks = size / nThreads;

  // Do work
  invert_bit_kernel<<<nBlocks, nThreads>>>(cudaImage);

  /* Get stuff from device to host */
  cudaMemcpy(image, cudaImage, size, cudaMemcpyDeviceToHost);
  cudaFree(cudaImage);

  /* Save the result to a new .png file */
  lodepng_encode24_file("lenna512x512_orig.png", image, width, height);

  return 0;
}

