#ifndef HRANA
#define HRANA 

#include "vrchol.h"

typedef struct Hrana {
  int c;
  Vrchol v;
  Vrchol u;
} Hrana;

bool jeIncidentna(Hrana h, Vrchol v); 

#endif // !HRANA
#define hranva()
