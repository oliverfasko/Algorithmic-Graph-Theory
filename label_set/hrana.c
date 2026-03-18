#include <stdio.h>
#include <stdbool.h>
#include "hrana.h"
#include "vrchol.h"

bool jeIncidentna(Hrana h, Vrchol v){
  return (h.v.cislo == v.cislo || h.u.cislo == v.cislo);
}
