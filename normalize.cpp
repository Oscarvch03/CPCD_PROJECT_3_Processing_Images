#include "pgmio.hpp"

int main(){
  string filename = "edge192x128.pgm";//"edge192x128.pgm";
  string filename2 = "out.pgm";
  //initialize 2D dynalloc array
  int m, n;
  pgmsize(filename, m, n);
  cout << "m: " << m << " n: " << n << endl;
  float* data = new float[m*n];

  pgmread(filename, data, m, n);

  pgmwrite(filename2, data, m, n);
  
  //delete dynalloc array
  delete[] data;
  
  return 0;
}
