#include <omp.h>
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <ctime>

using namespace std;

// Variables globales para compartir resultados
int tiempoA = 0, tiempoB = 0, tiempoC = 0, resultadoD = 0, resultadoF = 0;

void funcionA() {
    int espera = rand() % 4 + 1;
    sleep(espera);
    tiempoA = espera;
    #pragma omp critical
    cout << "A: Esperó " << tiempoA << " segundos" << endl;
}

void funcionB() {
    int espera = rand() % 5 + 1;
    sleep(espera);
    tiempoB = espera;
    #pragma omp critical
    cout << "B: Esperó " << tiempoB << " segundos" << endl;
}

void funcionC() {
    int espera = rand() % 3 + 1;
    sleep(espera);
    tiempoC = espera;
    #pragma omp critical
    cout << "C: Esperó " << tiempoC << " segundos" << endl;
}

void funcionD() {
    resultadoD = (tiempoB + tiempoC) * 2;
    #pragma omp critical
    cout << "D: Resultado = (B + C) * 2 = " << resultadoD << endl;
}

void funcionF() {
    resultadoF = tiempoB + tiempoA + 3;
    #pragma omp critical
    cout << "F: Resultado = B + A + 3 = " << resultadoF << endl;
}

void funcionE() {
    #pragma omp critical
    {
        if (resultadoD == resultadoF) {
            cout << "E: Resultado D (" << resultadoD << ") es igual a Resultado F (" << resultadoF << ")" << endl;
        } else {
            cout << "E: Resultado D (" << resultadoD << ") NO es igual a Resultado F (" << resultadoF << ")" << endl;
        }
    }
}

int main() {
    srand(time(NULL));

    // Ejecutar A, B y C en paralelo
    #pragma omp parallel sections
    {
        #pragma omp section
        {
            funcionA();
        }
        #pragma omp section
        {
            funcionB();
        }
        #pragma omp section
        {
            funcionC();
        }
    }

    // Ejecutar D (depende de B y C) y F (depende de A y B) en paralelo
    #pragma omp parallel sections
    {
        #pragma omp section
        {
            funcionD();
        }
        #pragma omp section
        {
            funcionF();
        }
    }

    // Ejecutar E (depende de D y F)
    funcionE();

    cout << "Fin de la ejecución" << endl;
    return 0;
}

