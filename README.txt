PROJECT 3 PROCESSING IMAGES MPI
PRESENTADO POR: OSCAR VELASCO CHIQUILLO
PRESENTADO A: CARLOS ALVAREZ
12 de Mayo de 2020

En este archivo se pretende describir el repositorio en el cual se encuentra el
Proyecto PROCESSING IMAGES con MPI.


Link Repositorio en Github: https://github.com/Oscarvch03/CPCD_PROJECT_3_Processing_Images


Nota: Se deben agregar las imagenes .pgm y considerar la siguiente numeracion:
1. edge192x128.pgm
   out_ser: edge192x128_re_ser.pgm
   out_par: edge192x128_re_par.pgm
2. edge256x192.pgm
   out_ser: edge256x192_re_ser.pgm
   out_par: edge256x192_re_par.pgm
3. edge512x384.pgm
   out_ser: edge512x384_re_ser.pgm
   out_par: edge512x384_re_par.pgm
4. edge768x768.pgm
   out_ser: edge768x768_re_ser.pgm
   out_par: edge768x768_re_par.pgm
5. edge4896x3264.pgm
   out_ser: edge4896x3264_re_ser.pgm
   out_par: edge4896x3264_re_par.pgm


Nota 2: Las pruebas se hicieron en el Cluster de la Universidad del Rosario.


Para Compilar:

    i = 1, 2, 3, 4, 5

    'make reconstruct_ser': Compila reconstruct_ser.cpp, que reconstruye la
                            imagen con el algoritmo serial.
                            Genera el ejecutable 'reconstruct_ser.out'

    'make reconstruct_par': Compila reconstruct_par.cpp, que reconstruye la
                            imagen con el algoritmo paralelo (MPI).
                            Genera el ejecutable 'reconstruct_par.out'

    'make clean': Borra los ejecutables generados.


Para Ejecutar:

    './reconstruct_ser k i': Se ejecuta el algoritmo serial con k iteraciones
                             para la imagen con numero i.
                             Genera el archivo "out_ser", asociado a la
                             imagen con numero i.

    'mpiexec -np n reconstruct_ser k i': Se ejecuta el algoritmo paralelo con
                                         n procesos y k iteraciones para la
                                         imagen con numero i.
                                         Genera el archivo "out_par", asociado
                                         a la imagen con numero i.
