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


/*
* a104188 - Ana Cerqueira
* It define the consumables available in the game
*/
Consumables *consumablesHeap(void) {
  
  /* Different foods and potions */
  Consumables *consumables = malloc(NUM_CONSUMABLES * sizeof(Consumables));
  consumables[0] = (Consumables){'=', "Apple", 
                                  {"Apple: +10 life                                  "},
                                  10, 10, 0, 0};
  consumables[1] = (Consumables){'=', "Avocado",
                                  {"Avocado: +20 life                                "},
                                  20, 10, 0, 0};
  consumables[2] = (Consumables){'=', "Chicken",
                                  {"Chicken: +30 life                                "},
                                   30, 15, 0, 0};
  consumables[3] = (Consumables){'@', "Dreamweaver Elixir",
                                  {"Dreamweaver Elixir: +20 life  +10 XP             "},
                                  20, 40, 0, 0};
  consumables[4] = (Consumables){'@', "Dragonfire Brew",
                                  {"Dragonfire Brew: +40 life  +20 XP                "},
                                  40, 60, 0, 0};

  return consumables;
}
