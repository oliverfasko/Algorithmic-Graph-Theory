#ifndef DIGRAF
#define DIGRAF

#include "hrana.h"
#include "vrchol.h"

typedef struct Digraf{
  int pocetVrcholov;
  int pocetHran;
  Vrchol *vrcholy;
  Hrana *orHrany;
} Digraf;

void pridajHranu(Digraf *g, Vrchol u, Vrchol v, int c);

#endif // !DIGRAF
