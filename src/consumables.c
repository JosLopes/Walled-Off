#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <math.h>
#include <time.h>
#include "datatypes.h"
#include "defines.h"
#include "movement.h"
#include "mapgen.h"
#include "consumables.h"

Consumables *consumablesHeap(void) {
  /* Different foods and potions */

  Consumables *consumables = malloc(NUM_CONSUMABLES * sizeof(Consumables));
  consumables[0] = (Consumables){'A', "Apple", 10, 0, 0, 0};
  consumables[1] = (Consumables){'V', "Avocado", 20, 0, 0, 0};
  consumables[2] = (Consumables){'C', "Chicken", 30, 0, 0, 0};
  consumables[3] = (Consumables){'E', "Dreamweaver Elixir", 20, 10, 0, 0};
  consumables[4] = (Consumables){'B', "Dragonfire Brew", 40, 20, 0, 0};

  /* Have to free the memory that I allocate here somewhere in the program. Do this later.*/
  return consumables;
}