#pragma once
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <CL/cl.h>
class CLbox 
{
    private:
    cl_platform_id platforms[64];
    unsigned int platformCount;
    cl_int Platformresult;
    cl_device_id device;
    cl_platform_id platform;
    cl_device_id devices[64];
    unsigned int deviceCount;
    cl_int deviceResult;
    char vendorName[256];
    size_t vendorNameLength;
    cl_int deviceInfoResult;
    cl_context context;
    cl_command_queue queue;
    cl_program program;
    public:
    cl_mem buffers[10];
    cl_kernel kernels[10];
    CLbox();
    void CreateProgram(const char* programSource);
    void CreateKernel(unsigned int index, const char* funcname);
    void CreateBuffer(unsigned int index, size_t size);
    void EnqueueWrite(unsigned int BufferIndex, size_t size, const void * ptr);
    void EnqueueRead(unsigned int BufferIndex, size_t size, void * ptr);
    void SetArg(unsigned int KernelIndex, cl_uint argindex, unsigned int BufferIndex);
    void EnqueKernel(unsigned int KernelIndex, cl_uint dimensions, const size_t* globalworksize);
    void await();
};
