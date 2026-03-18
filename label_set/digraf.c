#include <stdbool.h>
#include "hrana.h"
#include "vrchol.h"
#include "digraf.h"

void pridajHranu(Digraf *g, Vrchol u, Vrchol v, int c)
{
    int i = g->pocetHran;

     

    g->orHrany[i].u = u;
    g->orHrany[i].v = v;
    g->orHrany[i].c = c;

    g->pocetHran++;
}
