#pragma once

#include "../def.h"
#include "../Attributes.h"
#include "../Vector.h"

#include <CL/cl.h>
#include <stdio.h>

#define __CL_TOOLS(ret) \
  __inline__ ret

#define __CL_TOOLS_VOID \
  __inline__ void

__CL_TOOLS(char *) CL_read_file(const char *path) {
  FILE *f = fopen(path, "rb");
  if (!f) {
    return NULL;
  }
  fseek(f, 0, SEEK_END);
  Ulong size = ftell(f);
  fseek(f, 0, SEEK_SET);
  char *buffer = (char *)malloc(size + 1);
  fread(buffer, 1, size, f);
  *(buffer + size) = '\0';
  fclose(f);
  return buffer;
}

__CL_TOOLS(cl_platform_id) CL_get_platform(void) {
  cl_platform_id platform;
  cl_int err = clGetPlatformIDs(1, &platform, NULL);
  if (err != CL_SUCCESS) {
    printf("Error with clGetPlatformIDs: %d\n", err);
    return NULL;
  }
  return platform;
}

__CL_TOOLS(cl_device_id) CL_get_device(cl_platform_id platform) {
  cl_device_id device;
  cl_int err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);
  if (err != CL_SUCCESS) {
    printf("Error with clGetDeviceIDs: %d\n", err);
    return NULL;
  }
  return device;
}

__CL_TOOLS(cl_context) CL_create_context(cl_device_id device) {
	cl_int err; 
	cl_context context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);
	if (err != CL_SUCCESS) {
		printf("clCreateContext ERROR: %d\n", err);
    return NULL;
	}
  return context;
}

__CL_TOOLS(cl_command_queue) CL_create_command_queue(cl_context context, cl_device_id device) {
	cl_int err;
  cl_command_queue queue = clCreateCommandQueueWithProperties(context, device, 0, &err);
	if (err != CL_SUCCESS) {
		printf("clCreateCommandQueueWithProperties ERROR: %d\n", err);
		return NULL;
	}
  return queue;
}

__CL_TOOLS(cl_program) CL_create_program(cl_context context, const char *path) {
  char *source = CL_read_file(path);
  if (!source) {
		fprintf(stderr, "Kernel source is empty.\n");
		exit(1);
	}
  const char *cstr = source;
	cl_int err;
	cl_program program = clCreateProgramWithSource(context, 1, &cstr, NULL, &err);
  free(source);
	if (err != CL_SUCCESS) {
		printf("clCreateProgramWithSource ERROR: %d\n", err);
		return NULL;
	}
  return program;
}

__CL_TOOLS(cl_int) CL_build_program(cl_program program, cl_device_id device) {
	cl_int err = clBuildProgram(program, 1, &device, NULL, NULL, NULL);
  if (err != CL_SUCCESS) {
    Ulong log_size;
    clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
    MVector<char> log(log_size);
    clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, log_size, log.data(), NULL);
    printf("ERROR: %d, Buildlog:\n%s\n", err, log.data());
  }
  return err;
}

__CL_TOOLS(cl_kernel) CL_create_kernel(cl_program program, const char *name) {
	cl_int err;
	cl_kernel kernel = clCreateKernel(program, name, &err);
  if (err != CL_SUCCESS) {
    printf("Failed to create kernel ERROR: %d.\n", err);
    return NULL;
  }
  return kernel;
}

template<typename T>
__CL_TOOLS(cl_mem) CL_create_mem_buf(cl_context context, cl_mem_flags flags, T *data, Uint buf_size) {
	cl_int err;
	cl_mem buf = clCreateBuffer(context, flags, (buf_size * sizeof(T)), data, &err);
	if (err != CL_SUCCESS) {
		printf("clCreateBuffer ERROR: %d.\n", err);
		return NULL;
	}
	return buf;
}

__CL_TOOLS(cl_int) CL_set_kernel_arg_ptr(cl_kernel kernel, Uint index, cl_mem buf) {
  cl_int err = clSetKernelArg(kernel, index, sizeof(cl_mem), &buf);
  if (err != CL_SUCCESS) {
    fprintf(stderr, "failed to set kernel arg: %u, ERROR: %d.\n", index, err);
  }
  return err;
}

__CL_TOOLS(cl_int) CL_enqueue_kernel(cl_kernel kernel, cl_command_queue queue, Ulong size) {
	Ulong s = size;
	cl_int err = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &s, NULL, 0, NULL, NULL);
  if (err != CL_SUCCESS) {
    fprintf(stderr, "Failed to enqueuekernel ERROR: %d.\n", err);
  }
  return err;
}

template <typename T> 
__CL_TOOLS(cl_int) CL_read_kernel_buf(cl_command_queue queue, cl_mem buf, T *data, Ulong buf_size) {
	cl_int err = clEnqueueReadBuffer(queue, buf, 	CL_TRUE, 0, (buf_size * sizeof(T)), data, 0, NULL, NULL);
	if (err != CL_SUCCESS) {
    fprintf(stderr, "Failed to read buffer ERROR: %d.\n", err);
  }
  return err;
}