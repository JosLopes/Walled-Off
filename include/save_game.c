#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <math.h>
#include "datatypes.h"
#include "defines.h"
#include "movement.h"
#include "mapgen.h"

/*XP é guardado do jogo anterior*/
void save_game(int xp) 
{
  FILE* file = fopen("savegame.txt", "w");
  if (file == NULL) 
  {
    printf("Error opening savegame.txt\n");
    exit(1);
  }
  fprintf(file, "%d\n", xp);
  fclose(file);
}

int load_game() 
{
  FILE* file = fopen("savegame.txt", "r");
  if (file == NULL) 
  {
    printf("No savegame found\n");
    return 0;
  }
  int xp;
  fscanf(file, "%d", &xp);
  fclose(file);
  return xp;
}
