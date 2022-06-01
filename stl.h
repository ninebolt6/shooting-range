#pragma once

class Stl {
public:
  Stl(const char *fileName) {
    readStl(fileName);
  }
  int tnum;
  float *x,*y,*z;
  float *nx,*ny,*nz;
private:
  void readStl(const char *fileName);
};