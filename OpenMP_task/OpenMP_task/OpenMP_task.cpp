#include <iostream>
#include <fstream>
#include <cstdlib>
#include <omp.h>
#include <chrono>
#include <thread>

using namespace std;

int main(int argc, char** argv) {

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

    for (int i = 0; i < matrixSize; i++) {
        for (int j = 0; j < matrixSize; j++) {
            a[i][j] = rand() % 100;
            b[i][j] = rand() % 100;
        }
    }

    srand(time(NULL));

    ofstream outfile("results.csv", ios_base::app);
    if (!outfile.is_open()) {
        cerr << "Error opening file." << endl;
        return 1;
    }

    outfile << "Threads,Time\n";

    cout << "Calculation performing" << endl;
    cout << endl;

    int maxThreads = 4;

    for (int t = 1; t <= maxThreads; t++)
    {
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
        printf("Thread: %d; Time: %d seconds\n", t, elapsed_time / 1000);

        outfile << t << "," << elapsed_time / 1000 << "\n";

        this_thread::sleep_for(chrono::seconds(1));
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

    return 0;
}