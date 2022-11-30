#include <iostream>

#include <include/ze_api.h>
#include <sycl.hpp>
#include <ext/oneapi/backend/level_zero.hpp>
#include <oneapi/mkl.hpp>
#include "sycl.h"
#include "sycl.hpp"

#define __HIP_PLATFORM_SPIRV__
#include "hipblas.h"

#include "sycl_w.h"

struct syclblasHandle
{
    syclPlatform_t platform;
    syclDevice_t   device;
    syclContext_t  context;
    syclQueue_t    queue;
    hipStream_t    hip_stream;

    syclblasHandle(void) :
      platform(), device(), context(), queue(), hip_stream() {}
    
    ~syclblasHandle() {
      syclQueueDestroy(queue);
      syclContextDestroy(context);
      syclDeviceDestroy(device);
      syclPlatformDestroy(platform);
    } 
};

hipblasStatus_t syclblasCreate(syclblasHandle_t* handle)
{
  if (handle != nullptr) {
    auto res = new syclblasHandle();
    // FIX Me:  needs to get the default NULL stream from HIP runtime to set  
  }
  return (handle != nullptr) ? HIPBLAS_STATUS_SUCCESS : HIPBLAS_STATUS_HANDLE_IS_NULLPTR;
}

hipblasStatus_t syclblasDestroy(syclblasHandle_t handle)
{
  if(handle != nullptr)
  {
    delete handle;
  }
  return (handle != nullptr) ? HIPBLAS_STATUS_SUCCESS : HIPBLAS_STATUS_HANDLE_IS_NULLPTR;
}

hipblasStatus_t syclblasSetStream(syclblasHandle_t     handle,
                                  unsigned long const* lzHandles,
                                  int                  nHandles,
                                  hipStream_t          stream)
{
    if(handle != nullptr)
    {
        handle->hip_stream = stream;
        // Obtain the handles to the LZ constructs.
        assert(nHandles == 4);
        auto hDriver  = (ze_driver_handle_t)lzHandles[0];
        auto hDevice  = (ze_device_handle_t)lzHandles[1];
        auto hContext = (ze_context_handle_t)lzHandles[2];
        auto hQueue   = (ze_command_queue_handle_t)lzHandles[3];

        // Build SYCL platform/device/queue from the LZ handles.
        syclPlatformCreate(&handle->platform, hDriver);
        syclDeviceCreate(&handle->device, handle->platform, hDevice);
        // FIX ME: only 1 device is returned from CHIP-SPV's lzHandles
        syclContextCreate(
            &handle->context, &handle->device, 1 /*ndevices*/, hContext, 1 /*keep_ownership*/);
        syclQueueCreate(&handle->queue, handle->context, hQueue, 1 /* keep ownership */);

        auto asyncExceptionHandler = [](sycl::exception_list exceptions) {
            // Report all asynchronous exceptions that occurred.
            for(std::exception_ptr const& e : exceptions)
            {
                try
                {
                    std::rethrow_exception(e);
                }
                catch(std::exception& e)
                {
                    std::cerr << "Async exception: " << e.what() << std::endl;
                }
            }

            // Rethrow the first asynchronous exception.
            for(std::exception_ptr const& e : exceptions)
            {
                std::rethrow_exception(e);
            }
        };
    }

    return (handle != nullptr) ? HIPBLAS_STATUS_SUCCESS : HIPBLAS_STATUS_HANDLE_IS_NULLPTR;
}

syclQueue_t syclblasGetSyclQueue(syclblasHandle_t handle)
{
  return handle->queue;
}

void print_me()
{
    std::cout << "From sycl_wrapper library" << std::endl;
}