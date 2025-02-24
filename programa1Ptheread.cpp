#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>   

// Variables globales para compartir resultados
int tiempoA = 0, tiempoB = 0, tiempoC = 0, resultadoD = 0;
pthread_t hiloA, hiloB, hiloC, hiloD, hiloE;

// Tiempos de inicio para medir esperas
time_t inicioBC;  // Tiempo de inicio para B y C
time_t inicioGeneral; // Tiempo de inicio general

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
    pthread_join(hiloB, NULL);
    pthread_join(hiloC, NULL);

    // Calcula cuánto tiempo esperó D
    time_t finD = time(NULL); 
    int tiempoEsperaD = (int)(finD - inicioBC);  // El tiempo de espera de D es el tiempo desde que empezaron B y C
    resultadoD = (tiempoB + tiempoC) * 4;
    printf("D: Esperó %d segundos. Resultado = (B + C) * 4 = %d\n", tiempoEsperaD, resultadoD);
}

// Función E: Se ejecuta después de A y D
void funcionE() {
    pthread_join(hiloA, NULL);
    // pthread_join(hiloD, NULL);

    // Calcula cuánto tiempo esperó E
    time_t finE = time(NULL);
    int tiempoEsperaE = (int)(finE - inicioGeneral);  // El tiempo de espera de E es el tiempo desde el inicio general

    if (tiempoA == resultadoD) {
        printf("E: Esperó %d segundos. Tiempo A (%d) es igual al resultado de D (%d)\n", tiempoEsperaE, tiempoA, resultadoD);
    } else {
        printf("E: Esperó %d segundos. Tiempo A (%d) NO es igual al resultado de D (%d)\n", tiempoEsperaE, tiempoA, resultadoD);
    }
}

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

// void *ejecutarD(void *arg) {
//     funcionD();
//     pthread_exit(NULL);
// }

// void *ejecutarE(void *arg) {
//     funcionE();
//     pthread_exit(NULL);
// }

int main() {
    srand(time(NULL));

    // Iniciar los temporizadores para medir esperas
    inicioGeneral = time(NULL);
    inicioBC = inicioGeneral;

    // Ejecutar A, B y C en paralelo
    pthread_create(&hiloA, NULL, ejecutarA, NULL);
    pthread_create(&hiloB, NULL, ejecutarB, NULL);
    pthread_create(&hiloC, NULL, ejecutarC, NULL);

    // Ejecutar D y E en hilos
    // pthread_create(&hiloD, NULL, ejecutarD, NULL);
    // pthread_create(&hiloE, NULL, ejecutarE, NULL);
    funcionD();
    funcionE();
    // // Esperar a que termine E
    // pthread_join(hiloE, NULL);

    printf("Fin de la ejecución\n");
    return 0;
}

