#include <iostream>
#include <pthread.h>
#include <vector>
using namespace std;

// Define the maximum size for matrices
const int MAX = 100;

// Matrices
int matA[MAX][MAX], matB[MAX][MAX], matC[MAX][MAX];
int rowA, colA, rowB, colB;

// Structure to pass data to threads
struct ThreadData {
    int row;  // Row index of the result matrix
    int col;  // Column index of the result matrix
};

// Thread function to compute one element of the result matrix
void* multiply(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    int row = data->row;
    int col = data->col;
    matC[row][col] = 0;

    for (int k = 0; k < colA; k++) {
        matC[row][col] += matA[row][k] * matB[k][col];
    }
    pthread_exit(0);
}

int main() {
    // Input dimensions of matrix A
    cout << "Enter rows and columns for Matrix A: ";
    cin >> rowA >> colA;

    // Input dimensions of matrix B
    cout << "Enter rows and columns for Matrix B: ";
    cin >> rowB >> colB;

    // Check if multiplication is possible
    if (colA != rowB) {
        cout << "Matrix multiplication not possible: Column of A != Row of B.\n";
        return 0;
    }

    // Input Matrix A
    cout << "Enter elements of Matrix A:\n";
    for (int i = 0; i < rowA; i++) {
        for (int j = 0; j < colA; j++) {
            cin >> matA[i][j];
        }
    }

    // Input Matrix B
    cout << "Enter elements of Matrix B:\n";
    for (int i = 0; i < rowB; i++) {
        for (int j = 0; j < colB; j++) {
            cin >> matB[i][j];
        }
    }

    // Create threads
    pthread_t threads[MAX * MAX];
    ThreadData threadData[MAX * MAX];
    int threadCount = 0;

    for (int i = 0; i < rowA; i++) {
        for (int j = 0; j < colB; j++) {
            threadData[threadCount] = {i, j};
            pthread_create(&threads[threadCount], NULL, multiply, &threadData[threadCount]);
            threadCount++;
        }
    }

    // Wait for all threads to complete
    for (int i = 0; i < threadCount; i++) {
        pthread_join(threads[i], NULL);
    }

    // Print the result matrix
    cout << "Resultant Matrix (A x B):\n";
    for (int i = 0; i < rowA; i++) {
        for (int j = 0; j < colB; j++) {
            cout << matC[i][j] << " ";
        }
        cout << "\n";
    }

    return 0;
}
