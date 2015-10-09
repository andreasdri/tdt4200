#ifndef CLUTIL_H
#define CLUTIL_H
#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/opencl.h>
#endif

const char *clErrorStr(cl_int err);
void clError(char *s, cl_int err);
void printPlatformInfo(cl_platform_id platform);
void printDeviceInfo(cl_device_id device);
cl_kernel buildKernel(char* sourceFile, char* kernelName, char* options, cl_context context, cl_device_id device);

#endif
