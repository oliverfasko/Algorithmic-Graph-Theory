#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "hrana.h"
#include "vrchol.h"
#include "digraf.h"

Digraf nacitajSubor(const char *nazovSuboru) {
  FILE *f = fopen(nazovSuboru, "r");

  int pocetVrcholov = 1;
  int pocetHran = 0;

  int u, v, c;

  while (fscanf(f, "%d %d %d", &u, &v, &c) == 3) {
    pocetHran++;

    if (pocetVrcholov < u)
      pocetVrcholov = u;
    if (pocetVrcholov < v)
      pocetVrcholov = v;
  }

  fclose(f);

  Digraf g;
  g.pocetHran = 0;
  g.pocetVrcholov = pocetVrcholov;
  g.orHrany = malloc(pocetHran * sizeof(Hrana));
  g.vrcholy = malloc(pocetVrcholov * sizeof(Vrchol));

  f = fopen(nazovSuboru, "r");

  for (int j = 1; j <= pocetHran; j++) {
    int vCislo, uCislo, c;

    fscanf(f, "%d %d %d", &uCislo, &vCislo, &c);

    Vrchol u;
    Vrchol v;

    u.cislo = uCislo;
    v.cislo = vCislo;

    pridajHranu(&g, u, v, c);
  }

  fclose(f);

  return g;
}

// ZAKLADNY ALGORITMUS
void zakladnyAlgoritmus(Vrchol startovaciVrchol) {};

int main() {
  Digraf g = nacitajSubor("graf.txt");
  printf("pocet hran: %d\n", g.pocetHran);
  printf("pocet vrcholov: %d\n", g.pocetVrcholov);

  return 0;
}
