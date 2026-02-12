#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <mpi.h>

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int N = 50000;
    if (argc > 1)
        N = atoi(argv[1]);

    if (N <= 0)
    {
        if (rank == 0)
            printf("Error: size must be > 0\n");
        MPI_Finalize();
        return 1;
    }

    if (N % size != 0)
    {
        if (rank == 0)
            printf("Error: size must be divisible by %d\n", size);
        MPI_Finalize();
        return 1;
    }

    int local_n = N / size;
    int max_iter = 100;

    double *A = malloc(local_n * N * sizeof(double));
    double *b = malloc(local_n * sizeof(double));
    double *x = malloc(N * sizeof(double));
    double *x_new = malloc(N * sizeof(double));
    double *local_x_new = malloc(local_n * sizeof(double));

    if (A == NULL || b == NULL || x == NULL || x_new == NULL || local_x_new == NULL)
    {
        printf("Process %d: out of memory\n", rank);
        MPI_Finalize();
        return 1;
    }

    srand(time(NULL) + rank);

    for (int i = 0; i < local_n; i++)
    {
        int global_i = rank * local_n + i;
        double sum = 0.0;

        for (int j = 0; j < N; j++)
        {
            if (j != global_i)
            {
                A[i * N + j] = (rand() % 2000 - 1000) / 100.0;
                sum += fabs(A[i * N + j]);
            }
        }

        A[i * N + global_i] = sum * (1.001 + (rand() % 10) / 1000.0);
        b[i] = (double)(rand() % 1000) / 10.0;
    }

    if (rank == 0)
    {
        for (int i = 0; i < N; i++)
        {
            x[i] = (double)(rand() % 10000) / 100.0;
        }
    }

    MPI_Bcast(x, N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    double start_time = 0;
    if (rank == 0)
    {
        start_time = MPI_Wtime();
    }

    int iter;

    for (iter = 0; iter < max_iter; iter++)
    {
        for (int i = 0; i < local_n; i++)
        {
            int global_i = rank * local_n + i;
            double sum = 0.0;

            for (int j = 0; j < N; j++)
            {
                if (j != global_i)
                {
                    sum += A[i * N + j] * x[j];
                }
            }

            local_x_new[i] = (b[i] - sum) / A[i * N + global_i];
        }

        MPI_Allgather(local_x_new, local_n, MPI_DOUBLE,
                      x_new, local_n, MPI_DOUBLE, MPI_COMM_WORLD);

        double local_diff = 0.0;
        for (int i = rank * local_n; i < (rank + 1) * local_n; i++)
        {
            local_diff += fabs(x_new[i] - x[i]);
        }

        double global_diff;
        MPI_Allreduce(&local_diff, &global_diff, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
        global_diff /= N;

        for (int i = 0; i < N; i++)
        {
            x[i] = x_new[i];
        }
    }

    double end_time = 0;
    if (rank == 0)
    {
        end_time = MPI_Wtime();
        double total_time = end_time - start_time;
        printf("JACOBI: n=%d, processes=%d, time=%.4f sec, iter=%d\n",
               N, size, total_time, max_iter);
    }

    free(A);
    free(b);
    free(x);
    free(x_new);
    free(local_x_new);

    MPI_Finalize();
    return 0;
}