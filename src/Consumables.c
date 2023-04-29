#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <math.h>
#include "datatypes.h"
#include "defines.h"
#include "movement.h"
#include "mapgen.h"

/* Add x, y coordinates */
/* Initialized with (0,0) coordinates*/

/* Different foods */
Consumables Foods[3] = {
  {"Apple", +10, 0, 0},
  {"Avocado", +20, 0, 0},
  {"Chicken", +30, 0, 0},
  /*{"Arroz", +40},
  {"Espinafre", +50},
  {"Ananás", +60},
  {"Perú", +70},
  {"Manga", +80},
  {"IsoDrink", +90},
  {"Smoothie", +100}*/
};

/* Different potions */
Consumables Potions[2] = {
  {"Dreamweaver Elixir", +20, 0, 0},
  {"Dragonfire Brew", +40, 0, 0},
  /*{"Shadowmist Tonic", +60},
  {"Enchanted Essence", -30},
  {"Phoenix Tears Potion", -60},*/
};
