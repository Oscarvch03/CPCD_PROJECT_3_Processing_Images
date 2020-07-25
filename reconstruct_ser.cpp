// Algoritmo Serial hecho por Carlos Alvarez

#include <sys/time.h>
#include "pgmio.hpp"

double gettime(){
  struct timeval tp;
  gettimeofday (&tp, NULL);
  return tp.tv_sec + tp.tv_usec/(double)1.0e6;
}

int main(int argc, char** argv){

    // Imagenes para procesar //////////////////////////////////////////////////
    int I = 5;

    string fileA = "edge192x128.pgm";
    string outA = "edge192x128_re_ser.pgm";

    string fileB = "edge256x192.pgm";
    string outB = "edge256x192_re_ser.pgm";

    string fileC = "edge512x384.pgm";
    string outC = "edge512x384_re_ser.pgm";

    string fileD = "edge768x768.pgm";
    string outD = "edge768x768_re_ser.pgm";

    string fileE  = "edge4896x3264.pgm";
    string outE = "edge4896x3264_re_ser.pgm";
    ////////////////////////////////////////////////////////////////////////////

    string filenames[I] = {fileA, fileB, fileC, fileD, fileE};
    string outfiles[I] = {outA, outB, outC, outD, outE};

  int N = atoi(argv[1]);

    // Escoger Imagen que se quiere procesar ///////////////////////////////////
    int Nimg = atoi(argv[2]); // # de Imagen que se desea procesar
    string filename = filenames[Nimg - 1];
    string outfile = outfiles[Nimg - 1];
    ////////////////////////////////////////////////////////////////////////////

  //allocate buffer
  int m, n;
  pgmsize(filename, m, n);
  printf("\n");
  cout << "m: " << m << " n: " << n << endl;
  float* buff = new float[m*n];
  //allocate array to store the buffer + halo
  //allocate array old_b
  //allocate array new_b
  float* edge = new float[(m+2)*(n+2)];
  float* old_b = new float[(m+2)*(n+2)];
  float* new_b = new float[(m+2)*(n+2)];

  //set halos to 255
  for(int j = 0; j < n+2; j++){
    edge[0*(n+2)+j] = 255; edge[(m+1)*(n+2)+j] = 255;
    new_b[0*(n+2)+j] = 255; new_b[(m+1)*(n+2)+j] = 255;
  }
  for(int i = 1; i < m+1; i++){
    edge[i*(n+2)+0] = 255; edge[i*(n+2)+(n+1)] = 255;
    new_b[i*(n+2)+0] = 255; new_b[i*(n+2)+(n+1)] = 255;
  }

  //read image to buff
  pgmread(filename, buff, m, n);

  //copy buff to edge
  for(int i = 0; i < m; i++){
    for(int j = 0; j < n; j++){
      edge[(i+1)*(n+2)+(j+1)] = buff[i*n+j];
    }
  }

  //Initialize the old_b array
  for(int i = 0; i < m+2; i++){
    for(int j = 0; j < n+2; j++){
      old_b[i*(n+2)+j] = 255;
    }
  }

  double tstart = gettime();

  //Loop over iterations-------------------------------------
  for(int it = 0; it < N; it++){
    //get new values (do not update the halo)
    for(int i = 1; i < m+1; i++){
      for(int j = 1; j < n+1; j++){
  	new_b[i*(n+2)+j] = (old_b[i*(n+2)+(j-1)]+
			     old_b[i*(n+2)+(j+1)]+
			     old_b[(i-1)*(n+2)+j]+
			     old_b[(i+1)*(n+2)+j]-
			     edge[i*(n+2)+j]) / 4;
      }
    }

    //backup the new to the old array (without the halo)
    for(int i = 1; i < m+1; i++){
      for(int j = 1; j < n+1; j++){
  	old_b[i*(n+2)+j] = new_b[i*(n+2)+j];
      }
    }
  }
  //---------------------------------------------------------

  double tstop = gettime();

  printf("Time Serial: %f seconds\n", tstop-tstart);
  printf("\n");

  //copy old_b back to buff
  for(int i = 1; i < m+1; i++){
    for(int j = 1; j < n+1; j++){
      buff[(i-1)*n+(j-1)] = old_b[i*(n+2)+j];
    }
  }

  //write image
  pgmwrite(outfile, buff, m, n);
  printf("\n");

  //free edge, old_b and new_b
  delete[] edge;
  delete[] old_b;
  delete[] new_b;
  //free buffer
  delete[] buff;

  return 0;
}
