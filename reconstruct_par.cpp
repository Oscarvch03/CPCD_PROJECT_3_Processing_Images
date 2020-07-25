// INCLUIR LIBRERIAS NECESARIAS ////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


#include <sys/time.h>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <random>
#include <mpi.h>

#include "pgmio.hpp"

using namespace std;


// DEFINICION DE FUNCIONES NECESARIAS //////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


// Imprimir Matriz
void display(float * M, int m, int n, int rank){
    cout << "rank = " << rank << endl;
    for(int i = 0; i < m * n; i++){
        cout << " " << M[i];
        if((i+1) % n == 0){
            cout << endl;
        }
    }
    cout << endl;
}


// Obtener Tiempo (Actual)
double gettime(){
    return MPI_Wtime();
}


// Copiar una Matriz a otra
void deep_copy(float * M1, float * M2, int m, int n){ // REVISAR n & m
    for(int i = 0; i < m * n; i++){
        M2[i] = M1[i];
    }
}


// Inicializar Matriz con todos sus valores en Color Blanco
void init_M(float * M, int m, int n){
    for(int i = 0; i < m * n; i++){
        M[i] = 255;
    }
}


// Copiar buff a edge
void buff2edge(float * buff, float * edge, int m, int n){
    for(int i = 0; i < m; i++){
        for(int j = 0; j < n; j++){
            edge[(i * (n + 2)) + (j + 1)] = buff[(i * n) + j];
        }
    }
}


// Devolver edge a buff
void edge2buff(float * edge, float * buff, int m, int n){
    for(int i = 0; i < m; i++){
        for(int j = 0; j < n; j++){
            buff[(i * n) + j] = edge[(i * (n + 2)) + (j + 1)];
        }
    }
}

// BLOQUE PRINCIPAL DE INSTRUCCIONES ///////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


int main(int argc, char ** argv){

    MPI_Init(&argc, &argv);

    int rank, np;
    MPI_Comm comm = MPI_COMM_WORLD;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &np);

    // Imagenes para procesar //////////////////////////////////////////////////
    int I = 5;

    string fileA = "edge192x128.pgm";
    string outA = "edge192x128_re_par.pgm";

    string fileB = "edge256x192.pgm";
    string outB = "edge256x192_re_par.pgm";

    string fileC = "edge512x384.pgm";
    string outC = "edge512x384_re_par.pgm";

    string fileD = "edge768x768.pgm";
    string outD = "edge768x768_re_par.pgm";

    string fileE  = "edge4896x3264.pgm";
    string outE = "edge4896x3264_re_par.pgm";
    ////////////////////////////////////////////////////////////////////////////

    string filenames[I] = {fileA, fileB, fileC, fileD, fileE};
    string outfiles[I] = {outA, outB, outC, outD, outE};

    int N = atoi(argv[1]); // # de Iteraciones

    int Nimg = atoi(argv[2]); // # de Imagen que se desea procesar
    string filename = filenames[Nimg - 1];
    string outfile = outfiles[Nimg - 1];

    int m; // # de filas
    int n; // # de columnas
    pgmsize(filename, m, n);

    if(rank == 0){
        printf("\n");
        cout << "m: " << m << " n: " << n << endl;
    }

    float * buff;
    float * edge;

    float * old_b;
    float * new_b;

    float * tmp;

    int rows = m / np;
    int local_size = (n + 2) * rows;
    int izq_sz = n + 2;
    int der_sz = n + 2;
    int local_size_b = izq_sz + local_size + der_sz;

    old_b = new float[local_size_b];
    new_b = new float[local_size_b];

    init_M(old_b, rows + 2, n + 2);
    init_M(new_b, rows + 2, n + 2);

    if(rank == 0){
        buff = new float[m * n];

        // Solo agrego borde izq y der
        edge = new float[m * (n + 2)];

        // Lleno los bordes izq y der
        for(int i = 0; i < m; i++){
            edge[i * (n + 2) + 0] = 255;
            edge[i * (n + 2) + (n + 1)] = 255;
        }

        pgmread(filename, buff, m, n);

        // Copiar buff a edge
        buff2edge(buff, edge, m, n);
    }


    tmp = new float[local_size_b];
    deep_copy(old_b, tmp, rows + 2, n + 2);

    MPI_Scatter(edge, local_size, MPI_FLOAT,
                &tmp[n + 2], local_size, MPI_FLOAT, 0, comm);

    // Ejecutar N veces
    MPI_Status status;
    int prevproc = rank - 1;
    int nextproc = rank + 1;

    double tstart = gettime();

    for(int it = 0; it < N; it++){
        if(rank == 0){
            MPI_Sendrecv(&old_b[local_size], n + 2, MPI_FLOAT, nextproc, n + 2,
                         &old_b[local_size + n + 2], n + 2, MPI_FLOAT, nextproc, n + 2,
                         comm, &status);
        }
        else if(rank == np - 1){
            MPI_Sendrecv(&old_b[n + 2], n + 2, MPI_FLOAT, prevproc, n + 2,
                         &old_b[0], n + 2, MPI_FLOAT, prevproc, n + 2,
                         comm, &status);
        }
        else{
            MPI_Sendrecv(&old_b[local_size], n + 2, MPI_FLOAT, nextproc, n + 2,
                         &old_b[0], n + 2, MPI_FLOAT, prevproc, n + 2,
                         comm, &status);

            MPI_Sendrecv(&old_b[n + 2], n + 2, MPI_FLOAT, prevproc, n + 2,
                         &old_b[local_size + n + 2], n + 2, MPI_FLOAT, nextproc, n + 2,
                         comm, &status);
        }


        // Calcular Formula // Interpolacion
        for(int i = 1; i < rows + 1; i++){
            for(int j = 1; j < n + 1; j++){
                new_b[i * (n + 2) + j] = (old_b[i * (n + 2) + (j - 1)] +
                                          old_b[i * (n + 2) + (j + 1)] +
                                          old_b[(i - 1) * (n + 2) + j] +
                                          old_b[(i + 1) * (n + 2) + j] -
                                          tmp[i * (n + 2) + j]) / 4.0;
            }
        }

        // Copiar new_b a old_b
        for(int i = n + 2; i < local_size + n + 2; i++){
            old_b[i] = new_b[i];
        }
    }

    // Recuperar toda la matriz en rank = 0
    MPI_Gather(&old_b[n + 2], local_size, MPI_FLOAT,
               edge, local_size, MPI_FLOAT, 0, comm);

    double tstop = gettime();

    if(rank == 0){

        printf("Time Parallel: %f seconds\n", tstop-tstart);
        printf("\n");

        // Devolver edge a buff
        edge2buff(edge, buff, m, n);

        // Escribir matriz de salida
        pgmwrite(outfile, buff, m, n);
        printf("\n");

        delete [] buff;
        delete [] edge;
    }

    delete [] old_b;
    delete [] new_b;
    delete [] tmp;

    MPI_Finalize();

    return 0;

}
