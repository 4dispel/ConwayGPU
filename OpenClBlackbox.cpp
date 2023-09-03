#include "OpenClBlackbox.h"
#include <cassert>

CLbox::CLbox()
{
    Platformresult = clGetPlatformIDs( 64, platforms, &platformCount); 
    assert(Platformresult == CL_SUCCESS);
    device = nullptr;


    for(int i = 0; i < platformCount; ++i) {
        devices[64];
        deviceCount;
        deviceResult = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_GPU, 64, devices, &deviceCount);

        if ( deviceResult == CL_SUCCESS ) {
			for( int j = 0; j < deviceCount; ++j ) {
				vendorName[256];
				vendorNameLength;
				deviceInfoResult = clGetDeviceInfo( devices[j], CL_DEVICE_VENDOR, 256, vendorName, &vendorNameLength );
				if ( deviceInfoResult == CL_SUCCESS && std::string( vendorName ).substr( 0, vendorNameLength ) == "NVIDIA Corporation" ) {
					device = devices[j];
                    platform = platforms[i];
					break;
				}
			}
		}
    }

    cl_int contextResult;
    context = clCreateContext( nullptr, 1, &device, nullptr, nullptr, &contextResult );
	assert( contextResult == CL_SUCCESS );

    cl_int commandQueueResult;
	queue = clCreateCommandQueue( context, device, 0, &commandQueueResult );
	assert( commandQueueResult == CL_SUCCESS );
}

void CLbox::CreateProgram(const char* programSource)
{
    size_t length = 0;
    cl_int programresult;
    program = clCreateProgramWithSource(context, 1, &programSource, &length, &programresult);
    assert(programresult == CL_SUCCESS);

    cl_int programBuildresult = clBuildProgram(program, 1, &device, "", nullptr, nullptr);
    if (programBuildresult != CL_SUCCESS) {
        char log[256];
        size_t logLength;
        cl_int programBuildInforesult = clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, 256, log, &logLength);
        assert(programBuildInforesult == CL_SUCCESS);  
        std::cout << log << std::endl;
		assert( log );
    }
}

void CLbox::CreateKernel(unsigned int index, const char* funcname)
{
    cl_int kernelresult;
    kernels[index] = clCreateKernel(program, funcname, &kernelresult);
    assert(kernelresult == CL_SUCCESS);
}

void CLbox::CreateBuffer(unsigned int index, size_t size)
{
    cl_int currentresult;
    buffers[index] = clCreateBuffer( context, CL_MEM_READ_WRITE, size, nullptr, &currentresult);
    assert(currentresult == CL_SUCCESS);
}

void CLbox::EnqueueWrite(unsigned int BufferIndex, size_t size, const void * ptr)
{
    cl_int enqueueCurrentresult = clEnqueueWriteBuffer(queue, buffers[BufferIndex], CL_TRUE, 0, size, ptr, 0, nullptr, nullptr);
    assert(enqueueCurrentresult == CL_SUCCESS);
}

void CLbox::EnqueueRead(unsigned int BufferIndex, size_t size, void * ptr)
{
    cl_int enqueueReadBufferResult = clEnqueueReadBuffer( queue, buffers[BufferIndex], CL_TRUE, 0, size, ptr, 0, nullptr, nullptr );
    assert(enqueueReadBufferResult == CL_SUCCESS);
}


void CLbox::SetArg(unsigned int KernelIndex, cl_uint argindex, unsigned int BufferIndex)
{
    cl_int kernelArgaResult = clSetKernelArg( kernels[KernelIndex], argindex, sizeof(cl_mem), &(buffers[BufferIndex]) );
	assert( kernelArgaResult == CL_SUCCESS );
}

void CLbox::EnqueKernel(unsigned int KernelIndex, cl_uint dimensions, const size_t* globalworksize)
{
    //std::cout << KernelIndex << std::endl;
    //std::cout << dimensions << std::endl;
    //std::cout << globalworksize << std::endl;
    cl_int enqueKernelresult = clEnqueueNDRangeKernel(queue, kernels[KernelIndex], dimensions, 0, globalworksize , nullptr, 0, nullptr, nullptr);
    assert(enqueKernelresult == CL_SUCCESS);
}

void CLbox::await()
{
    clFinish(queue);
}