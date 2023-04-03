#include "onemkl.h"
#include "sycl.hpp"

#include <oneapi/mkl.hpp>

// SYCL & HIP backend used in blas, default is level0
hipBlasBackend hipblas_backend = level0;

// This is a workaround to flush MKL submissions into Level-zero queue, 
// using unspecified but guaranteed behavior of intel-sycl runtime. 
// Once SYCL standard committee approves sycl::queue::flush() we will change the macro to use the same 
#define __FORCE_MKL_FLUSH__(cmd)                 \
    if (hipblas_backend == opencl)               \
        get_native<sycl::backend::opencl>(cmd);  \
    else                                         \
        get_native<sycl::backend::ext_oneapi_level_zero>(cmd);

// gemm

// https://spec.oneapi.io/versions/1.0-rev-1/elements/oneMKL/source/domains/blas/gemm.html

oneapi::mkl::transpose convert(onemklTranspose val) {
    switch (val) {
    case ONEMKL_TRANSPOSE_NONTRANS:
        return oneapi::mkl::transpose::nontrans;
    case ONEMKL_TRANSPOSE_TRANS:
        return oneapi::mkl::transpose::trans;
    case ONEMLK_TRANSPOSE_CONJTRANS:
        return oneapi::mkl::transpose::conjtrans;
    }
}

oneapi::mkl::uplo convert(onemklUplo val) {
    switch(val) {
        case ONEMKL_UPLO_UPPER:
            return oneapi::mkl::uplo::upper;
        case ONEMKL_UPLO_LOWER:
            return oneapi::mkl::uplo::lower;
    }
}

oneapi::mkl::side convert(onemklSideMode val) {
    switch(val) {
        case ONEMKL_SIDE_LEFT:
            return oneapi::mkl::side::left;
        case ONEMKL_SIDE_RIGHT:
            return oneapi::mkl::side::right;
    }
}

oneapi::mkl::diag convert(onemklDiag val) {
    switch(val) {
        case ONEMKL_DIAG_NONUNIT:
            return oneapi::mkl::diag::nonunit;
        case ONEMKL_DIAG_UNIT:
            return oneapi::mkl::diag::unit;
    }
}

extern "C" void onemklHdot(syclQueue_t device_queue, int64_t n,
                           const short *x, int64_t incx, const short *y,
                           int64_t incy, short *result) {
    auto status = oneapi::mkl::blas::column_major::dot(device_queue->val, n,
                            reinterpret_cast<const sycl::half *>(x), incx,
                            reinterpret_cast<const sycl::half *>(y), incy,
                            reinterpret_cast<sycl::half *>(result));
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklSdot(syclQueue_t device_queue, int64_t n,
                           const float *x, int64_t incx, const float *y,
                           int64_t incy, float *result) {
    auto status = oneapi::mkl::blas::column_major::dot(device_queue->val, n, x,
                                                       incx, y, incy, result);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklDdot(syclQueue_t device_queue, int64_t n,
                           const double *x, int64_t incx, const double *y,
                           int64_t incy, double *result) {
    auto status = oneapi::mkl::blas::column_major::dot(device_queue->val, n, x,
                                                       incx, y, incy, result);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklCdotc(syclQueue_t device_queue, int64_t n,
                           const float _Complex *x, int64_t incx, const float _Complex *y,
                           int64_t incy, float _Complex *result) {
    auto status = oneapi::mkl::blas::column_major::dotc(device_queue->val, n,
                                                reinterpret_cast<const std::complex<float> *>(x), incx,
                                                reinterpret_cast<const std::complex<float> *>(y), incy,
                                                reinterpret_cast<std::complex<float> *>(result));
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklZdotc(syclQueue_t device_queue, int64_t n,
                           const double _Complex *x, int64_t incx, const double _Complex *y,
                           int64_t incy, double _Complex *result) {
    auto status = oneapi::mkl::blas::column_major::dotc(device_queue->val, n,
                                                reinterpret_cast<const std::complex<double> *>(x), incx,
                                                reinterpret_cast<const std::complex<double> *>(y), incy,
                                                reinterpret_cast<std::complex<double> *>(result));
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklCdotu(syclQueue_t device_queue, int64_t n,
                           const float _Complex *x, int64_t incx, const float _Complex *y,
                           int64_t incy, float _Complex *result) {
    auto status = oneapi::mkl::blas::column_major::dotu(device_queue->val, n,
                                                reinterpret_cast<const std::complex<float> *>(x), incx,
                                                reinterpret_cast<const std::complex<float> *>(y), incy,
                                                reinterpret_cast<std::complex<float> *>(result));
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklZdotu(syclQueue_t device_queue, int64_t n,
                           const double _Complex *x, int64_t incx, const double _Complex *y,
                           int64_t incy, double _Complex *result) {
    auto status = oneapi::mkl::blas::column_major::dotu(device_queue->val, n,
                                                reinterpret_cast<const std::complex<double> *>(x), incx,
                                                reinterpret_cast<const std::complex<double> *>(y), incy,
                                                reinterpret_cast<std::complex<double> *>(result));
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklSasum(syclQueue_t device_queue, int64_t n, 
                            const float *x, int64_t incx,
                            float *result) {
    auto status = oneapi::mkl::blas::column_major::asum(device_queue->val, n, x,
                                                        incx, result);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklDasum(syclQueue_t device_queue, int64_t n,
                            const double *x, int64_t incx,
                            double *result) {
    auto status = oneapi::mkl::blas::column_major::asum(device_queue->val, n, x,
                                                        incx, result);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklCasum(syclQueue_t device_queue, int64_t n,
                            const float _Complex *x, int64_t incx,
                            float *result) {
    auto status = oneapi::mkl::blas::column_major::asum(device_queue->val, n, 
                                        reinterpret_cast<const std::complex<float> *>(x),
                                        incx, result);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklZasum(syclQueue_t device_queue, int64_t n,
                            const double _Complex *x, int64_t incx,
                            double *result) {
    auto status = oneapi::mkl::blas::column_major::asum(device_queue->val, n, 
                                        reinterpret_cast<const std::complex<double> *>(x),
                                        incx, result);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklHaxpy(syclQueue_t device_queue, int64_t n, uint16_t alpha,
                            const short *x, std::int64_t incx, short *y, int64_t incy) {
    auto status = oneapi::mkl::blas::column_major::axpy(device_queue->val, n,
                            sycl::bit_cast<sycl::half>(alpha), reinterpret_cast<const sycl::half *>(x),
                            incx, reinterpret_cast<sycl::half *>(y), incy);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklSaxpy(syclQueue_t device_queue, int64_t n, float alpha,
                            const float *x, std::int64_t incx, float *y, int64_t incy) {
    auto status = oneapi::mkl::blas::column_major::axpy(device_queue->val, n, alpha, x,
                                                incx, y, incy);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklDaxpy(syclQueue_t device_queue, int64_t n, double alpha, 
                            const double *x, std::int64_t incx, double *y, int64_t incy) {
    auto status = oneapi::mkl::blas::column_major::axpy(device_queue->val, n, alpha, x,
                                                incx, y, incy);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklCaxpy(syclQueue_t device_queue, int64_t n, float _Complex alpha,
                        const float _Complex *x, std::int64_t incx, float _Complex *y, int64_t incy) {
    auto status = oneapi::mkl::blas::column_major::axpy(device_queue->val, n, alpha,
                            reinterpret_cast<const std::complex<float> *>(x), incx,
                            reinterpret_cast<std::complex<float> *>(y), incy);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklZaxpy(syclQueue_t device_queue, int64_t n, double _Complex alpha,
                        const double _Complex *x, std::int64_t incx, double _Complex *y, int64_t incy) {
    auto status = oneapi::mkl::blas::column_major::axpy(device_queue->val, n, alpha,
                            reinterpret_cast<const std::complex<double> *>(x), incx,
                            reinterpret_cast<std::complex<double> *>(y), incy);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklSaxpy_strided(syclQueue_t device_queue, int64_t n, float alpha, const float *x,
                int64_t incx, int64_t stridex, float *y, int64_t incy, int64_t stridey, int64_t batch_size) {
    auto status = oneapi::mkl::blas::column_major::axpy_batch(device_queue->val, n, alpha, x,
                                                incx, stridex, y, incy, stridey, batch_size);
    __FORCE_MKL_FLUSH__(status);
}
extern "C" void onemklDaxpy_strided(syclQueue_t device_queue, int64_t n, double alpha, const double *x,
                int64_t incx, int64_t stridex, double *y, int64_t incy, int64_t stridey, int64_t batch_size) {
    auto status = oneapi::mkl::blas::column_major::axpy_batch(device_queue->val, n, alpha, x,
                                                incx, stridex, y, incy, stridey, batch_size);
    __FORCE_MKL_FLUSH__(status);
}
extern "C" void onemklCaxpy_strided(syclQueue_t device_queue, int64_t n, float _Complex alpha,const float _Complex *x,
                int64_t incx, int64_t stridex, float _Complex *y, int64_t incy, int64_t stridey, int64_t batch_size) {
    auto status = oneapi::mkl::blas::column_major::axpy_batch(device_queue->val, n, alpha,
                            reinterpret_cast<const std::complex<float> *>(x), incx, stridex,
                            reinterpret_cast<std::complex<float> *>(y), incy, stridey, batch_size);
    __FORCE_MKL_FLUSH__(status);
}
extern "C" void onemklZaxpy_strided(syclQueue_t device_queue, int64_t n, double _Complex alpha, const double _Complex *x,
                int64_t incx, int64_t stridex, double _Complex *y, int64_t incy, int64_t stridey, int64_t batch_size) {
    auto status = oneapi::mkl::blas::column_major::axpy_batch(device_queue->val, n, alpha,
                            reinterpret_cast<const std::complex<double> *>(x), incx, stridex,
                            reinterpret_cast<std::complex<double> *>(y), incy, stridey, batch_size);
    __FORCE_MKL_FLUSH__(status);
}

// Support Level-1: SCAL primitive
extern "C" void onemklDscal(syclQueue_t device_queue, int64_t n, double alpha,
                            double *x, int64_t incx) {
    auto status = oneapi::mkl::blas::column_major::scal(device_queue->val, n, alpha,
                                                    x, incx);
    __FORCE_MKL_FLUSH__(status);

}

extern "C" void onemklSscal(syclQueue_t device_queue, int64_t n, float alpha,
                            float *x, int64_t incx) {
    auto status = oneapi::mkl::blas::column_major::scal(device_queue->val, n, alpha,
                                                         x, incx);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklCscal(syclQueue_t device_queue, int64_t n,
                            float _Complex alpha, float _Complex *x,
                            int64_t incx) {
    auto status = oneapi::mkl::blas::column_major::scal(device_queue->val, n,
                                        static_cast<std::complex<float> >(alpha),
                                        reinterpret_cast<std::complex<float> *>(x),incx);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklCsscal(syclQueue_t device_queue, int64_t n,
                            float alpha, float _Complex *x,
                            int64_t incx) {
    auto status = oneapi::mkl::blas::column_major::scal(device_queue->val, n, alpha,
                                        reinterpret_cast<std::complex<float> *>(x),incx);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklZscal(syclQueue_t device_queue, int64_t n,
                            double _Complex alpha, double _Complex *x,
                            int64_t incx) {
    auto status = oneapi::mkl::blas::column_major::scal(device_queue->val, n,
                                        static_cast<std::complex<double> >(alpha),
                                        reinterpret_cast<std::complex<double> *>(x),incx);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklZdscal(syclQueue_t device_queue, int64_t n,
                            double alpha, double _Complex *x,
                            int64_t incx) {
    auto status = oneapi::mkl::blas::column_major::scal(device_queue->val, n, alpha,
                                        reinterpret_cast<std::complex<double> *>(x),incx);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklDnrm2(syclQueue_t device_queue, int64_t n, const double *x,
                            int64_t incx, double *result) {
    auto status = oneapi::mkl::blas::column_major::nrm2(device_queue->val, n, x, incx, result);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklSnrm2(syclQueue_t device_queue, int64_t n, const float *x,
                            int64_t incx, float *result) {
    auto status = oneapi::mkl::blas::column_major::nrm2(device_queue->val, n, x, incx, result);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklCnrm2(syclQueue_t device_queue, int64_t n, const float _Complex *x,
                            int64_t incx, float *result) {
    auto status = oneapi::mkl::blas::column_major::nrm2(device_queue->val, n,
                    reinterpret_cast<const std::complex<float> *>(x), incx, result);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklZnrm2(syclQueue_t device_queue, int64_t n, const double _Complex *x,
                            int64_t incx, double *result) {
    auto status = oneapi::mkl::blas::column_major::nrm2(device_queue->val, n,
                    reinterpret_cast<const std::complex<double> *>(x), incx, result);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklDcopy(syclQueue_t device_queue, int64_t n, const double *x,
                            int64_t incx, double *y, int64_t incy) {
    auto status = oneapi::mkl::blas::column_major::copy(device_queue->val, n, x, incx, y, incy);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklScopy(syclQueue_t device_queue, int64_t n, const float *x,
                            int64_t incx, float *y, int64_t incy) {
    auto status = oneapi::mkl::blas::column_major::copy(device_queue->val, n, x, incx, y, incy);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklZcopy(syclQueue_t device_queue, int64_t n, const double _Complex *x,
                            int64_t incx, double _Complex *y, int64_t incy) {
    auto status = oneapi::mkl::blas::column_major::copy(device_queue->val, n,
        reinterpret_cast<const std::complex<double> *>(x), incx,
        reinterpret_cast<std::complex<double> *>(y), incy);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklCcopy(syclQueue_t device_queue, int64_t n, const float _Complex *x,
                            int64_t incx, float _Complex *y, int64_t incy) {
    auto status = oneapi::mkl::blas::column_major::copy(device_queue->val, n,
        reinterpret_cast<const std::complex<float> *>(x), incx,
        reinterpret_cast<std::complex<float> *>(y), incy);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklDamax(syclQueue_t device_queue, int64_t n, const double *x,
                            int64_t incx, int64_t *result){
    auto status = oneapi::mkl::blas::column_major::iamax(device_queue->val, n, x, incx, result);
    __FORCE_MKL_FLUSH__(status);
}
extern "C" void onemklSamax(syclQueue_t device_queue, int64_t n, const float  *x,
                            int64_t incx, int64_t *result){
    auto status = oneapi::mkl::blas::column_major::iamax(device_queue->val, n, x, incx, result);
    __FORCE_MKL_FLUSH__(status);
}
extern "C" void onemklZamax(syclQueue_t device_queue, int64_t n, const double _Complex *x,
                            int64_t incx, int64_t *result){
    auto status = oneapi::mkl::blas::column_major::iamax(device_queue->val, n,
                            reinterpret_cast<const std::complex<double> *>(x), incx, result);
    __FORCE_MKL_FLUSH__(status);
}
extern "C" void onemklCamax(syclQueue_t device_queue, int64_t n, const float _Complex *x,
                            int64_t incx, int64_t *result){
    auto status = oneapi::mkl::blas::column_major::iamax(device_queue->val, n,
                            reinterpret_cast<const std::complex<float> *>(x), incx, result);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklDamin(syclQueue_t device_queue, int64_t n, const double *x,
                            int64_t incx, int64_t *result){
    auto status = oneapi::mkl::blas::column_major::iamin(device_queue->val, n, x, incx, result);
    __FORCE_MKL_FLUSH__(status);
}
extern "C" void onemklSamin(syclQueue_t device_queue, int64_t n, const float  *x,
                            int64_t incx, int64_t *result){
    auto status = oneapi::mkl::blas::column_major::iamin(device_queue->val, n, x, incx, result);
    __FORCE_MKL_FLUSH__(status);
}
extern "C" void onemklZamin(syclQueue_t device_queue, int64_t n, const double _Complex *x,
                            int64_t incx, int64_t *result){
    auto status = oneapi::mkl::blas::column_major::iamin(device_queue->val, n,
                            reinterpret_cast<const std::complex<double> *>(x), incx, result);
    __FORCE_MKL_FLUSH__(status);
}
extern "C" void onemklCamin(syclQueue_t device_queue, int64_t n, const float _Complex *x,
                            int64_t incx, int64_t *result){
    auto status = oneapi::mkl::blas::column_major::iamin(device_queue->val, n,
                            reinterpret_cast<const std::complex<float> *>(x), incx, result);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklSswap(syclQueue_t device_queue, int64_t n, float *x, int64_t incx,\
                            float *y, int64_t incy){
    auto status = oneapi::mkl::blas::column_major::swap(device_queue->val, n, x, incx, y, incy);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklDswap(syclQueue_t device_queue, int64_t n, double *x, int64_t incx,
                            double *y, int64_t incy){
    auto status = oneapi::mkl::blas::column_major::swap(device_queue->val, n, x, incx, y, incy);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklCswap(syclQueue_t device_queue, int64_t n, float _Complex *x, int64_t incx,
                            float _Complex *y, int64_t incy){
    auto status = oneapi::mkl::blas::column_major::swap(device_queue->val, n,
                            reinterpret_cast<std::complex<float> *>(x), incx,
                            reinterpret_cast<std::complex<float> *>(y), incy);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklZswap(syclQueue_t device_queue, int64_t n, double _Complex *x, int64_t incx,
                            double _Complex *y, int64_t incy){
    auto status = oneapi::mkl::blas::column_major::swap(device_queue->val, n,
                            reinterpret_cast<std::complex<double> *>(x), incx,
                            reinterpret_cast<std::complex<double> *>(y), incy);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklSrot(syclQueue_t device_queue, int n, float* x, int incx, float* y, int incy,
                const float c, const float s){
    auto status = oneapi::mkl::blas::column_major::rot(device_queue->val, n, x, incx, y, incy, c, s);
    __FORCE_MKL_FLUSH__(status);
}
extern "C" void onemklDrot(syclQueue_t device_queue, int n, double* x, int incx, double* y, int incy,
                const double c, const double s){
    auto status = oneapi::mkl::blas::column_major::rot(device_queue->val, n, x, incx, y, incy, c, s);
    __FORCE_MKL_FLUSH__(status);
}
extern "C" void onemklCrot(syclQueue_t device_queue, int n, float _Complex* x, int incx, float _Complex* y, int incy,
                const float c, const float _Complex s){
    auto status = oneapi::mkl::blas::column_major::rot(device_queue->val, n,
                    reinterpret_cast<std::complex<float> *>(x), incx,
                    reinterpret_cast<std::complex<float> *>(y), incy,
                    c, static_cast<std::complex<float> >(s));
    __FORCE_MKL_FLUSH__(status);
}
extern "C" void onemklCsrot(syclQueue_t device_queue, int n, float _Complex* x, int incx, float _Complex* y, int incy,
                const float c, const float s){
    auto status = oneapi::mkl::blas::column_major::rot(device_queue->val, n,
                    reinterpret_cast<std::complex<float> *>(x), incx,
                    reinterpret_cast<std::complex<float> *>(y), incy, c, s);
    __FORCE_MKL_FLUSH__(status);
}
extern "C" void onemklZrot(syclQueue_t device_queue, int n, double _Complex* x, int incx, double _Complex* y, int incy,
                const double c, const double _Complex s){
    auto status = oneapi::mkl::blas::column_major::rot(device_queue->val, n,
                    reinterpret_cast<std::complex<double> *>(x), incx,
                    reinterpret_cast<std::complex<double> *>(y), incy,
                    c, static_cast<std::complex<double> >(s));
    __FORCE_MKL_FLUSH__(status);
}
extern "C" void onemklZdrot(syclQueue_t device_queue, int n, double _Complex* x, int incx, double _Complex* y, int incy,
                const double c, const double s){
    auto status = oneapi::mkl::blas::column_major::rot(device_queue->val, n,
                    reinterpret_cast<std::complex<double> *>(x), incx,
                    reinterpret_cast<std::complex<double> *>(y), incy, c, s);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklSrotg(syclQueue_t device_queue, float* a, float* b, float* c, float* s){
    auto status = oneapi::mkl::blas::column_major::rotg(device_queue->val, a, b, c, s);
    __FORCE_MKL_FLUSH__(status);
}
extern "C" void onemklDrotg(syclQueue_t device_queue, double* a, double* b, double* c, double* s){
    auto status = oneapi::mkl::blas::column_major::rotg(device_queue->val, a, b, c, s);
    __FORCE_MKL_FLUSH__(status);
}
extern "C" void onemklCrotg(syclQueue_t device_queue, float _Complex* a, float _Complex* b, float* c, float _Complex* s){
    auto status = oneapi::mkl::blas::column_major::rotg(device_queue->val,
                                                        reinterpret_cast<std::complex<float> *>(a),
                                                        reinterpret_cast<std::complex<float> *>(b), c,
                                                        reinterpret_cast<std::complex<float> *>(s));
    __FORCE_MKL_FLUSH__(status);
}
extern "C" void onemklZrotg(syclQueue_t device_queue, double _Complex* a, double _Complex* b, double* c, double _Complex* s){
    auto status = oneapi::mkl::blas::column_major::rotg(device_queue->val,
                                                        reinterpret_cast<std::complex<double> *>(a),
                                                        reinterpret_cast<std::complex<double> *>(b), c,
                                                        reinterpret_cast<std::complex<double> *>(s));
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklSrotm(syclQueue_t device_queue, int64_t n, float *x, int64_t incx,
                float *y, int64_t incy, float* param) {
    auto status = oneapi::mkl::blas::column_major::rotm(device_queue->val, n, x, incx, y, incy, param);
    __FORCE_MKL_FLUSH__(status);
}
extern "C" void onemklDrotm(syclQueue_t device_queue, int64_t n, double *x, int64_t incx,
                double *y, int64_t incy, double* param){
    auto status = oneapi::mkl::blas::column_major::rotm(device_queue->val, n, x, incx, y, incy, param);
    __FORCE_MKL_FLUSH__(status);
}

// Level-2
extern "C" void onemklSgbmv(syclQueue_t device_queue, onemklTranspose trans,
                            int64_t m, int64_t n, int64_t kl, int64_t ku,
                            float alpha, const float *a, int64_t lda,
                            const float *x, int64_t incx, float beta, float *y,
                            int64_t incy) {
    auto status = oneapi::mkl::blas::column_major::gbmv(device_queue->val,
                                convert(trans), m, n, kl, ku, alpha, a, lda, x,
                                incx, beta, y, incy);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklDgbmv(syclQueue_t device_queue, onemklTranspose trans,
                            int64_t m, int64_t n, int64_t kl, int64_t ku,
                            double alpha, const double *a, int64_t lda,
                            const double *x, int64_t incx, double beta, double *y,
                            int64_t incy) {
    auto status = oneapi::mkl::blas::column_major::gbmv(device_queue->val, convert(trans),
                                    m, n, kl, ku, alpha, a, lda, x, incx, beta, y, incy);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklCgbmv(syclQueue_t device_queue, onemklTranspose trans,
                            int64_t m, int64_t n, int64_t kl, int64_t ku,
                            float _Complex alpha, const float _Complex *a, int64_t lda,
                            const float _Complex *x, int64_t incx, float _Complex beta,
                            float _Complex *y, int64_t incy) {
    auto status = oneapi::mkl::blas::column_major::gbmv(device_queue->val, convert(trans),
                                    m, n, kl, ku, static_cast<std::complex<float> >(alpha),
                                    reinterpret_cast<const std::complex<float> *>(a),
                                    lda, reinterpret_cast<const std::complex<float> *>(x),
                                    incx, static_cast<std::complex<float> >(beta),
                                    reinterpret_cast<std::complex<float> *>(y), incy);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklZgbmv(syclQueue_t device_queue, onemklTranspose trans,
                            int64_t m, int64_t n, int64_t kl, int64_t ku,
                            double _Complex alpha, const double _Complex *a, int64_t lda,
                            const double _Complex *x, int64_t incx, double _Complex beta,
                            double _Complex *y, int64_t incy) {
    auto status = oneapi::mkl::blas::column_major::gbmv(device_queue->val, convert(trans), m,
                                        n, kl, ku, static_cast<std::complex<double> >(alpha),
                                        reinterpret_cast<const std::complex<double> *>(a),
                                        lda, reinterpret_cast<const std::complex<double> *>(x), incx,
                                        static_cast<std::complex<double> >(beta),
                                        reinterpret_cast<std::complex<double> *>(y), incy);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklSgemv(syclQueue_t device_queue, onemklTranspose trans,
                            int64_t m, int64_t n, float alpha, const float *a,
                            int64_t lda, const float *x, int64_t incx, float beta,
                            float *y, int64_t incy) {
    auto status = oneapi::mkl::blas::column_major::gemv(device_queue->val, convert(trans),
                                            m, n, alpha, a, lda, x, incx, beta, y, incy);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklDgemv(syclQueue_t device_queue, onemklTranspose trans,
                            int64_t m, int64_t n, double alpha, const double *a,
                            int64_t lda, const double *x, int64_t incx, double beta,
                            double *y, int64_t incy) {
    auto status = oneapi::mkl::blas::column_major::gemv(device_queue->val, convert(trans),
                                            m, n, alpha, a, lda, x, incx, beta, y, incy);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklCgemv(syclQueue_t device_queue, onemklTranspose trans,
                            int64_t m, int64_t n, float _Complex alpha,
                            const float _Complex *a, int64_t lda,
                            const float _Complex *x, int64_t incx,
                            float _Complex beta, float _Complex *y,
                            int64_t incy) {
    auto status = oneapi::mkl::blas::column_major::gemv(device_queue->val, convert(trans), m, n,
                                            static_cast<std::complex<float> >(alpha),
                                            reinterpret_cast<const std::complex<float> *>(a), lda,
                                            reinterpret_cast<const std::complex<float> *>(x), incx,
                                            static_cast<std::complex<float> >(beta),
                                            reinterpret_cast<std::complex<float> *>(y), incy);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklZgemv(syclQueue_t device_queue, onemklTranspose trans,
                            int64_t m, int64_t n, double _Complex alpha,
                            const double _Complex *a, int64_t lda,
                            const double _Complex *x, int64_t incx,
                            double _Complex beta, double _Complex *y,
                            int64_t incy) {
    auto status = oneapi::mkl::blas::column_major::gemv(device_queue->val, convert(trans), m, n,
                                            static_cast<std::complex<double> >(alpha),
                                            reinterpret_cast<const std::complex<double> *>(a), lda,
                                            reinterpret_cast<const std::complex<double> *>(x), incx,
                                            static_cast<std::complex<double> >(beta),
                                            reinterpret_cast<std::complex<double> *>(y), incy);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklSger(syclQueue_t device_queue, int64_t m, int64_t n, float alpha,
                           const float *x, int64_t incx, const float *y, int64_t incy,
                           float *a, int64_t lda) {
    auto status = oneapi::mkl::blas::column_major::ger(device_queue->val, m, n, alpha, x,
                                                    incx, y, incy, a, lda);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklDger(syclQueue_t device_queue, int64_t m, int64_t n, double alpha,
                           const double *x, int64_t incx, const double *y, int64_t incy,
                           double *a, int64_t lda) {
    auto status = oneapi::mkl::blas::column_major::ger(device_queue->val, m, n, alpha, x,
                                                    incx, y, incy, a, lda);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklCgerc(syclQueue_t device_queue, int64_t m, int64_t n, float _Complex alpha,
                           const float _Complex *x, int64_t incx, const float _Complex *y, int64_t incy,
                           float _Complex *a, int64_t lda) {
    auto status = oneapi::mkl::blas::column_major::gerc(device_queue->val, m, n,
                                            static_cast<std::complex<float> >(alpha),
                                            reinterpret_cast<const std::complex<float> *>(x), incx,
                                            reinterpret_cast<const std::complex<float> *>(y), incy,
                                            reinterpret_cast<std::complex<float> *>(a), lda);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklCgeru(syclQueue_t device_queue, int64_t m, int64_t n, float _Complex alpha,
                           const float _Complex *x, int64_t incx, const float _Complex *y, int64_t incy,
                           float _Complex *a, int64_t lda) {
    auto status = oneapi::mkl::blas::column_major::geru(device_queue->val, m, n,
                                            static_cast<std::complex<float> >(alpha),
                                            reinterpret_cast<const std::complex<float> *>(x), incx,
                                            reinterpret_cast<const std::complex<float> *>(y), incy,
                                            reinterpret_cast<std::complex<float> *>(a), lda);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklZgerc(syclQueue_t device_queue, int64_t m, int64_t n, double _Complex alpha,
                           const double _Complex *x, int64_t incx, const double _Complex *y, int64_t incy,
                           double _Complex *a, int64_t lda) {
    auto status = oneapi::mkl::blas::column_major::gerc(device_queue->val, m, n,
                                          static_cast<std::complex<float> >(alpha),
                                          reinterpret_cast<const std::complex<double> *>(x), incx,
                                          reinterpret_cast<const std::complex<double> *>(y), incy,
                                          reinterpret_cast<std::complex<double> *>(a), lda);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklZgeru(syclQueue_t device_queue, int64_t m, int64_t n, double _Complex alpha,
                           const double _Complex *x, int64_t incx, const double _Complex *y, int64_t incy,
                           double _Complex *a, int64_t lda) {
    auto status = oneapi::mkl::blas::column_major::geru(device_queue->val, m, n,
                                          static_cast<std::complex<float> >(alpha),
                                          reinterpret_cast<const std::complex<double> *>(x), incx,
                                          reinterpret_cast<const std::complex<double> *>(y), incy,
                                          reinterpret_cast<std::complex<double> *>(a), lda);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklChbmv(syclQueue_t device_queue, onemklUplo uplo, int64_t n,
                            int64_t k, float _Complex alpha, const float _Complex *a,
                            int64_t lda, const float _Complex *x, int64_t incx, float _Complex beta,
                            float _Complex *y, int64_t incy) {
    auto status = oneapi::mkl::blas::column_major::hbmv(device_queue->val, convert(uplo), n,
                                          k, static_cast<std::complex<float> >(alpha),
                                          reinterpret_cast<const std::complex<float> *>(a),
                                          lda, reinterpret_cast<const std::complex<float> *>(x),
                                          incx, static_cast<std::complex<float> >(beta),
                                          reinterpret_cast<std::complex<float> *>(y), incy);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklZhbmv(syclQueue_t device_queue, onemklUplo uplo, int64_t n,
                            int64_t k, double _Complex alpha, const double _Complex *a,
                            int64_t lda, const double _Complex *x, int64_t incx, double _Complex beta,
                            double _Complex *y, int64_t incy) {
    auto status = oneapi::mkl::blas::column_major::hbmv(device_queue->val, convert(uplo), n,
                                          k, static_cast<std::complex<double> >(alpha),
                                          reinterpret_cast<const std::complex<double> *>(a),
                                          lda, reinterpret_cast<const std::complex<double> *>(x),
                                          incx, static_cast<std::complex<double> >(beta),
                                          reinterpret_cast<std::complex<double> *>(y), incy);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklChemv(syclQueue_t device_queue, onemklUplo uplo, int64_t n,
                            float _Complex alpha, const float _Complex *a, int64_t lda,
                            const float _Complex *x, int64_t incx, float _Complex beta,
                            float _Complex *y, int64_t incy) {
    auto status = oneapi::mkl::blas::column_major::hemv(device_queue->val, convert(uplo), n,
                                          static_cast<std::complex<float> >(alpha),
                                          reinterpret_cast<const std::complex<float> *>(a),
                                          lda, reinterpret_cast<const std::complex<float> *>(x), incx,
                                          static_cast<std::complex<float> >(beta),
                                          reinterpret_cast<std::complex<float> *>(y), incy);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklZhemv(syclQueue_t device_queue, onemklUplo uplo, int64_t n,
                            double _Complex alpha, const double _Complex *a, int64_t lda,
                            const double _Complex *x, int64_t incx, double _Complex beta,
                            double _Complex *y, int64_t incy) {
    auto status = oneapi::mkl::blas::column_major::hemv(device_queue->val, convert(uplo), n,
                                          static_cast<std::complex<double> >(alpha),
                                          reinterpret_cast<const std::complex<double> *>(a),
                                          lda, reinterpret_cast<const std::complex<double> *>(x), incx,
                                          static_cast<std::complex<double> >(beta),
                                          reinterpret_cast<std::complex<double> *>(y), incy);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklCher(syclQueue_t device_queue, onemklUplo uplo, int64_t n, float alpha,
                           const float _Complex *x, int64_t incx, float _Complex *a,
                           int64_t lda) {
    auto status = oneapi::mkl::blas::column_major::her(device_queue->val, convert(uplo), n, alpha,
                                        reinterpret_cast<const std::complex<float> *>(x), incx,
                                        reinterpret_cast<std::complex<float> *>(a), lda);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklZher(syclQueue_t device_queue, onemklUplo uplo, int64_t n, double alpha,
                           const double _Complex *x, int64_t incx, double _Complex *a,
                           int64_t lda) {
    auto status = oneapi::mkl::blas::column_major::her(device_queue->val, convert(uplo), n, alpha,
                                        reinterpret_cast<const std::complex<double> *>(x), incx,
                                        reinterpret_cast<std::complex<double> *>(a), lda);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklCher2(syclQueue_t device_queue, onemklUplo uplo, int64_t n, float _Complex alpha,
                            const float _Complex *x, int64_t incx, const float _Complex *y, int64_t incy,
                            float _Complex *a, int64_t lda) {
    auto status = oneapi::mkl::blas::column_major::her2(device_queue->val, convert(uplo), n,
                                          static_cast<std::complex<float> >(alpha),
                                          reinterpret_cast<const std::complex<float> *>(x), incx,
                                          reinterpret_cast<const std::complex<float> *>(y), incy,
                                          reinterpret_cast<std::complex<float> *>(a), lda);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklZher2(syclQueue_t device_queue, onemklUplo uplo, int64_t n, double _Complex alpha,
                            const double _Complex *x, int64_t incx, const double _Complex *y, int64_t incy,
                            double _Complex *a, int64_t lda) {
    auto status = oneapi::mkl::blas::column_major::her2(device_queue->val, convert(uplo), n,
                                          static_cast<std::complex<double> >(alpha),
                                          reinterpret_cast<const std::complex<double> *>(x), incx,
                                          reinterpret_cast<const std::complex<double> *>(y), incy,
                                          reinterpret_cast<std::complex<double> *>(a), lda);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklChpmv(syclQueue_t device_queue, onemklUplo uplo, int64_t n, float _Complex alpha,
                const float _Complex *a, const float _Complex *x, int64_t incx,
                float _Complex beta, float _Complex *y, int64_t incy)
{
    auto status = oneapi::mkl::blas::column_major::hpmv(device_queue->val, convert(uplo), n,
                                        static_cast<std::complex<float> >(alpha),
                                        reinterpret_cast<const std::complex<float> *>(a),
                                        reinterpret_cast<const std::complex<float> *>(x), incx,
                                        static_cast<std::complex<float> >(beta),
                                        reinterpret_cast<std::complex<float> *>(y), incy);
    __FORCE_MKL_FLUSH__(status);
}
extern "C" void onemklZhpmv(syclQueue_t device_queue, onemklUplo uplo, int64_t n, double _Complex alpha,
                const double _Complex *a, const double _Complex *x, int64_t incx,
                double _Complex beta, double _Complex *y, int64_t incy)
{
    auto status = oneapi::mkl::blas::column_major::hpmv(device_queue->val, convert(uplo), n,
                                        static_cast<std::complex<double> >(alpha),
                                        reinterpret_cast<const std::complex<double> *>(a),
                                        reinterpret_cast<const std::complex<double> *>(x), incx,
                                        static_cast<std::complex<double> >(beta),
                                        reinterpret_cast<std::complex<double> *>(y), incy);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklChpr(syclQueue_t device_queue, onemklUplo uplo, int64_t n, float alpha,
                const float _Complex *x, int64_t incx, float _Complex *a)
{
    auto status = oneapi::mkl::blas::column_major::hpr(device_queue->val, convert(uplo), n,
                                        alpha, reinterpret_cast<const std::complex<float> *>(x), incx,
                                        reinterpret_cast<std::complex<float> *>(a));
    __FORCE_MKL_FLUSH__(status);
}
extern "C" void onemklZhpr(syclQueue_t device_queue, onemklUplo uplo, int64_t n, double alpha,
                const double _Complex *x, int64_t incx, double _Complex *a)
{
    auto status = oneapi::mkl::blas::column_major::hpr(device_queue->val, convert(uplo), n,
                                        alpha, reinterpret_cast<const std::complex<double> *>(x), incx,
                                        reinterpret_cast<std::complex<double> *>(a));
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklChpr2(syclQueue_t device_queue, onemklUplo uplo, int64_t n, float _Complex alpha,
                const float _Complex *x, int64_t incx, const float _Complex *y, int64_t incy, float _Complex *a)
{
    auto status = oneapi::mkl::blas::column_major::hpr2(device_queue->val, convert(uplo), n,
                                        static_cast<std::complex<float> >(alpha),
                                        reinterpret_cast<const std::complex<float> *>(x), incx,
                                        reinterpret_cast<const std::complex<float> *>(y), incy,
                                        reinterpret_cast<std::complex<float> *>(a));
    __FORCE_MKL_FLUSH__(status);
}
extern "C" void onemklZhpr2(syclQueue_t device_queue, onemklUplo uplo, int64_t n, double _Complex alpha,
                const double _Complex *x, int64_t incx, const double _Complex *y, int64_t incy, double _Complex *a)
{
    auto status = oneapi::mkl::blas::column_major::hpr2(device_queue->val, convert(uplo), n,
                                        static_cast<std::complex<double> >(alpha),
                                        reinterpret_cast<const std::complex<double> *>(x), incx,
                                        reinterpret_cast<const std::complex<double> *>(y), incy,
                                        reinterpret_cast<std::complex<double> *>(a));
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklSsbmv(syclQueue_t device_queue, onemklUplo uplo, int64_t n, int64_t k,
                            float alpha, const float *a, int64_t lda, const float *x,
                            int64_t incx, float beta, float *y, int64_t incy) {
    auto status = oneapi::mkl::blas::column_major::sbmv(device_queue->val, convert(uplo), n, k,
                                                    alpha, a, lda, x, incx, beta, y, incy);
    __FORCE_MKL_FLUSH__(status);
}
extern "C" void onemklDsbmv(syclQueue_t device_queue, onemklUplo uplo, int64_t n, int64_t k,
                            double alpha, const double *a, int64_t lda, const double *x,
                            int64_t incx, double beta, double *y, int64_t incy) {
    auto status = oneapi::mkl::blas::column_major::sbmv(device_queue->val, convert(uplo), n, k,
                                                    alpha, a, lda, x, incx, beta, y, incy);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklSspmv(syclQueue_t device_queue, onemklUplo uplo, int64_t n,
                            float alpha, const float *a, const float *x,
                            int64_t incx, float beta, float *y, int64_t incy) {
    auto status = oneapi::mkl::blas::column_major::spmv(device_queue->val, convert(uplo), n,
                                                    alpha, a, x, incx, beta, y, incy);
    __FORCE_MKL_FLUSH__(status);
}
extern "C" void onemklDspmv(syclQueue_t device_queue, onemklUplo uplo, int64_t n,
                            double alpha, const double *a, const double *x,
                            int64_t incx, double beta, double *y, int64_t incy) {
    auto status = oneapi::mkl::blas::column_major::spmv(device_queue->val, convert(uplo), n,
                                                    alpha, a, x, incx, beta, y, incy);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklSspr(syclQueue_t device_queue, onemklUplo uplo, int64_t n,
                 float alpha, const float *x, int64_t incx, float *a) {
    auto status = oneapi::mkl::blas::column_major::spr(device_queue->val, convert(uplo), n, alpha, x, incx, a);
    __FORCE_MKL_FLUSH__(status);
}
extern "C" void onemklDspr(syclQueue_t device_queue, onemklUplo uplo, int64_t n,
                 double alpha, const double *x, int64_t incx, double *a) {
    auto status = oneapi::mkl::blas::column_major::spr(device_queue->val, convert(uplo), n, alpha, x, incx, a);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklSspr2(syclQueue_t device_queue, onemklUplo uplo, int64_t n,
                 float alpha, const float *x, int64_t incx,
                 const float *y, int64_t incy, float *a) {
    auto status = oneapi::mkl::blas::column_major::spr2(device_queue->val, convert(uplo), n, alpha,
                 x, incx, y, incy, a);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklDspr2(syclQueue_t device_queue, onemklUplo uplo, int64_t n,
                 double alpha, const double *x, int64_t incx,
                 const double *y, int64_t incy, double *a) {
    auto status = oneapi::mkl::blas::column_major::spr2(device_queue->val, convert(uplo), n, alpha,
                 x, incx, y, incy, a);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklSsymv(syclQueue_t device_queue, onemklUplo uplo, int64_t n, float alpha,
                            const float *a, int64_t lda, const float *x, int64_t incx, float beta,
                            float *y, int64_t incy) {
    auto status = oneapi::mkl::blas::column_major::symv(device_queue->val, convert(uplo), n, alpha,
                                                    a, lda, x, incx, beta, y, incy);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklDsymv(syclQueue_t device_queue, onemklUplo uplo, int64_t n, double alpha,
                            const double *a, int64_t lda, const double *x, int64_t incx, double beta,
                            double *y, int64_t incy) {
    auto status = oneapi::mkl::blas::column_major::symv(device_queue->val, convert(uplo), n, alpha,
                                                    a, lda, x, incx, beta, y, incy);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklCsymv(syclQueue_t device_queue, onemklUplo uplo, int64_t n, float _Complex alpha,
                 const float _Complex *a, int64_t lda, const float _Complex *x, int64_t incx,
                 float _Complex beta, float _Complex *y, int64_t incy) {
    auto status = oneapi::mkl::blas::column_major::symv(device_queue->val, convert(uplo), n,
                                                    static_cast<std::complex<float>>(alpha),
                                                    reinterpret_cast<const std::complex<float> *>(a), lda,
                                                    reinterpret_cast<const std::complex<float> *>(x), incx,
                                                    static_cast<std::complex<float>>(beta),
                                                    reinterpret_cast<std::complex<float>*>(y), incy);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklZsymv(syclQueue_t device_queue, onemklUplo uplo, int64_t n,
                 double _Complex alpha, const double _Complex *a, int64_t lda, const double _Complex *x,
                 int64_t incx, double _Complex beta, double _Complex *y, int64_t incy) {
    auto status = oneapi::mkl::blas::column_major::symv(device_queue->val, convert(uplo), n,
                                                    static_cast<std::complex<double>>(alpha),
                                                    reinterpret_cast<const std::complex<double> *>(a), lda,
                                                    reinterpret_cast<const std::complex<double> *>(x), incx,
                                                    static_cast<std::complex<double>>(beta),
                                                    reinterpret_cast<std::complex<double>*>(y), incy);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklSsyr(syclQueue_t device_queue, onemklUplo uplo, int64_t n, float alpha,
                           const float *x, int64_t incx, float *a, int64_t lda) {
    auto status = oneapi::mkl::blas::column_major::syr(device_queue->val, convert(uplo), n, alpha,
                                                    x, incx, a, lda);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklDsyr(syclQueue_t device_queue, onemklUplo uplo, int64_t n, double alpha,
                           const double *x, int64_t incx, double *a, int64_t lda) {
    auto status = oneapi::mkl::blas::column_major::syr(device_queue->val, convert(uplo), n, alpha,
                                                    x, incx, a, lda);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklCsyr(syclQueue_t device_queue, onemklUplo uplo, int64_t n, float _Complex alpha,
                           const float _Complex *x, int64_t incx, float _Complex *a, int64_t lda) {
    auto status = oneapi::mkl::blas::column_major::syr(device_queue->val, convert(uplo), n,
                                                    static_cast<std::complex<float>>(alpha),
                                                    reinterpret_cast<const std::complex<float> *>(x), incx,
                                                    reinterpret_cast<std::complex<float> *>(a), lda);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklZsyr(syclQueue_t device_queue, onemklUplo uplo, int64_t n, double _Complex alpha,
                           const double _Complex *x, int64_t incx, double _Complex *a, int64_t lda) {
    auto status = oneapi::mkl::blas::column_major::syr(device_queue->val, convert(uplo), n,
                                                    static_cast<std::complex<double>>(alpha),
                                                    reinterpret_cast<const std::complex<double> *>(x), incx,
                                                    reinterpret_cast<std::complex<double> *>(a), lda);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklSsyr2(syclQueue_t device_queue, onemklUplo uplo, int64_t n, float alpha,
                           const float *x, int64_t incx, const float *y, int64_t incy, float *a, int64_t lda)
{
    auto status = oneapi::mkl::blas::column_major::syr2(device_queue->val, convert(uplo), n, alpha, x, incx, y, incy, a, lda);
    __FORCE_MKL_FLUSH__(status);
}
extern "C" void onemklDsyr2(syclQueue_t device_queue, onemklUplo uplo, int64_t n, double alpha,
                           const double *x, int64_t incx, const double *y, int64_t incy, double *a, int64_t lda)

{
    auto status = oneapi::mkl::blas::column_major::syr2(device_queue->val, convert(uplo), n, alpha, x, incx, y, incy, a, lda);
    __FORCE_MKL_FLUSH__(status);
}
extern "C" void onemklCsyr2(syclQueue_t device_queue, onemklUplo uplo, int64_t n, float _Complex alpha,
                           const float _Complex *x, int64_t incx, const float _Complex *y, int64_t incy,
                           float _Complex *a, int64_t lda) {
    auto status = oneapi::mkl::blas::column_major::syr2(device_queue->val, convert(uplo), n,
                           static_cast<std::complex<float>>(alpha),
                           reinterpret_cast<const std::complex<float> *>(x), incx,
                           reinterpret_cast<const std::complex<float> *>(y), incy,
                           reinterpret_cast<std::complex<float> *>(a), lda);
    __FORCE_MKL_FLUSH__(status);
}
extern "C" void onemklZsyr2(syclQueue_t device_queue, onemklUplo uplo, int64_t n, double _Complex alpha,
                           const double _Complex *x, int64_t incx, const double _Complex *y, int64_t incy,
                           double _Complex *a, int64_t lda){
    auto status = oneapi::mkl::blas::column_major::syr2(device_queue->val, convert(uplo), n,
                           static_cast<std::complex<double>>(alpha),
                           reinterpret_cast<const std::complex<double> *>(x), incx,
                           reinterpret_cast<const std::complex<double> *>(y), incy,
                           reinterpret_cast<std::complex<double> *>(a), lda);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklStbmv(syclQueue_t device_queue, onemklUplo uplo,
                            onemklTranspose trans, onemklDiag diag, int64_t n,
                            int64_t k, const float *a, int64_t lda, float *x, int64_t incx) {
    auto status = oneapi::mkl::blas::column_major::tbmv(device_queue->val, convert(uplo), convert(trans),
                                                        convert(diag), n, k, a, lda, x, incx);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklDtbmv(syclQueue_t device_queue, onemklUplo uplo,
                            onemklTranspose trans, onemklDiag diag, int64_t n,
                            int64_t k, const double *a, int64_t lda, double *x, int64_t incx) {
    auto status = oneapi::mkl::blas::column_major::tbmv(device_queue->val, convert(uplo), convert(trans),
                                                    convert(diag), n, k, a, lda, x, incx);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklCtbmv(syclQueue_t device_queue, onemklUplo uplo,
                            onemklTranspose trans, onemklDiag diag, int64_t n,
                            int64_t k, const float _Complex *a, int64_t lda, float _Complex *x,
                            int64_t incx) {
    auto status = oneapi::mkl::blas::column_major::tbmv(device_queue->val, convert(uplo), convert(trans),
                                            convert(diag), n, k, reinterpret_cast<const std::complex<float> *>(a),
                                            lda, reinterpret_cast<std::complex<float> *>(x), incx);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklZtbmv(syclQueue_t device_queue, onemklUplo uplo,
                            onemklTranspose trans, onemklDiag diag, int64_t n,
                            int64_t k, const double _Complex *a, int64_t lda, double _Complex *x,
                            int64_t incx) {
    auto status = oneapi::mkl::blas::column_major::tbmv(device_queue->val, convert(uplo), convert(trans),
                                        convert(diag), n, k, reinterpret_cast<const std::complex<double> *>(a),
                                        lda, reinterpret_cast<std::complex<double> *>(x), incx);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklStbsv(syclQueue_t device_queue, onemklUplo uplo,
                            onemklTranspose trans, onemklDiag diag, int64_t n,
                            int64_t k, const float *a, int64_t lda, float *x, int64_t incx) {
    auto status = oneapi::mkl::blas::column_major::tbsv(device_queue->val, convert(uplo), convert(trans),
                                                        convert(diag), n, k, a, lda, x, incx);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklDtbsv(syclQueue_t device_queue, onemklUplo uplo,
                            onemklTranspose trans, onemklDiag diag, int64_t n,
                            int64_t k, const double *a, int64_t lda, double *x, int64_t incx) {
    auto status = oneapi::mkl::blas::column_major::tbsv(device_queue->val, convert(uplo), convert(trans),
                                                    convert(diag), n, k, a, lda, x, incx);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklCtbsv(syclQueue_t device_queue, onemklUplo uplo,
                            onemklTranspose trans, onemklDiag diag, int64_t n,
                            int64_t k, const float _Complex *a, int64_t lda, float _Complex *x,
                            int64_t incx) {
    auto status = oneapi::mkl::blas::column_major::tbsv(device_queue->val, convert(uplo), convert(trans),
                                            convert(diag), n, k, reinterpret_cast<const std::complex<float> *>(a),
                                            lda, reinterpret_cast<std::complex<float> *>(x), incx);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklZtbsv(syclQueue_t device_queue, onemklUplo uplo,
                            onemklTranspose trans, onemklDiag diag, int64_t n,
                            int64_t k, const double _Complex *a, int64_t lda, double _Complex *x,
                            int64_t incx) {
    auto status = oneapi::mkl::blas::column_major::tbsv(device_queue->val, convert(uplo), convert(trans),
                                        convert(diag), n, k, reinterpret_cast<const std::complex<double> *>(a),
                                        lda, reinterpret_cast<std::complex<double> *>(x), incx);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklStpmv(syclQueue_t device_queue, onemklUplo uplo,
                onemklTranspose trans, onemklDiag diag, int64_t n,
                const float *a, float *x, int64_t incx) {
    auto status = oneapi::mkl::blas::column_major::tpmv(device_queue->val, convert(uplo), convert(trans),
                                                    convert(diag), n, a, x, incx);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklDtpmv(syclQueue_t device_queue, onemklUplo uplo,
                onemklTranspose trans, onemklDiag diag, int64_t n,
                const double *a, double *x, int64_t incx) {
    auto status = oneapi::mkl::blas::column_major::tpmv(device_queue->val, convert(uplo), convert(trans),
                                                    convert(diag), n, a, x, incx);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklCtpmv(syclQueue_t device_queue, onemklUplo uplo,
                onemklTranspose trans, onemklDiag diag, int64_t n,
                const float _Complex *a, float _Complex *x, int64_t incx) {
    auto status = oneapi::mkl::blas::column_major::tpmv(device_queue->val, convert(uplo), convert(trans),
                                            convert(diag), n, reinterpret_cast<const std::complex<float> *>(a),
                                            reinterpret_cast<std::complex<float> *>(x), incx);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklZtpmv(syclQueue_t device_queue, onemklUplo uplo,
                onemklTranspose trans, onemklDiag diag, int64_t n,
                const double _Complex *a, double _Complex *x, int64_t incx) {
    auto status = oneapi::mkl::blas::column_major::tpmv(device_queue->val, convert(uplo), convert(trans),
                                        convert(diag), n, reinterpret_cast<const std::complex<double> *>(a),
                                        reinterpret_cast<std::complex<double> *>(x), incx);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklStpsv(syclQueue_t device_queue, onemklUplo uplo,
                onemklTranspose trans, onemklDiag diag, int64_t m,
                const float *a, float *x, int64_t incx) {
    auto status = oneapi::mkl::blas::column_major::tpsv(device_queue->val, convert(uplo), convert(trans),
                                                    convert(diag), m, a, x, incx);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklDtpsv(syclQueue_t device_queue, onemklUplo uplo,
                onemklTranspose trans, onemklDiag diag, int64_t m,
                const double *a, double *x, int64_t incx) {
    auto status = oneapi::mkl::blas::column_major::tpsv(device_queue->val, convert(uplo), convert(trans),
                                                    convert(diag), m, a, x, incx);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklCtpsv(syclQueue_t device_queue, onemklUplo uplo,
                onemklTranspose trans, onemklDiag diag, int64_t m,
                const float _Complex *a, float _Complex *x, int64_t incx) {
    auto status = oneapi::mkl::blas::column_major::tpsv(device_queue->val, convert(uplo), convert(trans),
                                            convert(diag), m, reinterpret_cast<const std::complex<float> *>(a),
                                            reinterpret_cast<std::complex<float> *>(x), incx);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklZtpsv(syclQueue_t device_queue, onemklUplo uplo,
                onemklTranspose trans, onemklDiag diag, int64_t m,
                const double _Complex *a, double _Complex *x, int64_t incx) {
    auto status = oneapi::mkl::blas::column_major::tpsv(device_queue->val, convert(uplo), convert(trans),
                                        convert(diag), m, reinterpret_cast<const std::complex<double> *>(a),
                                        reinterpret_cast<std::complex<double> *>(x), incx);
    __FORCE_MKL_FLUSH__(status);
}

// trmv - level2
extern "C" void onemklStrmv(syclQueue_t device_queue, onemklUplo uplo, onemklTranspose trans,
                            onemklDiag diag, int64_t n, const float *a, int64_t lda, float *x,
                            int64_t incx) {
    auto status = oneapi::mkl::blas::column_major::trmv(device_queue->val, convert(uplo), convert(trans),
                                        convert(diag), n, a, lda, x, incx);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklDtrmv(syclQueue_t device_queue, onemklUplo uplo, onemklTranspose trans,
                            onemklDiag diag, int64_t n, const double *a, int64_t lda, double *x,
                            int64_t incx) {
    auto status = oneapi::mkl::blas::column_major::trmv(device_queue->val, convert(uplo), convert(trans),
                                        convert(diag), n, a, lda, x, incx);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklCtrmv(syclQueue_t device_queue, onemklUplo uplo, onemklTranspose trans,
                            onemklDiag diag, int64_t n, const float _Complex *a, int64_t lda, float _Complex *x,
                            int64_t incx) {
    auto status = oneapi::mkl::blas::column_major::trmv(device_queue->val, convert(uplo), convert(trans),
                                        convert(diag), n, reinterpret_cast<const std::complex<float> *>(a),
                                        lda, reinterpret_cast<std::complex<float> *>(x), incx);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklZtrmv(syclQueue_t device_queue, onemklUplo uplo, onemklTranspose trans,
                            onemklDiag diag, int64_t n, const double _Complex *a, int64_t lda, double _Complex *x,
                            int64_t incx) {
    auto status = oneapi::mkl::blas::column_major::trmv(device_queue->val, convert(uplo), convert(trans),
                                        convert(diag), n, reinterpret_cast<const std::complex<double> *>(a),
                                        lda, reinterpret_cast<std::complex<double> *>(x), incx);
    __FORCE_MKL_FLUSH__(status);
}

// trsv
extern "C" void onemklStrsv(syclQueue_t device_queue, onemklUplo uplo, onemklTranspose trans,
                            onemklDiag diag, int64_t n, const float *a, int64_t lda, float *x,
                            int64_t incx) {
    auto status = oneapi::mkl::blas::column_major::trsv(device_queue->val, convert(uplo), convert(trans),
                                          convert(diag), n, a, lda, x, incx);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklDtrsv(syclQueue_t device_queue, onemklUplo uplo, onemklTranspose trans,
                            onemklDiag diag, int64_t n, const double *a, int64_t lda, double *x,
                            int64_t incx) {
    auto status = oneapi::mkl::blas::column_major::trsv(device_queue->val, convert(uplo), convert(trans),
                                          convert(diag), n, a, lda, x, incx);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklCtrsv(syclQueue_t device_queue, onemklUplo uplo, onemklTranspose trans,
                            onemklDiag diag, int64_t n, const float  _Complex *a, int64_t lda,
                            float _Complex *x, int64_t incx) {
    auto status = oneapi::mkl::blas::column_major::trsv(device_queue->val, convert(uplo), convert(trans),
                                          convert(diag), n, reinterpret_cast<const std::complex<float> *>(a),
                                          lda, reinterpret_cast<std::complex<float> *>(x), incx);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklZtrsv(syclQueue_t device_queue, onemklUplo uplo, onemklTranspose trans,
                            onemklDiag diag, int64_t n, const double _Complex *a, int64_t lda,
                            double _Complex *x, int64_t incx) {
    auto status = oneapi::mkl::blas::column_major::trsv(device_queue->val, convert(uplo), convert(trans),
                                          convert(diag), n, reinterpret_cast<const std::complex<double> *>(a),
                                          lda, reinterpret_cast<std::complex<double> *>(x), incx);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" int onemklHgemm(syclQueue_t device_queue, onemklTranspose transA,
                           onemklTranspose transB, int64_t m, int64_t n,
                           int64_t k, uint16_t alpha, const short *A, int64_t lda,
                           const short *B, int64_t ldb, uint16_t beta, short *C,
                           int64_t ldc) {
    auto status = oneapi::mkl::blas::column_major::gemm(device_queue->val, convert(transA),
                                          convert(transB), m, n, k,sycl::bit_cast<sycl::half>(alpha),
                                          reinterpret_cast<const sycl::half *>(A), lda,
                                          reinterpret_cast<const sycl::half *>(B), ldb,
                                          sycl::bit_cast<sycl::half>(beta),
                                          reinterpret_cast<sycl::half *>(C), ldc);
    __FORCE_MKL_FLUSH__(status);
    return 0;
}

extern "C" int onemklSgemm(syclQueue_t device_queue, onemklTranspose transA,
                           onemklTranspose transB, int64_t m, int64_t n,
                           int64_t k, float alpha, const float *A, int64_t lda,
                           const float *B, int64_t ldb, float beta, float *C,
                           int64_t ldc) {
    auto status = oneapi::mkl::blas::column_major::gemm(device_queue->val, convert(transA),
                                          convert(transB), m, n, k, alpha, A,
                                          lda, B, ldb, beta, C, ldc);
    __FORCE_MKL_FLUSH__(status);
    return 0;
}

extern "C" int onemklDgemm(syclQueue_t device_queue, onemklTranspose transA,
                           onemklTranspose transB, int64_t m, int64_t n,
                           int64_t k, double alpha, const double *A,
                           int64_t lda, const double *B, int64_t ldb,
                           double beta, double *C, int64_t ldc) {
    auto status = oneapi::mkl::blas::column_major::gemm(device_queue->val, convert(transA),
                                          convert(transB), m, n, k, alpha, A,
                                          lda, B, ldb, beta, C, ldc);
    __FORCE_MKL_FLUSH__(status);
    return 0;
}

extern "C" int onemklCgemm(syclQueue_t device_queue, onemklTranspose transA,
                           onemklTranspose transB, int64_t m, int64_t n,
                           int64_t k, float _Complex alpha,
                           const float _Complex *A, int64_t lda,
                           const float _Complex *B, int64_t ldb,
                           float _Complex beta, float _Complex *C,
                           int64_t ldc) {
    auto status = oneapi::mkl::blas::column_major::gemm(
        device_queue->val, convert(transA), convert(transB), m, n, k, alpha,
        reinterpret_cast<const std::complex<float> *>(A), lda,
        reinterpret_cast<const std::complex<float> *>(B), ldb, beta,
        reinterpret_cast<std::complex<float> *>(C), ldc);
    __FORCE_MKL_FLUSH__(status);
    return 0;
}

extern "C" int onemklZgemm(syclQueue_t device_queue, onemklTranspose transA,
                           onemklTranspose transB, int64_t m, int64_t n,
                           int64_t k, double _Complex alpha,
                           const double _Complex *A, int64_t lda,
                           const double _Complex *B, int64_t ldb,
                           double _Complex beta, double _Complex *C,
                           int64_t ldc) {
    auto status = oneapi::mkl::blas::column_major::gemm(
        device_queue->val, convert(transA), convert(transB), m, n, k, alpha,
        reinterpret_cast<const std::complex<double> *>(A), lda,
        reinterpret_cast<const std::complex<double> *>(B), ldb, beta,
        reinterpret_cast<std::complex<double> *>(C), ldc);
    __FORCE_MKL_FLUSH__(status);
    return 0;
}

extern "C" int onemklSgemm_strided(syclQueue_t device_queue, onemklTranspose transA,
                onemklTranspose transB, int64_t m, int64_t n, int64_t k,
                float alpha, const float *A, int64_t lda, int64_t stridea,
                const float *B, int64_t ldb, int64_t strideb, float beta,
                float *C, int64_t ldc, int64_t stridec, int64_t batch_size) {
    auto status = oneapi::mkl::blas::column_major::gemm_batch(device_queue->val, convert(transA),
                                          convert(transB), m, n, k, alpha, A,
                                          lda, stridea, B, ldb, strideb, beta, C, ldc, stridec, batch_size);
    __FORCE_MKL_FLUSH__(status);
    return 0;
}
extern "C" int onemklDgemm_strided(syclQueue_t device_queue, onemklTranspose transA,
                onemklTranspose transB, int64_t m, int64_t n, int64_t k,
                double alpha, const double *A, int64_t lda, int64_t stridea,
                const double *B, int64_t ldb, int64_t strideb, double beta,
                double *C, int64_t ldc, int64_t stridec, int64_t batch_size) {
    auto status = oneapi::mkl::blas::column_major::gemm_batch(device_queue->val, convert(transA),
                                          convert(transB), m, n, k, alpha, A,
                                          lda, stridea, B, ldb, strideb, beta, C, ldc, stridec, batch_size);
    __FORCE_MKL_FLUSH__(status);
    return 0;
}
extern "C" int onemklCgemm_strided(syclQueue_t device_queue, onemklTranspose transA,
                onemklTranspose transB, int64_t m, int64_t n, int64_t k,
                float _Complex alpha, const float _Complex *A, int64_t lda, int64_t stridea,
                const float _Complex *B, int64_t ldb, int64_t strideb, float _Complex beta,
                float _Complex *C, int64_t ldc, int64_t stridec, int64_t batch_size) {
    auto status = oneapi::mkl::blas::column_major::gemm_batch(
        device_queue->val, convert(transA), convert(transB), m, n, k, alpha,
        reinterpret_cast<const std::complex<float> *>(A), lda, stridea,
        reinterpret_cast<const std::complex<float> *>(B), ldb, strideb, beta,
        reinterpret_cast<std::complex<float> *>(C), ldc, stridec, batch_size);
    __FORCE_MKL_FLUSH__(status);
    return 0;
}
extern "C" int onemklZgemm_strided(syclQueue_t device_queue, onemklTranspose transA,
                onemklTranspose transB, int64_t m, int64_t n, int64_t k,
                double _Complex alpha, const double _Complex *A, int64_t lda, int64_t stridea,
                const double _Complex *B, int64_t ldb, int64_t strideb, double _Complex beta,
                double _Complex *C, int64_t ldc, int64_t stridec, int64_t batch_size) {
    auto status = oneapi::mkl::blas::column_major::gemm_batch(
        device_queue->val, convert(transA), convert(transB), m, n, k, alpha,
        reinterpret_cast<const std::complex<double> *>(A), lda, stridea,
        reinterpret_cast<const std::complex<double> *>(B), ldb, strideb, beta,
        reinterpret_cast<std::complex<double> *>(C), ldc, stridec, batch_size);
    __FORCE_MKL_FLUSH__(status);
    return 0;
}

extern "C" void onemklCherk(syclQueue_t device_queue, onemklUplo uplo, onemklTranspose trans, int64_t n, int64_t k,
                float alpha, const float _Complex* a, int64_t lda, float beta, float _Complex* c, int64_t ldc) {
    auto status = oneapi::mkl::blas::column_major::herk(device_queue->val, convert(uplo), convert(trans), n, k,
                alpha, reinterpret_cast<const std::complex<float> *>(a), lda,
                beta, reinterpret_cast<std::complex<float> *>(c), ldc);
    __FORCE_MKL_FLUSH__(status);
}
extern "C" void onemklZherk(syclQueue_t device_queue, onemklUplo uplo, onemklTranspose trans, int64_t n, int64_t k,
                double alpha, const double _Complex* a, int64_t lda, double beta, double _Complex* c, int64_t ldc) {
    auto status = oneapi::mkl::blas::column_major::herk(device_queue->val, convert(uplo), convert(trans), n, k,
                alpha, reinterpret_cast<const std::complex<double> *>(a), lda,
                beta, reinterpret_cast<std::complex<double> *>(c), ldc);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklCher2k(syclQueue_t device_queue, onemklUplo uplo, onemklTranspose trans, int64_t n, int64_t k,
                float _Complex alpha, const float _Complex* a, int64_t lda, const float _Complex* b, int64_t ldb,
                float beta, float _Complex* c, int64_t ldc) {
    auto status = oneapi::mkl::blas::column_major::her2k(device_queue->val, convert(uplo), convert(trans), n, k,
                alpha, reinterpret_cast<const std::complex<float> *>(a), lda,
                reinterpret_cast<const std::complex<float> *>(b), ldb,
                beta, reinterpret_cast<std::complex<float> *>(c), ldc);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklZher2k(syclQueue_t device_queue, onemklUplo uplo, onemklTranspose trans, int64_t n, int64_t k,
                double _Complex alpha, const double _Complex* a, int64_t lda,  const double _Complex* b, int64_t ldb,
                double beta, double _Complex* c, int64_t ldc) {
    auto status = oneapi::mkl::blas::column_major::her2k(device_queue->val, convert(uplo), convert(trans), n, k,
                alpha, reinterpret_cast<const std::complex<double> *>(a), lda,
                reinterpret_cast<const std::complex<double> *>(b), ldb,
                beta, reinterpret_cast<std::complex<double> *>(c), ldc);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklSsymm(syclQueue_t device_queue, onemklSideMode side, onemklUplo uplo, int64_t m, int64_t n,
                float alpha, const float* a, int64_t lda, const float* b, int64_t ldb,
                float beta, float* c, int64_t ldc) {
    auto status = oneapi::mkl::blas::column_major::symm(device_queue->val, convert(side), convert(uplo), m, n,
                alpha, a, lda, b, ldb, beta, c, ldc);
    __FORCE_MKL_FLUSH__(status);
}
extern "C" void onemklDsymm(syclQueue_t device_queue, onemklSideMode side, onemklUplo uplo, int64_t m, int64_t n,
                double alpha, const double* a, int64_t lda, const double* b, int64_t ldb,
                double beta, double* c, int64_t ldc) {
    auto status = oneapi::mkl::blas::column_major::symm(device_queue->val, convert(side), convert(uplo), m, n,
                alpha, a, lda, b, ldb, beta, c, ldc);
    __FORCE_MKL_FLUSH__(status);
}
extern "C" void onemklCsymm(syclQueue_t device_queue, onemklSideMode side, onemklUplo uplo, int64_t m, int64_t n,
                float _Complex alpha, const float _Complex* a, int64_t lda, const float _Complex* b, int64_t ldb,
                float _Complex beta, float _Complex* c, int64_t ldc) {
    auto status = oneapi::mkl::blas::column_major::symm(device_queue->val, convert(side), convert(uplo), m, n,
                static_cast<std::complex<float>>(alpha), reinterpret_cast<const std::complex<float> *>(a), lda,
                reinterpret_cast<const std::complex<float> *>(b), ldb,
                static_cast<const std::complex<float>>(beta), reinterpret_cast<std::complex<float> *>(c), ldc);
    __FORCE_MKL_FLUSH__(status);
}
extern "C" void onemklZsymm(syclQueue_t device_queue, onemklSideMode side, onemklUplo uplo, int64_t m, int64_t n,
                double _Complex alpha, const double _Complex* a, int64_t lda, const double _Complex* b, int64_t ldb,
                double _Complex beta, double _Complex* c, int64_t ldc) {
    auto status = oneapi::mkl::blas::column_major::symm(device_queue->val, convert(side), convert(uplo), m, n,
                static_cast<std::complex<double>>(alpha), reinterpret_cast<const std::complex<double> *>(a), lda,
                reinterpret_cast<const std::complex<double> *>(b), ldb,
                static_cast<std::complex<double>>(beta), reinterpret_cast<std::complex<double> *>(c), ldc);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklSsyrk(syclQueue_t device_queue, onemklUplo uplo, onemklTranspose trans, int64_t n, int64_t k,
                float alpha, const float* a, int64_t lda, float beta, float* c, int64_t ldc) {
    auto status = oneapi::mkl::blas::column_major::syrk(device_queue->val, convert(uplo), convert(trans), n, k,
                alpha, a, lda, beta, c, ldc);
    __FORCE_MKL_FLUSH__(status);
}
extern "C" void onemklDsyrk(syclQueue_t device_queue, onemklUplo uplo, onemklTranspose trans, int64_t n, int64_t k,
                double alpha, const double* a, int64_t lda, double beta, double* c, int64_t ldc) {
    auto status = oneapi::mkl::blas::column_major::syrk(device_queue->val, convert(uplo), convert(trans), n, k,
                alpha, a, lda, beta, c, ldc);
    __FORCE_MKL_FLUSH__(status);
}
extern "C" void onemklCsyrk(syclQueue_t device_queue, onemklUplo uplo, onemklTranspose trans, int64_t n, int64_t k,
                float _Complex alpha, const float _Complex* a, int64_t lda, float _Complex beta, float _Complex* c, int64_t ldc) {
    auto status = oneapi::mkl::blas::column_major::syrk(device_queue->val, convert(uplo), convert(trans), n, k,
                static_cast<std::complex<float>>(alpha), reinterpret_cast<const std::complex<float> *>(a), lda,
                static_cast<std::complex<float>>(beta), reinterpret_cast<std::complex<float> *>(c), ldc);
    __FORCE_MKL_FLUSH__(status);
}
extern "C" void onemklZsyrk(syclQueue_t device_queue, onemklUplo uplo, onemklTranspose trans, int64_t n, int64_t k,
                double _Complex alpha, const double _Complex* a, int64_t lda, double _Complex beta, double _Complex* c, int64_t ldc) {
    auto status = oneapi::mkl::blas::column_major::syrk(device_queue->val, convert(uplo), convert(trans), n, k,
                static_cast<std::complex<double>>(alpha), reinterpret_cast<const std::complex<double> *>(a), lda,
                static_cast<std::complex<double>>(beta), reinterpret_cast<std::complex<double> *>(c), ldc);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklSsyr2k(syclQueue_t device_queue, onemklUplo uplo, onemklTranspose trans, int64_t n, int64_t k,
                float alpha, const float* a, int64_t lda, const float* b, int64_t ldb, float beta, float* c, int64_t ldc) {
    auto status = oneapi::mkl::blas::column_major::syr2k(device_queue->val, convert(uplo), convert(trans), n, k,
                alpha, a, lda, b, ldb, beta, c, ldc);
    __FORCE_MKL_FLUSH__(status);
}
extern "C" void onemklDsyr2k(syclQueue_t device_queue, onemklUplo uplo, onemklTranspose trans, int64_t n, int64_t k,
                double alpha, const double* a, int64_t lda, const double* b, int64_t ldb, double beta, double* c, int64_t ldc) {
    auto status = oneapi::mkl::blas::column_major::syr2k(device_queue->val, convert(uplo), convert(trans), n, k,
                alpha, a, lda, b, ldb, beta, c, ldc);
    __FORCE_MKL_FLUSH__(status);
}
extern "C" void onemklCsyr2k(syclQueue_t device_queue, onemklUplo uplo, onemklTranspose trans, int64_t n, int64_t k,
                float _Complex alpha, const float _Complex* a, int64_t lda, const float _Complex* b, int64_t ldb,
                float _Complex beta, float _Complex* c, int64_t ldc) {
    auto status = oneapi::mkl::blas::column_major::syr2k(device_queue->val, convert(uplo), convert(trans), n, k,
                static_cast<std::complex<float>>(alpha), reinterpret_cast<const std::complex<float> *>(a), lda,
                reinterpret_cast<const std::complex<float> *>(b), ldb,
                static_cast<std::complex<float>>(beta), reinterpret_cast<std::complex<float> *>(c), ldc);
    __FORCE_MKL_FLUSH__(status);
}
extern "C" void onemklZsyr2k(syclQueue_t device_queue, onemklUplo uplo, onemklTranspose trans, int64_t n, int64_t k,
                double _Complex alpha, const double _Complex* a, int64_t lda, const double _Complex* b, int64_t ldb,
                double _Complex beta, double _Complex* c, int64_t ldc) {
    auto status = oneapi::mkl::blas::column_major::syr2k(device_queue->val, convert(uplo), convert(trans), n, k,
                static_cast<std::complex<double>>(alpha), reinterpret_cast<const std::complex<double> *>(a), lda,
                reinterpret_cast<const std::complex<double> *>(b), ldb,
                static_cast<std::complex<double>>(beta), reinterpret_cast<std::complex<double> *>(c), ldc);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklChemm(syclQueue_t device_queue, onemklSideMode side, onemklUplo uplo, int64_t m, int64_t n,
                float _Complex alpha, const float _Complex* a, int64_t lda, const float _Complex* b, int64_t ldb,
                float _Complex beta, float _Complex* c, int64_t ldc) {
    auto status = oneapi::mkl::blas::column_major::hemm(device_queue->val, convert(side), convert(uplo), m, n,
                static_cast<std::complex<float>>(alpha), reinterpret_cast<const std::complex<float> *>(a), lda,
                reinterpret_cast<const std::complex<float> *>(b), ldb, static_cast<std::complex<float>>(beta),
                reinterpret_cast<std::complex<float> *>(c), ldc);
    __FORCE_MKL_FLUSH__(status);
}
extern "C" void onemklZhemm(syclQueue_t device_queue, onemklSideMode side, onemklUplo uplo, int64_t m, int64_t n,
                double _Complex alpha, const double _Complex* a, int64_t lda, const double _Complex* b, int64_t ldb,
                double _Complex beta, double _Complex* c, int64_t ldc) {
    auto status = oneapi::mkl::blas::column_major::hemm(device_queue->val, convert(side), convert(uplo), m, n,
                static_cast<std::complex<double>>(alpha), reinterpret_cast<const std::complex<double> *>(a), lda,
                reinterpret_cast<const std::complex<double> *>(b), ldb, static_cast<std::complex<double>>(beta),
                reinterpret_cast<std::complex<double> *>(c), ldc);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklStrmm(syclQueue_t device_queue, onemklSideMode side, onemklUplo uplo, onemklTranspose trans, onemklDiag diag,
                 int64_t m, int64_t n, float alpha, const float *a, int64_t lda, float *b, int64_t ldb) {
    auto status = oneapi::mkl::blas::column_major::trmm(device_queue->val, convert(side), convert(uplo), convert(trans), convert(diag),
                 m, n, alpha, a, lda, b, ldb);
    __FORCE_MKL_FLUSH__(status);
}
extern "C" void onemklDtrmm(syclQueue_t device_queue, onemklSideMode side, onemklUplo uplo, onemklTranspose trans, onemklDiag diag,
                 int64_t m, int64_t n, double alpha, const double *a, int64_t lda, double *b, int64_t ldb) {
    auto status = oneapi::mkl::blas::column_major::trmm(device_queue->val, convert(side), convert(uplo), convert(trans), convert(diag),
                 m, n, alpha, a, lda, b, ldb);
    __FORCE_MKL_FLUSH__(status);
}
extern "C" void onemklCtrmm(syclQueue_t device_queue, onemklSideMode side, onemklUplo uplo, onemklTranspose trans, onemklDiag diag,
                 int64_t m, int64_t n, float _Complex alpha, const float _Complex*a, int64_t lda, float _Complex*b, int64_t ldb) {
    auto status = oneapi::mkl::blas::column_major::trmm(device_queue->val, convert(side), convert(uplo), convert(trans), convert(diag),
                 m, n, static_cast<std::complex<float>>(alpha), reinterpret_cast<const std::complex<float> *>(a), lda,
                reinterpret_cast<std::complex<float> *>(b), ldb);
    __FORCE_MKL_FLUSH__(status);
}
extern "C" void onemklZtrmm(syclQueue_t device_queue, onemklSideMode side, onemklUplo uplo, onemklTranspose trans, onemklDiag diag,
                 int64_t m, int64_t n, double _Complex alpha, const double _Complex *a, int64_t lda, double _Complex *b, int64_t ldb) {
    auto status = oneapi::mkl::blas::column_major::trmm(device_queue->val, convert(side), convert(uplo), convert(trans), convert(diag),
                 m, n, static_cast<std::complex<double>>(alpha), reinterpret_cast<const std::complex<double> *>(a), lda,
                reinterpret_cast<std::complex<double> *>(b), ldb);
    __FORCE_MKL_FLUSH__(status);
}

extern "C" void onemklStrsm(syclQueue_t device_queue, onemklSideMode side, onemklUplo uplo, onemklTranspose trans, onemklDiag diag,
                 int64_t m, int64_t n, float alpha, const float *a, int64_t lda, float *b, int64_t ldb) {
    auto status = oneapi::mkl::blas::column_major::trsm(device_queue->val, convert(side), convert(uplo), convert(trans), convert(diag),
                 m, n, alpha, a, lda, b, ldb);
    __FORCE_MKL_FLUSH__(status);
}
extern "C" void onemklDtrsm(syclQueue_t device_queue, onemklSideMode side, onemklUplo uplo, onemklTranspose trans, onemklDiag diag,
                 int64_t m, int64_t n, double alpha, const double *a, int64_t lda, double *b, int64_t ldb) {
    auto status = oneapi::mkl::blas::column_major::trsm(device_queue->val, convert(side), convert(uplo), convert(trans), convert(diag),
                 m, n, alpha, a, lda, b, ldb);
    __FORCE_MKL_FLUSH__(status);
}
extern "C" void onemklCtrsm(syclQueue_t device_queue, onemklSideMode side, onemklUplo uplo, onemklTranspose trans, onemklDiag diag,
                 int64_t m, int64_t n, float _Complex alpha, const float _Complex*a, int64_t lda, float _Complex*b, int64_t ldb) {
    auto status = oneapi::mkl::blas::column_major::trsm(device_queue->val, convert(side), convert(uplo), convert(trans), convert(diag),
                 m, n, static_cast<std::complex<float>>(alpha), reinterpret_cast<const std::complex<float> *>(a), lda,
                reinterpret_cast<std::complex<float> *>(b), ldb);
    __FORCE_MKL_FLUSH__(status);
}
extern "C" void onemklZtrsm(syclQueue_t device_queue, onemklSideMode side, onemklUplo uplo, onemklTranspose trans, onemklDiag diag,
                 int64_t m, int64_t n, double _Complex alpha, const double _Complex *a, int64_t lda, double _Complex *b, int64_t ldb) {
    auto status = oneapi::mkl::blas::column_major::trsm(device_queue->val, convert(side), convert(uplo), convert(trans), convert(diag),
                 m, n, static_cast<std::complex<double>>(alpha), reinterpret_cast<const std::complex<double> *>(a), lda,
                reinterpret_cast<std::complex<double> *>(b), ldb);
    __FORCE_MKL_FLUSH__(status);
}

void onemklStrsm_strided(syclQueue_t device_queue, onemklSideMode side, onemklUplo uplo, onemklTranspose trans, onemklDiag diag, int64_t m,
                int64_t n, float alpha, const float *a, int64_t lda, int64_t stridea, float *b, int64_t ldb, int64_t strideb, int64_t batch_size) {
    auto status = oneapi::mkl::blas::column_major::trsm_batch(device_queue->val, convert(side), convert(uplo), convert(trans), convert(diag),
                 m, n, alpha, a, stridea, lda, b, ldb, strideb, batch_size);
    __FORCE_MKL_FLUSH__(status);
}
void onemklDtrsm_strided(syclQueue_t device_queue, onemklSideMode side, onemklUplo uplo, onemklTranspose trans, onemklDiag diag, int64_t m,
                int64_t n, double alpha, const double *a, int64_t lda, int64_t stridea, double *b, int64_t ldb, int64_t strideb, int64_t batch_size) {
    auto status = oneapi::mkl::blas::column_major::trsm_batch(device_queue->val, convert(side), convert(uplo), convert(trans), convert(diag),
                 m, n, alpha, a, stridea, lda, b, ldb, strideb, batch_size);
    __FORCE_MKL_FLUSH__(status);
}
void onemklCtrsm_strided(syclQueue_t device_queue, onemklSideMode side, onemklUplo uplo, onemklTranspose trans, onemklDiag diag, int64_t m,
                int64_t n, float _Complex alpha, const float _Complex*a, int64_t lda, int64_t stridea, float _Complex*b, int64_t ldb, int64_t strideb, int64_t batch_size) {
    auto status = oneapi::mkl::blas::column_major::trsm_batch(device_queue->val, convert(side), convert(uplo), convert(trans), convert(diag),
                 m, n, static_cast<std::complex<float>>(alpha), reinterpret_cast<const std::complex<float> *>(a), lda, stridea,
                reinterpret_cast<std::complex<float> *>(b), ldb, strideb, batch_size);
    __FORCE_MKL_FLUSH__(status);
}
void onemklZtrsm_strided(syclQueue_t device_queue, onemklSideMode side, onemklUplo uplo, onemklTranspose trans, onemklDiag diag, int64_t m,
                int64_t n, double _Complex alpha, const double _Complex *a, int64_t lda, int64_t stridea, double _Complex *b, int64_t ldb, int64_t strideb, int64_t batch_size) {
    auto status = oneapi::mkl::blas::column_major::trsm_batch(device_queue->val, convert(side), convert(uplo), convert(trans), convert(diag),
                 m, n, static_cast<std::complex<double>>(alpha), reinterpret_cast<const std::complex<double> *>(a), lda, stridea,
                reinterpret_cast<std::complex<double> *>(b), ldb, strideb, batch_size);
    __FORCE_MKL_FLUSH__(status);
}

// other

// oneMKL keeps a cache of SYCL queues and tries to destroy them when unloading the library.
// that is incompatible with oneAPI.jl destroying queues before that, so expose a function
// to manually wipe the device cache when we're destroying queues.

namespace oneapi {
namespace mkl {
namespace gpu {
int clean_gpu_caches();
}
}
}

extern "C" void onemklDestroy() {
    oneapi::mkl::gpu::clean_gpu_caches();
}
