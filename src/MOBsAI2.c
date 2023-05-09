#include "defines.h"
#include "datatypes.h"
#include <stdlib.h>

void init_is_awake (int number_of_enemies, Awake *is_awake)
{
  is_awake -> enemies_awaken = malloc (sizeof (Enemy) * number_of_enemies);
  is_awake -> total_size = number_of_enemies; 
  is_awake -> current_size = 0; /* Starts with 0 enemies in the awaken array */ 
}

int subtract_to_max (int x, int y)
{
  int res;
  if (x > y) res = x - y;
  else res = y - x;
  return res;
}

/* Calculates the distance from the player to the enemy */
int distance_from_character (Character *character, Enemy enemy)
{
  int res;
  res = subtract_to_max (character -> y, enemy.y) + subtract_to_max (character -> x, enemy.x);
  return res;
}

/* 
  When the enemy is in a certain range from the player, it awakens (is added to the
  awaken array, inicating that it will pursue the player to kill him) */
void init_awaken_enemies (Character *character, Enemy *enemies, Awake *is_awake)
{
  int index, distance;

  for (index = 0; index < is_awake -> total_size; index++)
  {
    /* Distance between the player and the enemy */
    distance = distance_from_character (character, enemies[index]);
    if (distance < AWAKE_RANGE && enemies[index].awake == 1)
    {
      is_awake -> enemies_awaken[is_awake -> current_size] = enemies[index];
      is_awake -> current_size ++;
      enemies[index].awake = 0;
    }
  }
}