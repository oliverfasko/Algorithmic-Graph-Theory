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
  g.vrcholy =
      malloc((pocetVrcholov + 1) * sizeof(Vrchol)); // +1: indexujeme od 1

  for (int i = 1; i <= pocetVrcholov; i++) {
    g.vrcholy[i].cislo = i;
  }

  f = fopen(nazovSuboru, "r");
  for (int j = 0; j < pocetHran; j++) {
    int uCislo, vCislo, cena;
    fscanf(f, "%d %d %d", &uCislo, &vCislo, &cena);

    Vrchol uu, vv;
    uu.cislo = uCislo;
    vv.cislo = vCislo;
    pridajHranu(&g, uu, vv, cena);
  }
  fclose(f);

  return g;
}

void label_set_algoritmus(Digraf *graf, Vrchol zaciatocny_vrchol,
                          Vrchol konecny_vrchol) {

  int pocet_vrcholov = graf->pocetVrcholov;

  int t[pocet_vrcholov + 1]; // vzdialenost
  bool v_mnozine_E[pocet_vrcholov + 1];

  for (int i = 0; i <= pocet_vrcholov; i++) {
    t[i] = 9999;
    v_mnozine_E[i] = false;
  }

  t[zaciatocny_vrchol.cislo] = 0;
  v_mnozine_E[zaciatocny_vrchol.cislo] = true;

  printf("Startovaci vrchol: %d, Cielovy vrchol: %d\n\n",
         zaciatocny_vrchol.cislo, konecny_vrchol.cislo);

  int krok = 0;

  // ideme pokial konecny vrchol nebude v Epsilon 
  while (!v_mnozine_E[konecny_vrchol.cislo]) {
    krok++;

    // ak je vrchol u v Epsilon a cesta z u do v je kratsia ako doteraz tak nastavime t[v]
    for (int h = 0; h < graf->pocetHran; h++) {
      Hrana hrana = graf->orHrany[h];
      int u = hrana.u.cislo;
      int v = hrana.v.cislo;
      int c = hrana.c;

      if (v_mnozine_E[u] && t[u] + c < t[v]) {
        t[v] = t[u] + c;
      }
    }

    // z vrcholov ktore nie su v Epsilon, vyberieme ten s najmensou vzdialenostou a pridame do Epislon
    int min_t = 9999;
    int novy_vrchol = -1;

    for (int i = 1; i <= pocet_vrcholov; i++) {
      if (!v_mnozine_E[i] && t[i] < min_t) {
        min_t = t[i];
        novy_vrchol = i;
      }
    }

    v_mnozine_E[novy_vrchol] = true;

    printf("Krok %d: vrchol %d pridany do Epsilon, t=%d\n", krok, novy_vrchol, min_t);
  }

  // Výsledok
  printf("\nNajkratsia vzdialenost z %d do %d je %d\n", zaciatocny_vrchol.cislo,
         konecny_vrchol.cislo, t[konecny_vrchol.cislo]);
}

// hlavna funkcia
int main() {
  Digraf g = nacitajSubor("graf.txt");

  label_set_algoritmus(&g, g.vrcholy[5], g.vrcholy[1]);

  return 0;
}
