#pragma once

#include <level_zero/ze_api.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct syclblasHandle* syclblasHandle_t;
bool                           sycl_init(uintptr_t* nativeHandles, int* numHandles);
hipblasStatus_t                syclblasCreate(syclblasHandle_t* handle);
hipblasStatus_t                syclblasDestroy(syclblasHandle_t handle);
hipblasStatus_t                syclblasSetStream(syclblasHandle_t handle, hipStream_t stream);
//syclQueue_t syclblasGetSyclQueue(syclblasHandle_t handle);

void print_me();

#ifdef __cplusplus
}
#endif
