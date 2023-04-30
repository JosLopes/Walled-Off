#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <math.h>
#include "datatypes.h"
#include "defines.h"
#include "movement.h"
#include "mapgen.h"

/* Add x, y coordinates */
/* Initialized with (0,0) coordinates */

void ConsumablesHeap() {
  /* Different foods */
  Consumables *Foods = malloc(numFoods*sizeof(Consumables));
  Foods[0] = (Consumables){"Apple", +10, 0, 0};
  Foods[1] = (Consumables){"Avocado", +20, 0, 0};
  Foods[2] = (Consumables){"Chicken", +30, 0, 0};

  /* Different potions */
  Consumables *Potions = malloc(numPotions*sizeof(Consumables));
  Potions[0] = (Consumables){"Dreamweaver Elixir", +20, 0, 0};
  Potions[1] = (Consumables){"Dragonfire Brew", +40, 0, 0};

  free(Foods);
  Foods = NULL;
  free(Potions);
  Potions = NULL;

}


