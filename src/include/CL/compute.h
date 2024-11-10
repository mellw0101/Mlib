#pragma once

#include "../Pair.h"
#include "../String.h"
#include "../Vector.h"
#include "tools.h"

#include <CL/cl.h>

class ClComputeProgram {
 private:
  cl_platform_id                    platform;
  cl_device_id                      device;
  cl_context                        context;
  cl_command_queue                  queue;
  cl_program                        program;
  cl_kernel                         kernel;
  MVector<cl_mem>                   bufs;

 public:
  ~ClComputeProgram(void) {
    for (Uint i = 0; i < bufs.size(); ++i) {
      clReleaseMemObject(bufs[i]);
    }
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);
  }
  
  void init(const char *file_path, const char *kernel_name) {
    platform = CL_get_platform();
    device   = CL_get_device(platform);
    context  = CL_create_context(device);
    queue    = CL_create_command_queue(context, device);
    program  = CL_create_program(context, file_path);
    CL_build_program(program, device);
    kernel = CL_create_kernel(program, kernel_name);
  }

  template <typename T>
  inline cl_mem create_buffer(cl_mem_flags flags, T *data, Uint buf_size) {
    cl_mem buf = CL_create_mem_buf(context, flags, data, buf_size);
    bufs.push_back(buf);
    return buf;
  }

  template <typename T>
  inline cl_mem create_buffer_at_idx(Uint idx, cl_mem_flags flags, T *data, Uint buf_size) {
    cl_mem buf = CL_create_mem_buf(context, flags, data, buf_size);
    bufs.push_back(buf);
    set_kernel_arg_buffer_ptr(buf, idx);
    return buf;
  }

  template <typename T>
  inline void write_to_buffer(cl_mem buf, const T *data, Uint buf_size) {
    clEnqueueWriteBuffer(queue, buf, CL_TRUE, 0, (buf_size * sizeof(T)), data, 0, NULL, NULL);
  }

  template <typename T>
  inline cl_mem realloc_buffer(cl_mem ptr, cl_mem_flags flags, T *data, Uint buf_size) {
    for (Uint i = 0; i < bufs.size(); ++i) {
      if (bufs[i] == ptr) {
        clReleaseMemObject(bufs[i]);
        bufs[i] = CL_create_mem_buf(context, flags, data, buf_size);
        return bufs[i];
      }
    }
    return NULL;
  }

  template <typename T>
  inline cl_mem realloc_buffer_at_idx(Uint idx, cl_mem ptr, cl_mem_flags flags, T *data, Uint buf_size) {
    for (Uint i = 0; i < bufs.size(); ++i) {
      if (bufs[i] == ptr) {
        clReleaseMemObject(bufs[i]);
        bufs[i] = CL_create_mem_buf(context, flags, data, buf_size);
        set_kernel_arg_buffer_ptr(bufs[i], idx);
        return bufs[i];
      }
    }
    return NULL;
  }

  inline void set_kernel_arg_buffer_ptr(cl_mem buf, Uint index) {
    CL_set_kernel_arg_ptr(kernel, index, buf);
  }

  template <typename T>
  inline void read_kernel_buf(cl_mem buf, T *data, Uint buf_size) {
    CL_read_kernel_buf(queue, buf, data, buf_size);
  }

  inline void enqueue_kernel(Uint size) {
    CL_enqueue_kernel(kernel, queue, size);
  }
};
