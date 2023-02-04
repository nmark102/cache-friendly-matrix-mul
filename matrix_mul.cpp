#include <iostream>
#include <cstdlib>
#include <time.h>
#include <chrono>
#include <memory.h>

using namespace std;

// Change this size parameter to test out matrices of different sizes!
// NOTE: For the sake of simplicity, we're working entirely with square matrices here
const int SIZE = 10000;

long** A;
long** B;
long** C;

void matrix_mul(long** a, long** b, long** c, const int size) {
    for(int i = 0; i < size; ++i) {
        cout << "i = " << i << endl;
        for(int j = 0; j < size; ++j) {
            for(int k = 0; k < size; ++k) {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}

void matrix_mul_cache_friendly(long** a, long** b, long** c, const int size) {
    /*
    This is NOT a standard matrix multiplication algorithm;
    we're instead trying to maximize iterating over matrices column-by-column,
    and minimize jumps between rows. 

    Hence, as the name "cache-friendly" suggests, maximizing column-by-column
    iterations seek to maximize cache hits during memory reads, as elements on
    the same row (but different columns) are much more likely to be fetched into
    cache together than elements on different rows but the same columns.
    */
    for(int k = 0; k < size; ++k) {
        cout << "k = " << k << endl;
        for(int i = 0; i < size; ++i) {
            for(int j = 0; j < size; ++j) {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}

void init(long** a, const int size) {
    for(int i = 0; i < size; ++i) {
        for(int j = 0; j < size; ++j) {
            a[i][j] = rand() & 0xFFFF;
        }
    }
}

void reset_matrix(long** a, const int size) {
    for(int i = 0; i < size; ++i) {
        memset(a[i], 0, sizeof(long));
    }
}

int main()
{
    //---- initialize matrices A and B to random values.
    srand(time(NULL));

    A = (long**) malloc(sizeof(long*) * SIZE);
    B = (long**) malloc(sizeof(long*) * SIZE);
    C = (long**) malloc(sizeof(long*) * SIZE);

    for(int i = 0; i < SIZE; ++i) {
        A[i] = (long*) malloc(sizeof(long) * SIZE);
        B[i] = (long*) malloc(sizeof(long) * SIZE);
        C[i] = (long*) calloc(SIZE, sizeof(long));
    }

    init(A, SIZE);
    init(B, SIZE);

    
    //---- STANDARD MATRIX MULTIPLICATION ALGORITHM ----
    
    //---- start time
    auto start_time = chrono::high_resolution_clock::now();

    // run the standard matrix multiplication implementation
    matrix_mul(A, B, C, SIZE);

    //---- finish time
    auto finish_time = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = finish_time - start_time;
    cout << "Elapsed time (standard matrix multiplication): " << elapsed.count() << " s\n";

    // reset the resulting matrix
    reset_matrix(C, SIZE);

    //---- CACHE-FRIENDLY MATRIX MULTIPLICATION ALGORITHM ----
    start_time = chrono::high_resolution_clock::now();

    // run the "cache-friendly" matrix multiplication algorithm
    matrix_mul_cache_friendly(A, B, C, SIZE);

    //---- finish time
    finish_time = chrono::high_resolution_clock::now();
    elapsed = finish_time - start_time;
    cout << "Elapsed time (cache-friendly matrix multiplication): " << elapsed.count() << " s\n";
    
    //---- deallocate memory for matrices
    for(int i = 0; i < SIZE; ++i) {
        free(A[i]);
        free(B[i]);
        free(C[i]);
    }

    free(A);
    free(B);
    free(C);

    
    return 0;
}

