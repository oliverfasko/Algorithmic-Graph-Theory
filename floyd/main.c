#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "digraf.h"
#include "hrana.h"
#include "vrchol.h"

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

void floyd(Digraf *g) {
  int n = g->pocetVrcholov;

  // Vytvorenie a inicializacia matice vzdialenosti
  int dist[n][n];
  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++)
      if (i == j) { // sam od seba je vzdialeny 0
        dist[i][j] = 0;
      } else {
        dist[i][j] = 9999;
      }

  // Naplnenie cien podla hran grafu (cisla vrcholov su od 1, indexy od 0)
  for (int i = 0; i < g->pocetHran; i++) {
    int u = g->orHrany[i].u.cislo - 1;
    int v = g->orHrany[i].v.cislo - 1;
    int c = g->orHrany[i].c;
    dist[u][v] = c;
  }

  // Hlavna slucka
  for (int k = 0; k < n; k++)
    for (int i = 0; i < n; i++)
      for (int j = 0; j < n; j++)
        if (dist[i][k] + dist[k][j] < dist[i][j])
          dist[i][j] = dist[i][k] + dist[k][j];

  // Vypis matice
  printf("\nMatica najkratsich vzdialenosti:\n");
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (dist[i][j] == 9999)
        printf("  9999");
      else
        printf("%6d", dist[i][j]);
    }
    printf("\n");
  }
}

int main() {
  Digraf g = nacitajSubor("graf.txt");

  floyd(&g);

  return 0;
}
