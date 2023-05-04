#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <math.h>
#include "datatypes.h"
#include "defines.h"
#include "movement.h"
#include "mapgen.h"
#include <time.h>

/* Add x, y coordinates */
/* Initialized with (0,0) coordinates */
void ConsumablesHeap() {
  /* Different foods */
  Consumables *Foods = malloc(NUM_FOODS*sizeof(Consumables));
  Consumables Apple = {"Apple", +10, 0, 0};
  Consumables Avocado = {"Avocado", +20, 0, 0};
  Consumables Chicken = {"Chicken", +30, 0, 0};

  Foods[0] = Apple;
  Foods[1] = Avocado;
  Foods[2] = Chicken;
  
  /* Different potions */
  Consumables *Potions = malloc(NUM_POTIONS*sizeof(Consumables));
  Consumables Dreamweaver_Elixir = {"Dreamweaver Elixir", +20, 0, 0};
  Consumables Dragonfire_Brew = {"Dragonfire Brew", +40, 0, 0};

  Potions[0] = Dreamweaver_Elixir;
  Potions[1] = Dragonfire_Brew;

  /* Have to free the memory that I allocate here somewhere in the program. Do this later.*/
}


