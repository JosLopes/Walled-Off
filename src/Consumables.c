#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <math.h>
#include "datatypes.h"
#include "defines.h"
#include "movement.h"
#include "mapgen.h"

//Different foods
Consumables Foods[3] = {
  {"Apple", +10},
  {"Avocado", +20},
  {"Chicken", +30},
  /*{"Arroz", +40},
  {"Espinafre", +50},
  {"Ananás", +60},
  {"Perú", +70},
  {"Manga", +80},
  {"IsoDrink", +90},
  {"Smoothie", +100}*/
};

//Different potions
Consumables Potions[2] = {
  {"Dreamweaver Elixir", +20},
  {"Dragonfire Brew", +40},
  /*{"Shadowmist Tonic", +60},
  {"Enchanted Essence", -30},
  {"Phoenix Tears Potion", -60},*/
};
