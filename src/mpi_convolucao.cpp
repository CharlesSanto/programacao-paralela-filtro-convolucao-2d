#include <iostream>
#include <vector>
#include <cstdlib>
#include <mpi.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int N = (argc > 1) ? std::atoi(argv[1]) : 1000;
    int P = (argc > 2) ? std::atoi(argv[2]) : 30;

    int rows_per_proc = N / size;
    int local_N = rows_per_proc + 2;

    std::vector<float> local_a(local_N * N, 1.0f);
    std::vector<float> local_b(local_N * N, 0.0f);

    double start_time = MPI_Wtime();

    for (int p = 0; p < P; p++) {
        int top_neighbor = (rank == 0) ? MPI_PROC_NULL : rank - 1;
        int bottom_neighbor = (rank == size - 1) ? MPI_PROC_NULL : rank + 1;

        MPI_Sendrecv(&local_a[N], N, MPI_FLOAT, top_neighbor, 0,
                     &local_a[(local_N - 1) * N], N, MPI_FLOAT, bottom_neighbor, 0,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        MPI_Sendrecv(&local_a[(local_N - 2) * N], N, MPI_FLOAT, bottom_neighbor, 1,
                     &local_a[0], N, MPI_FLOAT, top_neighbor, 1,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        for (int y = 1; y <= rows_per_proc; y++) {
            int global_y = rank * rows_per_proc + (y - 1);
            if (global_y == 0 || global_y == N - 1) continue;

            for (int x = 1; x < N - 1; x++) {
                float s = 0.0f;
                for (int dy = -1; dy <= 1; dy++) {
                    for (int dx = -1; dx <= 1; dx++) {
                        s += local_a[(y + dy) * N + (x + dx)];
                    }
                }
                local_b[y * N + x] = s / 9.0f;
            }
        }
        local_a.swap(local_b);
    }

    double end_time = MPI_Wtime();

    double local_sum = 0.0;
    for (int y = 1; y <= rows_per_proc; y++) {
        for (int x = 0; x < N; x++) {
            local_sum += local_a[y * N + x];
        }
    }

    double global_checksum = 0.0;
    MPI_Reduce(&local_sum, &global_checksum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        std::cout << "Tempo MPI: " << (end_time - start_time) << " s | Checksum: " << global_checksum << std::endl;
    }

    MPI_Finalize();
    return 0;
}