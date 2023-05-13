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

void ConsumablesHeap() {
  /* Different foods and potions */
  Consumables *consumables = malloc(NUM_CONSUMABLES*sizeof(Consumables));
  Consumables Apple = {"Apple", +10, 0, 0, 0};
  Consumables Avocado = {"Avocado", +20, 0, 0, 0};
  Consumables Chicken = {"Chicken", +30, 0, 0, 0};
  Consumables Dreamweaver_Elixir = {"Dreamweaver Elixir", +20, +10, 0, 0};
  Consumables Dragonfire_Brew = {"Dragonfire Brew", +40, +20, 0, 0};

  consumables[0] = Apple;
  consumables[1] = Avocado;
  consumables[2] = Chicken;
  consumables[3] = Dreamweaver_Elixir;
  consumables[4] = Dragonfire_Brew;

  /* Have to free the memory that I allocate here somewhere in the program. Do this later.*/
}