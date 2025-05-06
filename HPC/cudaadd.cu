#include <iostream>  
#include <cuda_runtime.h>  
  
#define N 1000000 // Size of the vectors  
  
// Kernel function for vector addition  
__global__ void vectorAdd(int *A, int *B, int *C, int n) {  
    int idx = threadIdx.x + blockIdx.x * blockDim.x;  
    if (idx < n) {  
        C[idx] = A[idx] + B[idx];  
    }  
}  
  
int main() {  
    int *A, *B, *C;       // Host vectors  
    int *d_A, *d_B, *d_C; // Device vectors  
  
    size_t size = N * sizeof(int);  
  
    // Allocate memory on the host  
    A = (int*)malloc(size);  
    B = (int*)malloc(size);  
    C = (int*)malloc(size);  
  
    // Allocate memory on the device  
    cudaMalloc(&d_A, size);  
    cudaMalloc(&d_B, size);  
    cudaMalloc(&d_C, size);  
  
    // Initialize host vectors with random values  
    for (int i = 0; i < N; i++) {  
        A[i] = rand() % 100;  
        B[i] = rand() % 100;  
    }  
  
    // Copy data from host to device  
    cudaMemcpy(d_A, A, size, cudaMemcpyHostToDevice);  
    cudaMemcpy(d_B, B, size, cudaMemcpyHostToDevice);  
  
    // Define block and grid sizes  
    int blockSize = 256;  // Number of threads per block  
    int gridSize = (N + blockSize - 1) / blockSize; // Calculate number of blocks  
  
    // Launch kernel for vector addition  
    vectorAdd<<<gridSize, blockSize>>>(d_A, d_B, d_C, N);  
  
    // Copy result back to host  
    cudaMemcpy(C, d_C, size, cudaMemcpyDeviceToHost);  
  
    // Display the input vectors (A and B) and the result vector (C)  
    std::cout << "First 10 elements of Vector A (Input):" << std::endl;  
    for (int i = 0; i < 10; i++) {  
        std::cout << "A[" << i << "] = " << A[i] << std::endl;  
    }  
  
    std::cout << "\nFirst 10 elements of Vector B (Input):" << std::endl;  
    for (int i = 0; i < 10; i++) {  
        std::cout << "B[" << i << "] = " << B[i] << std::endl;  
    }  
  
    std::cout << "\nFirst 10 elements of Vector C (Output):" << std::endl;  
    for (int i = 0; i < 10; i++) {  
        std::cout << "C[" << i << "] = " << C[i] << std::endl;  
    }  
  
    // Free memory  
    free(A);  
    free(B);  
    free(C);  
    cudaFree(d_A);  
    cudaFree(d_B);  
    cudaFree(d_C);  
  
    return 0;  
} 