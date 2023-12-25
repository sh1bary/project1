#include <iostream>
#include <fstream>
#include <cstdlib>
#include <mpi.h>
#include <chrono>
#include <omp.h>
#include <thread>

using namespace std;

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int matrixSize = 1000;
    int** a, ** b, ** c;

    a = new int* [matrixSize];
    b = new int* [matrixSize];
    c = new int* [matrixSize];

    for (int i = 0; i < matrixSize; ++i) {
        a[i] = new int[matrixSize];
        b[i] = new int[matrixSize];
        c[i] = new int[matrixSize];
    }

    cout << "Creating matrix values" << endl;
    cout << endl;

    if (rank == 0) {
        for (int i = 0; i < matrixSize; i++) {
            for (int j = 0; j < matrixSize; j++) {
                a[i][j] = rand() % 100;
                b[i][j] = rand() % 100;
            }
        }
    }

    MPI_Bcast(&a[0][0], matrixSize * matrixSize, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&b[0][0], matrixSize * matrixSize, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);

    ofstream outfile("results_mpi.csv", ios_base::app);
    if (!outfile.is_open()) {
        cerr << "Error opening file." << endl;
        MPI_Abort(MPI_COMM_WORLD, 1);
        return 1;
    }

    outfile << "Process,Threads,Time\n";

    int maxThreads = 4;

    for (int t = 1; t <= maxThreads; t++) {
        omp_set_num_threads(t);
        int i, j, k;
        unsigned int start = clock();

#pragma omp parallel for private(i, j, k)
        for (i = 0; i < matrixSize; i++) {
            for (j = 0; j < matrixSize; j++) {
                for (k = 0; k < matrixSize; k++) {
                    c[i][j] += (a[i][k] * b[k][j]);
                }
            }
        }

        unsigned int elapsed_time = clock() - start;
        printf("Process: %d; Threads: %d; Time: %d seconds\n", rank, t, elapsed_time / 1000);

        outfile << rank << "," << t << "," << elapsed_time / 1000 << "\n";

        this_thread::sleep_for(chrono::seconds(1));

        MPI_Barrier(MPI_COMM_WORLD);
    }

    outfile.close();

    for (int i = 0; i < matrixSize; ++i) {
        delete[] a[i];
        delete[] b[i];
        delete[] c[i];
    }

    delete[] a;
    delete[] b;
    delete[] c;

    MPI_Finalize();
    return 0;
}
