// Mini Project bmpfuncs.cpp 12171788 박지은

#include <stdio.h>
#include <stdlib.h>

#include "bmpfuncs.h"

#define VERBOSE false

void storeImage(float *imageOut, const char *filename, int rows, int cols,
   const char* refFilename) {

   FILE *ifp, *ofp;
   unsigned char tmp;
   int offset;
   unsigned char *buffer;
   int i, j;

   int bytes;

   int height, width;

   ifp = fopen(refFilename, "rb");
   if (ifp == NULL) {
      perror("bmpfunc.cpp : error file name ");
      perror(filename);
  
