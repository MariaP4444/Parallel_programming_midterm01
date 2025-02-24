#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>   

// Variables globales para compartir resultados
int tiempoA = 0, tiempoB = 0, tiempoC = 0, resultadoD = 0, resultadoF = 0;
pthread_t hiloA, hiloB, hiloC, hiloD, hiloE, hiloF;

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
    pthread_join(hiloB, NULL);  // Espera a que B termine
    pthread_join(hiloC, NULL);  // Espera a que C termine
    resultadoD = (tiempoB + tiempoC) * 2;
    printf("D: Resultado = (B + C) * 2 = %d\n", resultadoD);
}

// Función F: Se ejecuta después de B y A
void funcionF() {
    pthread_join(hiloA, NULL);  // Espera a que A termine
    pthread_join(hiloB, NULL);  // Espera a que B termine
    resultadoF = tiempoB + tiempoA + 3;
    printf("F: Resultado = B + A + 3 = %d\n", resultadoF);
}

// Función E: Se ejecuta después de D y F
void funcionE() {
    pthread_join(hiloD, NULL);  // Espera a que D termine
    pthread_join(hiloF, NULL);  // Espera a que F termine

    if (resultadoD == resultadoF) {
        printf("E: Resultado D (%d) es igual a Resultado F (%d)\n", resultadoD, resultadoF);
    } else {
        printf("E: Resultado D (%d) NO es igual a Resultado F (%d)\n", resultadoD, resultadoF);
    }
}

// Wrappers para hilos
void *ejecutarA(void *arg) {
    funcionA();
    pthread_exit(NULL);
}

void *ejecutarB(void *arg) {
    funcionB();
    pthread_exit(NULL);
}

void *ejecutarC(void *arg) {
    funcionC();
    pthread_exit(NULL);
}

void *ejecutarD(void *arg) {
    funcionD();
    pthread_exit(NULL);
}

void *ejecutarF(void *arg) {
    funcionF();
    pthread_exit(NULL);
}

void *ejecutarE(void *arg) {
    funcionE();
    pthread_exit(NULL);
}

#include <time.h>   // Añadir esta librería

int main() {
    struct timespec inicio, fin;
    double tiempoTotal;
    clock_gettime(CLOCK_MONOTONIC, &inicio);
    srand(time(NULL));

    // Ejecutar A, B y C en paralelo
    pthread_create(&hiloA, NULL, ejecutarA, NULL);
    pthread_create(&hiloB, NULL, ejecutarB, NULL);
    pthread_create(&hiloC, NULL, ejecutarC, NULL);

    // Ejecutar D (depende de B y C) y F (depende de B y A) en paralelo
    pthread_create(&hiloD, NULL, ejecutarD, NULL);
    pthread_create(&hiloF, NULL, ejecutarF, NULL);

    // Ejecutar E (depende de D y F)
    pthread_create(&hiloE, NULL, ejecutarE, NULL);
    pthread_join(hiloE, NULL);
    // funcionE();
    clock_gettime(CLOCK_MONOTONIC, &fin);

    tiempoTotal = (fin.tv_sec - inicio.tv_sec) + (fin.tv_nsec - inicio.tv_nsec) / 1e9;
    printf("Tiempo de ejecución: %f segundos\n", tiempoTotal);

    printf("Fin de la ejecución\n");
    return 0;
}


