#ifndef VRCHOl 
#define VRCHOl 


typedef struct Vrchol {
  int cislo;
  int t;
  struct Vrchol* x;
} Vrchol;

char* toString(Vrchol v);

#endif 
