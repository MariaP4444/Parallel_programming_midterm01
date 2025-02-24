#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <omp.h>

// Variables globales para compartir resultados
int tiempoA = 0, tiempoB = 0, tiempoC = 0, resultadoD = 0;

// Tiempos de inicio para medir esperas
double inicioBC;  // Tiempo de inicio para B y C
double inicioGeneral; // Tiempo de inicio general

// Función A: Espera un tiempo aleatorio y retorna ese tiempo
void funcionA() {
    int espera = rand() % 4 + 1;
    sleep(espera);
    tiempoA = espera;
    printf("A: Esperó %d segundos\n", tiempoA);
}

// Función B: Espera un tiempo aleatorio y retorna ese tiempo
void funcionB() {
    int espera = rand() % 5 + 1;
    sleep(espera);
    tiempoB = espera;
    printf("B: Esperó %d segundos\n", tiempoB);
}

// Función C: Espera un tiempo aleatorio y retorna ese tiempo
void funcionC() {
    int espera = rand() % 3 + 1;
    sleep(espera);
    tiempoC = espera;
    printf("C: Esperó %d segundos\n", tiempoC);
}

// Función D: Se ejecuta después de B y C
void funcionD() {
    // Calcula cuánto tiempo esperó D
    double finD = omp_get_wtime();
    int tiempoEsperaD = (int)(finD - inicioBC);  // El tiempo de espera de D es el tiempo desde que empezaron B y C
    resultadoD = (tiempoB + tiempoC) * 4;
    printf("D: Esperó %d segundos. Resultado = (B + C) * 4 = %d\n", tiempoEsperaD, resultadoD);
}

// Función E: Se ejecuta después de A y D
void funcionE() {
    // Calcula cuánto tiempo esperó E
    double finE = omp_get_wtime();
    int tiempoEsperaE = (int)(finE - inicioGeneral);  // El tiempo de espera de E es el tiempo desde el inicio general

    if (tiempoA == resultadoD) {
        printf("E: Esperó %d segundos. Tiempo A (%d) es igual al resultado de D (%d)\n", tiempoEsperaE, tiempoA, resultadoD);
    } else {
        printf("E: Esperó %d segundos. Tiempo A (%d) NO es igual al resultado de D (%d)\n", tiempoEsperaE, tiempoA, resultadoD);
    }
}

int main() {
    srand(time(NULL));

    // Iniciar los temporizadores para medir esperas
    inicioGeneral = omp_get_wtime();
    inicioBC = inicioGeneral;

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

    // Ejecutar D después de B y C
    funcionD();

    // Ejecutar E después de A y D
    funcionE();

    printf("Fin de la ejecución\n");
    return 0;
}

