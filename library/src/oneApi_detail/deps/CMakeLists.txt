set(CMAKE_CXX_COMPILER ${ICPX_EXECUTABLE})
set(CMAKE_CXX_FLAGS "-Wno-deprecated-declarations")
set(POSITION_INDEPENDENT_CODE ON)

add_library(sycl_wrapper SHARED ./sycl_wrapper.cpp ./onemkl.cpp)
target_include_directories(sycl_wrapper PUBLIC ${PROJECT_SOURCE_DIR}/level_zero)
set_target_properties(sycl_wrapper PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)
target_link_libraries(sycl_wrapper PUBLIC MKL::MKL MKL::mkl_intel_ilp64 MKL::mkl_sequential MKL::mkl_core)
target_include_directories(sycl_wrapper
                            PUBLIC  $<BUILD_INTERFACE:~/packages>
                            PRIVATE ${PROJECT_BINARY_DIR}/include/hipblas
                            PRIVATE $<BUILD_INTERFACE:${HIP_INCLUDE_DIRS}>
                            PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/.. "$<TARGET_PROPERTY:MKL::MKL,INTERFACE_INCLUDE_DIRECTORIES>")
target_compile_options(sycl_wrapper PRIVATE "$<TARGET_PROPERTY:MKL::MKL,INTERFACE_COMPILE_OPTIONS>")

# Install sycl_wrapper lib
install(TARGETS sycl_wrapper DESTINATION ${CMAKE_INSTALL_LIBDIR})