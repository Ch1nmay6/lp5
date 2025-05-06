#include <iostream>  
#include <cuda_runtime.h>  
  
#define N 1024 // Size of the matrix (NxN)  
  
__global__ void matrixMul(int *A, int *B, int *C, int n) {  
    int row = blockIdx.y * blockDim.y + threadIdx.y;  
    int col = blockIdx.x * blockDim.x + threadIdx.x;  
  
    if (row < n && col < n) {  
        int sum = 0;  
        for (int k = 0; k < n; k++) {  
            sum += A[row * n + k] * B[k * n + col];  
        }  
        C[row * n + col] = sum;  
    }  
}  
  
int main() {  
    int *A, *B, *C;       // Host matrices  
    int *d_A, *d_B, *d_C; // Device matrices  
  
    size_t size = N * N * sizeof(int);  
  
    // Allocate memory on the host  
    A = (int*)malloc(size);  
    B = (int*)malloc(size);  
    C = (int*)malloc(size);  
  
    // Allocate memory on the device  
    cudaMalloc(&d_A, size);  
    cudaMalloc(&d_B, size);  
    cudaMalloc(&d_C, size);  
  
    // Initialize matrices with random values  
    for (int i = 0; i < N; i++) {  
        for (int j = 0; j < N; j++) {  
            A[i * N + j] = rand() % 10;  
            B[i * N + j] = rand() % 10;  
        }  
    }  
  
    // Copy matrices from host to device  
    cudaMemcpy(d_A, A, size, cudaMemcpyHostToDevice);  
    cudaMemcpy(d_B, B, size, cudaMemcpyHostToDevice);  
  
    // Define block and grid sizes  
    dim3 threadsPerBlock(16, 16);  
    dim3 numBlocks((N + threadsPerBlock.x - 1) / threadsPerBlock.x,  
                   (N + threadsPerBlock.y - 1) / threadsPerBlock.y);  
  
    // Launch kernel for matrix multiplication  
    matrixMul<<<numBlocks, threadsPerBlock>>>(d_A, d_B, d_C, N);  
  
    // Copy result back to host  
    cudaMemcpy(C, d_C, size, cudaMemcpyDeviceToHost);  
  
    // Display input matrices A and B  
    std::cout << "First 5x5 elements of Matrix A (Input):" << std::endl;  
    for (int i = 0; i < 5; i++) {  
        for (int j = 0; j < 5; j++) {  
            std::cout << A[i * N + j] << " ";  
        }  
        std::cout << std::endl;  
    }  
  
    std::cout << "\nFirst 5x5 elements of Matrix B (Input):" << std::endl;  
    for (int i = 0; i < 5; i++) {  
        for (int j = 0; j < 5; j++) {  
            std::cout << B[i * N + j] << " ";  
        }  
        std::cout << std::endl;  
    }  
  
    // Display the resulting matrix C (First 5x5)  
    std::cout << "\nFirst 5x5 elements of Matrix C (Output):" << std::endl;  
    for (int i = 0; i < 5; i++) {  
        for (int j = 0; j < 5; j++) {  
            std::cout << C[i * N + j] << " ";  
        }  
        std::cout << std::endl;  
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