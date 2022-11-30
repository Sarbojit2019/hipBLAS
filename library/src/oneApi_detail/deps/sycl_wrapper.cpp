#include <iostream>

#include "sycl.h"
#include "sycl.hpp"
#include <ext/oneapi/backend/level_zero.hpp>
#include <include/ze_api.h>
#include <oneapi/mkl.hpp>

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

    syclblasHandle(void)
        : platform()
        , device()
        , context()
        , queue()
        , hip_stream()
    {
    }

    ~syclblasHandle()
    {
        syclQueueDestroy(queue);
        syclContextDestroy(context);
        syclDeviceDestroy(device);
        syclPlatformDestroy(platform);
    }
};

// Global syclblasHandle
syclblasHandle* g_SyclBlasHandle = nullptr;

bool sycl_init(uintptr_t* nativeHandles, int* numHandles)
{
    if(nativeHandles == nullptr or numHandles < 4)
    {
        return false;
    }
    // Extract the native information
    ze_driver_handle_t        hDriver  = (ze_driver_handle_t)nativeHandles[0];
    ze_device_handle_t        hDevice  = (ze_device_handle_t)nativeHandles[1];
    ze_context_handle_t       hContext = (ze_context_handle_t)nativeHandles[2];
    ze_command_queue_handle_t hQueue   = (ze_command_queue_handle_t)nativeHandles[3];

    sycl::platform sycl_platform = sycl::level_zero::make<sycl::platform>(hDriver);

    sycl::device sycl_device = sycl::level_zero::make<sycl::device>(sycl_platform, hDevice);
    std::vector<sycl::device> devices;
    devices.push_back(sycl_device);
    sycl::context sycl_context = sycl::level_zero::make<sycl::context>(devices, hContext);
    sycl::queue   sycl_queue   = sycl::level_zero::make<sycl::queue>(sycl_context, hQueue);

    if(g_SyclBlasHandle == nullptr)
    {
        g_SyclBlasHandle               = new syclblasHandle();
        g_SyclBlasHandle->platform.val = sycl_platform;
        g_SyclBlasHandle->device.val   = sycl_device;
        g_SyclBlasHandle->context      = sycl_context;
        g_SyclBlasHandle->queue        = sycl_queue;
    }
    return true;
}

hipblasStatus_t syclblasCreate(syclblasHandle_t* handle)
{
    if(handle != nullptr)
    {
        // This is error case as there is no native handle initialization has happened
        if(g_SyclBlasHandle == nullptr)
        {
            return HIPBLAS_STATUS_HANDLE_IS_NULLPTR;
        }
        // shall we do deep copy?
        handle = &g_SyclBlasHandle
        // FIX Me:  needs to get the default NULL stream from HIP runtime to set
    }
    return (handle != nullptr) ? HIPBLAS_STATUS_SUCCESS : HIPBLAS_STATUS_HANDLE_IS_NULLPTR;
}

hipblasStatus_t syclblasDestroy(syclblasHandle_t handle)
{
    if(handle != nullptr)
    {
        // release all sycl resources
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
