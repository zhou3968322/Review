#include <iostream>
#include <stdlib.h>
#include <cuda_runtime.h>
#include <cublas_v2.h>




int main() {
    const int M = 1024;
    const int N = 1024;
    const int K = 1024;

    // Allocate and initialize host memory
    float *h_A = (float*)malloc(M * K * sizeof(float));
    float *h_B = (float*)malloc(K * N * sizeof(float));
    float *h_C_cublas = (float*)malloc(M * N * sizeof(float));
    float *h_C_custom = (float*)malloc(M * N * sizeof(float));

    // Initialize matrices A and B
    for (int i = 0; i < M * K; i++) h_A[i] = static_cast<float>(rand()) / RAND_MAX;
    for (int i = 0; i < K * N; i++) h_B[i] = static_cast<float>(rand()) / RAND_MAX;

    // Allocate device memory
    float *d_A, *d_B, *d_C_cublas, *d_C_custom;
    cudaMalloc(&d_A, M * K * sizeof(float));
    cudaMalloc(&d_B, K * N * sizeof(float));
    cudaMalloc(&d_C_cublas, M * N * sizeof(float));
    cudaMalloc(&d_C_custom, M * N * sizeof(float));

    // Copy data to device
    cudaMemcpy(d_A, h_A, M * K * sizeof(float), cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, h_B, K * N * sizeof(float), cudaMemcpyHostToDevice);

    // cuBLAS handle
    cublasHandle_t handle;
    cublasCreate(&handle);

    // Timing variables
    cudaEvent_t start, stop;
    float milliseconds = 0;

    // cuBLAS GEMM timing
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
    cudaEventRecord(start);

    cublas_gemm(handle, d_A, d_B, d_C_cublas, M, N, K);

    cudaEventRecord(stop);
    cudaEventSynchronize(stop);
    cudaEventElapsedTime(&milliseconds, start, stop);
    std::cout << "cuBLAS GEMM Time: " << milliseconds << " ms" << std::endl;

    // Custom GEMM timing
    cudaEventRecord(start);

    custom_gemm(d_A, d_B, d_C_custom, M, N, K);

    cudaEventRecord(stop);
    cudaEventSynchronize(stop);
    cudaEventElapsedTime(&milliseconds, start, stop);
    std::cout << "Custom GEMM Time: " << milliseconds << " ms" << std::endl;

    // Cleanup
    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C_cublas);
    cudaFree(d_C_custom);
    free(h_A);
    free(h_B);
    free(h_C_cublas);
    free(h_C_custom);

    cublasDestroy(handle);
    return 0;