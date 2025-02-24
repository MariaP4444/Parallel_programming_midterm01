#include <mpi.h>
#include <iostream>
#include <vector>
#include <iomanip>
#include <random>
using namespace std;

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distribucion(1, 100);

    int mensaje = distribucion(gen);
    const int numIteraciones = 15;
    vector<double> tiemposRTT;

    for (int i = 0; i < numIteraciones; i++) {
        double tiempoInicio, tiempoFin, rtt;

        if (rank == 0) {
            cout << "Iteración " << i << " - Nodo 0: Enviando mensaje: " << mensaje << endl;
            tiempoInicio = MPI_Wtime();

            MPI_Send(&mensaje, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
            MPI_Recv(&mensaje, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            tiempoFin = MPI_Wtime();
            rtt = (tiempoFin - tiempoInicio) * 1e6;

            cout << "Iteración " << i << " - Nodo 0: Mensaje recibido: " << mensaje << endl;
            cout << "RTT para esta iteración: " << rtt << " μs\n" << endl;
        }
        else {
            int datoRecibido;

            MPI_Recv(&datoRecibido, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            cout << "Iteración " << i << " - Nodo 1: Mensaje recibido: " << datoRecibido << endl;

            datoRecibido += 1;
            cout << "Iteración " << i << " - Nodo 1: Enviando de vuelta: " << datoRecibido << endl;
            MPI_Send(&datoRecibido, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }

        tiemposRTT.push_back(rtt);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    cout << "\n----------------------------------------\n";
    cout << "  Nodo " << rank << " - RTT por iteración (μs)\n";
    cout << "----------------------------------------\n";
    cout << "   Iteración   |    RTT (microsegundos) \n";
    cout << "----------------------------------------\n";
    for (int i = 0; i < numIteraciones; i++) {
        cout << setw(10) << i << "   |   " << setw(10) << tiemposRTT[i] << "\n";
    }
    cout << "----------------------------------------\n";

    MPI_Finalize();
    return 0;
}

