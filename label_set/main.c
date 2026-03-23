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

// --- Adjacency list: pre kazdy vrchol zoznam hran ktore z neho vychadzaju ---
 
typedef struct {
  Hrana *hrany;        // pole hran vychadzajucich z tohto vrcholu
  int   pocetHran;     // kolko hran aktualne obsahuje
  int   kapacita;      // alokovana kapacita
} ZoznamHranVrcholu;
 
ZoznamHranVrcholu *postavAdjacencyList(Digraf *graf) {
  int pocetVrcholov = graf->pocetVrcholov;
 

  // for (vsetky hrany grafu) {        
  //  if (hrana nevychádza z r) continue;
  ZoznamHranVrcholu *adjacencyList = calloc(pocetVrcholov + 1, sizeof(ZoznamHranVrcholu));
 
  for (int poradieHrany = 0; poradieHrany < graf->pocetHran; poradieHrany++) {
    Hrana aktualnaHrana = graf->orHrany[poradieHrany];
    int zdrojVrchol = aktualnaHrana.u.cislo;
 
    ZoznamHranVrcholu *zoznam = &adjacencyList[zdrojVrchol];
 
    // Zdvojnasob kapacitu ak je zoznam plny
    if (zoznam->pocetHran == zoznam->kapacita) {
      zoznam->kapacita = zoznam->kapacita == 0 ? 4 : zoznam->kapacita * 2;
      zoznam->hrany = realloc(zoznam->hrany, zoznam->kapacita * sizeof(Hrana));
    }
 
    zoznam->hrany[zoznam->pocetHran++] = aktualnaHrana;
  }
 
  return adjacencyList;
}

void uvolniAdjacencyList(ZoznamHranVrcholu *adjacencyList, int pocetVrcholov) {
  for (int idx = 1; idx <= pocetVrcholov; idx++) {
    free(adjacencyList[idx].hrany);
  }
  free(adjacencyList);
}

// --- Pomocne funkcie pre mnozinu epsilon (dynamicke pole) ---

int epsilon_obsahuje(int *epsilon, int velkostEpsilon, int cisloVrcholu) {
  for (int idx = 0; idx < velkostEpsilon; idx++) {
    if (epsilon[idx] == cisloVrcholu)
      return 1;
  }
  return 0;
}

void epsilon_pridaj(int **epsilon, int *velkostEpsilon, int cisloVrcholu) {
  if (epsilon_obsahuje(*epsilon, *velkostEpsilon, cisloVrcholu))
    return;
  (*velkostEpsilon)++;
  *epsilon = realloc(*epsilon, (*velkostEpsilon) * sizeof(int));
  (*epsilon)[*velkostEpsilon - 1] = cisloVrcholu;
}

void epsilon_odstran(int **epsilon, int *velkostEpsilon, int cisloVrcholu) {
  for (int idx = 0; idx < *velkostEpsilon; idx++) {
    if ((*epsilon)[idx] == cisloVrcholu) {
      (*epsilon)[idx] = (*epsilon)[*velkostEpsilon - 1];
      (*velkostEpsilon)--;
      *epsilon = realloc(*epsilon, (*velkostEpsilon) * sizeof(int));
      return;
    }
  }
}

// ---------------------------------------
// Label Set
// ---------------------------------------

void label_set_algoritmus(Digraf *graf, Vrchol zaciatocnyVrchol,
                          Vrchol konecnyVrchol) {

  int pocetVrcholov = graf->pocetVrcholov;

  ZoznamHranVrcholu *adjacencyList = postavAdjacencyList(graf); // v jave Hashmap<Int ,List<Hrana>>

  int vzdialenost[pocetVrcholov + 1]; // najkratsia znama vzdialenost z pociatku
  int predchodca[pocetVrcholov+ 1]; // predchadzajuci vrchol na najkratsej ceste

  for (int idx = 0; idx <= pocetVrcholov; idx++) {
    vzdialenost[idx] = 9999;
    predchodca[idx] = -1;
  }

  vzdialenost[zaciatocnyVrchol.cislo] = 0;

  int *epsilon = malloc(sizeof(int));
  int velkostEpsilon = 0;
  epsilon_pridaj(&epsilon, &velkostEpsilon, zaciatocnyVrchol.cislo);

  int krok = 0;

  while (velkostEpsilon > 0) {
    krok++;

    // Z epsilonu vyberieme riadiaci vrchol s najmensou vzdialenostou
    int minVzdialenost = 9999;
    int riadiaciVrchol = -1;
    for (int idx = 0; idx < velkostEpsilon; idx++) {
      if (vzdialenost[epsilon[idx]] < minVzdialenost) { 
        minVzdialenost = vzdialenost[epsilon[idx]];
        riadiaciVrchol = epsilon[idx];
      }
    }

    epsilon_odstran(&epsilon, &velkostEpsilon, riadiaciVrchol);


    // vypis kroku
    printf("Krok %d: riadiaci=%d, vzdialenost=%d, epsilon={", krok, riadiaciVrchol, minVzdialenost);

    for (int idx = 0; idx < velkostEpsilon; idx++)
      printf(idx < velkostEpsilon - 1 ? "%d," : "%d", epsilon[idx]);

    printf("}\n");

    // Kontrola hran vychadzajucich z riadiaceho vrcholu
    ZoznamHranVrcholu *hranyRiadiaceho = &adjacencyList[riadiaciVrchol];

    // for (hrany tohto vrcholu)
    for (int poradieHrany = 0; poradieHrany < hranyRiadiaceho->pocetHran; poradieHrany++) {
      Hrana aktualnaHrana = hranyRiadiaceho->hrany[poradieHrany]; //napr 5
 
      int susednyVrchol   = aktualnaHrana.v.cislo; //napr 2
      int cenaHrany       = aktualnaHrana.c; // napr 20
      int novaVzdialenost = vzdialenost[riadiaciVrchol] + cenaHrany; // napr 0 + 20
 
      // Zlepsi sa label susedneho vrcholu?
      if (vzdialenost[susednyVrchol] > novaVzdialenost) { //napr 9999 > 20 -> TRUE
        vzdialenost[susednyVrchol] = novaVzdialenost; //napr 9999 sa prepise na 20
        predchodca[susednyVrchol]  = riadiaciVrchol; // napr z 5 sme prisli do 2
 
        // Pridame do epsilonu len ak tam este nie je
        epsilon_pridaj(&epsilon, &velkostEpsilon, susednyVrchol); //napr E = {2}
      }
    }
  }
   
  free(epsilon);
  uvolniAdjacencyList(adjacencyList, pocetVrcholov);


  // vypis cety konecnej vzdialenosti 
  printf("\nNajkratsia vzdialenost z %d do %d je %d\n", zaciatocnyVrchol.cislo,
         konecnyVrchol.cislo, vzdialenost[konecnyVrchol.cislo]); 

  // vypis cesty spetne cez predchodcov
  int cesta[pocetVrcholov + 1];
  int dlzkaCesty = 0;
  int aktualnyVrchol = konecnyVrchol.cislo;

  while (aktualnyVrchol != -1) {
    cesta[dlzkaCesty++] = aktualnyVrchol;
    aktualnyVrchol = predchodca[aktualnyVrchol];
  }

  printf("Cesta: ");
  for (int idx = dlzkaCesty - 1; idx >= 0; idx--) {
    if (idx > 0)
      printf("%d -> ", cesta[idx]);
    else
      printf("%d\n", cesta[idx]);
  }
}

// hlavna funkcia
int main() {
  Digraf g = nacitajSubor("graf.txt");

  label_set_algoritmus(&g, g.vrcholy[5], g.vrcholy[1]);

  return 0;
}
