#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include "datatypes.h"
#include "defines.h"
#include "movement.h"
#include "mapgen.h"

/*movement restriction*/
int movement_restrictions (int x, int y, char map[][MAP_WIDTH])
{
  /*in case he is trying to move into a wall it stays in the same place*/
  if (map [x][y] == WALL_CHAR)
  {
    return 1; /*true -> can't move*/
  }
  /*in case he is trying to move into a enemy it stays in the same place*/
  else if (map [x][y] == ENEMY_O || map [x][y] == ENEMY_G || map [x][y] == ENEMY_S)
  {
    return 1;
  }
  else
  {
    return 0; /*false -> there are no movement restrictions so it can move*/
  }
}
/****************************
 *     Directions:           *
 *         0 = right         *
 *         1 = down          *
 *         2 = left          *
 *         3 = up            *
 *     q -> quit comand      *
 ****************************/
void movement(Character *character, int map_height, int map_width, char map[][map_width], WINDOW *main_window)
{
  char previous_char = FLOOR_CHAR;
  int ch, direction = 0;

  while ((ch = getch()) != 'q')
  {
    switch (ch)
    {
    /*Define the movement of the various directions using the arrow keys*/
    case KEY_LEFT:
      if (character -> x > 0)
      {
        /*movement restriction*/
        /*in case the caracter can't move*/
        if (movement_restrictions(character -> x - 1, character -> y, map))
        {
          direction = 2;
        }
        else 
        {
          /*restores the previous character in the map*/
          map [character -> x][character -> y] = previous_char;  /*guarda a posição atual no */
          previous_char = map [-- character -> x][character -> y];
          map [character -> x][character -> y] = PLAYER_CHAR_LEFT;
          direction = 2;
        }
      }
      break;

    case KEY_RIGHT:
      if (character -> x < COLS - 1)
      {
        if (movement_restrictions(character -> x+1, character -> y, map))
        {
          direction = 0;
        }
        else 
        {
          map [character -> x][character -> y] = previous_char;
          previous_char = map [++ character -> x][character -> y];
          map [character -> x][character -> y] = PLAYER_CHAR_RIGHT;
          direction = 0;
        }
      }
      break;
    case KEY_UP:
      if (character -> y > 0)
      {
        if (movement_restrictions(character -> x, character -> y - 1, map))
        {
          direction = 3;
        }
        else 
        {
          map [character -> x][character -> y] = previous_char;
          previous_char = map [character -> x][-- character -> y];
          map [character -> x][character -> y] = PLAYER_CHAR_UP;
          direction = 3;
        }
      }
      break;
    case KEY_DOWN:
      if (character -> y < LINES - 1)
      {
        if (movement_restrictions(character -> x, character -> y + 1, map))
        {
          direction = 1;
        }
        else 
        {
          map [character -> x][character -> y] = previous_char;
          previous_char = map [character -> x][++ character -> y];
          map [character -> x][character -> y] = PLAYER_CHAR_DOWN;
          direction = 1;
        }
      }
      break;

    /*Sets the character's rotation about itself*/
    case 'a':
      /* a -> 90° rotation counter clockwise*/
      direction = (direction + 3) % 4;
      break;
    case 'd':
      /* d -> 90° rotation clockwise*/
      direction = (direction + 1) % 4;
      break;
    default:
      break;
    }

    /*WARNING*/
    /*Define a fuction to remove this clear*/

    if (direction == 0)
    {
      /*right*/
      character -> direction = PLAYER_CHAR_RIGHT;
    }
    else if (direction == 1)
    {
      /*down*/
      character -> direction = PLAYER_CHAR_DOWN;
    }
    else if (direction == 2)
    {
      /*left*/
      character -> direction = PLAYER_CHAR_LEFT;
    }
    else
    {
      /*up*/
      character -> direction = PLAYER_CHAR_UP;
    }
    
    mvwprintw(main_window, character -> y, character -> x, "%c", character -> direction );
    for (int i=0; i < map_width; i++) {
      for (int j=0; j < map_height; j++) {
        mvwprintw(main_window, j, i, "%c", map[j][i]);
      }
    }
    wrefresh(main_window);
  }
}

/*
void damage (Character *character, WINDOW *main_window){
  if (map [character -> x][character -> y] == FIRE_CHAR)
  {
    character -> life = character -> life - 20;
  }
}
void potion (Character *character, WINDOW *main_window)
void food (Character *character, WINDOW *main_window)
*/