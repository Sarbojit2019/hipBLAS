/* ************************************************************************
 * Copyright (C) 2016-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * ************************************************************************ */

#include "sycl_w.h"
#include <algorithm>
#include <exceptions.hpp>
#include <functional>
#include <hipblas.h>
//#include <hip/hip_runtime.h>
#include "deps/onemkl.h"
#include "hip/hip_interop.h"
//#include <math.h>

//using namespace oneapi;

void hip_init()
{
    // We need to call chip-spv init to make sure chip has got context and other handles created
    hipStream_t stream = nullptr;
    hipError_t  error;
    error = hipStreamCreate(&stream);
    uintptr_t nativeHandlers[4];
    int       numItems = 4;
    hipGetBackendNativeHandles((uintptr_t)stream, nativeHandlers, &numItems);
    sycl_init(nativeHandlers, &numItems);
}

hipblasStatus_t hipblasCreate(hipblasHandle_t* handle)
try
{
    hip_init();
    return syclblasCreate((syclblasHandle_t*)handle);
}
catch(...)
{
    return exception_to_hipblas_status();
}

hipblasStatus_t hipblasDestroy(hipblasHandle_t handle)
try
{
    return syclblasDestroy((syclblasHandle_t)handle);
}
catch(...)
{
    return exception_to_hipblas_status();
}

hipblasStatus_t hipblasSetStream(hipblasHandle_t handle, hipStream_t stream)
try
{
    // Obtain the handles to the LZ handlers.
    unsigned long lzHandles[4];
    int           nHandles = 0;
    //hiplzStreamNativeInfo(stream, lzHandles, &nHandles);

    //return syclblasSetStream((syclblasHandle_t)handle, nHandles, lzHandles, stream);
    return HIPBLAS_STATUS_NOT_SUPPORTED;
}
catch(...)
{
    return exception_to_hipblas_status();
}

hipblasStatus_t
    hipblasScopy(hipblasHandle_t handle, int n, const float* x, int incx, float* y, int incy)
try
{
    // print_me(); // coming from sycl_wrapper

    //onemklScopy(syclblasGetSyclQueue((syclblasHandle_t) handle), n, x, incx, y, incy);
    return HIPBLAS_STATUS_SUCCESS;
}
catch(...)
{
    return exception_to_hipblas_status();
}

hipblasStatus_t
    hipblasDcopy(hipblasHandle_t handle, int n, const double* x, int incx, double* y, int incy)
try
{
    // oneAPI call
    return HIPBLAS_STATUS_NOT_SUPPORTED;
    //return rocBLASStatusToHIPStatus(rocblas_dcopy((rocblas_handle)handle, n, x, incx, y, incy));
}
catch(...)
{
    return exception_to_hipblas_status();
}
