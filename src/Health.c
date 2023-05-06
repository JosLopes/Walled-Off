#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <math.h>
#include <time.h>
#include "datatypes.h"
#include "defines.h"
#include "movement.h"
#include "mapgen.h"
#include "Consumables.h"

/*
HEALTH is going to be affected by:
-Attacks
-Potions/Food

xP is going to be affected by:
-Potions
-Mobs until a certain %
*/

/*This function is responsible for the enemies attacks and for crossing the fire*/
/* function that takes in a map, a character, and an array of enemies, and 
decrements the character's life if they are attacked by an enemy within range */
void take_damage(char map[][MAP_WIDTH], Character* character, Enemy enemies[], int num_enemies) 
{
  /*case character is crossing the fire*/
  if (map [character -> y][character -> x] == FIRE_CHAR)
  {
    character -> life = character -> life - 10;
  }

  /*cross the array of enemies checking if there's any attacking the character*/
  for (int i = 0; i < num_enemies; i++) 
  {
    Enemy* enemy = &enemies[i];
    int distance = sqrt(pow(character->x - enemy->x, 2) + pow(character->y - enemy->y, 2));
    
    /*case character is on the range of attack of enemy*/
    if (distance <= enemy->tag.range) 
    {
      if ((character->xp > 25) && (character->xp - enemy->damage > 25)) {
        /*Enemy is within range, so decrement character's xp*/
        character->xp -= enemy->damage;

        mvprintw(5, 5, "%d", character->life);
        mvprintw(10, 10, "%d", character->xp);
      } else if ((character->xp > 25) && (character->xp - enemy->damage < 25)) {
        int take_from_xp = character->xp - 25;
        int remaining_damage = enemy->damage - take_from_xp;
        int take_from_life = character->life - remaining_damage;

        character->xp -= take_from_xp;
        character->life -= take_from_life;

        mvprintw(5, 5, "%d", character->life);
        mvprintw(10, 10, "%d", character->xp);
      } else {
        /*Enemy is within range, so decrement character's life*/
        character->life -= enemy->damage;

        mvprintw(5, 5, "%d", character->life);
        mvprintw(10, 10, "%d", character->xp);
      }

      /*Check if character is dead*/
      if (character->life <= 0) 
      {
        exit(0);
      }
    }
  }
}

/* 
* Function to do the spawn of foods and potions 
*/
void place_foods_and_potions (int map_heigth, int map_width, char map[][map_width], int nor_size, Room *rooms, Consumables *Foods[], Consumables *Potions[]) 
{
  /* Use this random number to determine the number of foods/potions in the map */
  int random_number;
  srand(time(0)); // Seed the random number generator with the current time
  random_number = rand() % 10; //Number between 0 and 9

  for (int i=0; i<random_number; i++)    
  {
    /* Use this random number to determine which food/potion we should put in the map */
    int random_number;
    srand(time(0)); // Seed the random number generator with the current time
    random_number = rand() % 5; //Number between 0 and 4

    /* If the random number is less than 3, then spawn a food */
    if (random_number < 3) {
      /* Generate a random number between 0 and the number of rooms */
      random_number = rand() % nor_size;
      /* Place the food in the room */
      map[rooms[random_number].y + 1][rooms[random_number].x + 1] = FOOD_CHAR;
      /* Set the food's coordinates */
      Foods[random_number] -> x = rooms[random_number].x + 1;
      Foods[random_number] -> y = rooms[random_number].y + 1;
    }

    /* If the random number is greater (or equal) than 3, then spawn a potion */
    else {
      /* Generate a random number between 0 and the number of rooms */
      random_number = rand() % nor_size;
      /* Place the potion in the room */
      map[rooms[random_number].y + 1][rooms[random_number].x + 1] = POTION_CHAR;
      /* Set the potion's coordinates */
      Potions[random_number] -> x = rooms[random_number].x + 1;
      Potions[random_number] -> y = rooms[random_number].y + 1;
    }
  }
}

/*
* This function is responsible for increasing/decreasing health based on ingestible food/potions
*/
void food_and_potions (char map[][MAP_WIDTH], Character* character, Consumables Foods[], Consumables Potions[]) 
{
  /*case character is crossing food*/
  if (map [character -> y][character -> x] == FOOD_CHAR)
  {
    /*Remove health*/
    character -> life = character -> life - Foods -> impact;
    /*Delete the food and substitute it by a point*/
    map [Foods -> y][Foods -> x] == '.';

  }
    /*case character is crossing potions*/
  if (map [character -> y][character -> x] == POTION_CHAR)
  {
    /*Remove health*/
    character -> life = character -> life - Potions -> impact;
    /*Delete the potion and substitute it by a point*/
    map [Potions -> y][Potions -> x] == '.';
  }
}