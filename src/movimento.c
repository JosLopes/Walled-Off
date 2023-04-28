#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include "datatypes.h"
#include "defines.h"
#include "movement.h"
#include "mapgen.h"
#include "vision.h"

/*movement restriction*/
int movement_restrictions(int x, int y, char map[][MAP_WIDTH])
{
  /*in case he is trying to move into a wall it stays in the same place*/
  if (map[y][x] == WALL_CHAR)
  {
    return 1; /*true -> can't move*/
  }
  /*in case he is trying to move into a enemy it stays in the same place*/
  else if (map[y][x] == ENEMY_O || map[y][x] == ENEMY_G || map[y][x] == ENEMY_S)
  {
    return 1;
  }
  /*in case the position is outside the map limits*/
  else if (y >= MAP_HEIGHT || x >= MAP_WIDTH || x < 0 || y < 0)
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
  int ch;

  while ((ch = wgetch(main_window)) != 'q')
  {
    /*Clear previous character position*/
    mvwprintw(main_window, character->y, character->x, " ");

    /*Update character position based on input*/
    switch (ch)
    {
    /*Define the movement of the various directions using the arrow keys*/
    case KEY_LEFT:
      /*movement restriction*/
      /*in case the caracter can't move*/
      if (movement_restrictions(character->x - 1, character->y, map))
      {
        character->direction = PLAYER_CHAR_LEFT;
        map[character->y][character->x] = PLAYER_CHAR_LEFT;
      }
      else
      {
        /*restores the previous character in the map*/
        map[character->y][character->x] = previous_char;
        previous_char = map[character->y][--character->x];
        map[character->y][character->x] = PLAYER_CHAR_LEFT;
        character->direction = PLAYER_CHAR_LEFT;
      }
      break;

    case KEY_RIGHT:
      if (movement_restrictions(character->x + 1, character->y, map))
      {
        character->direction = PLAYER_CHAR_RIGHT;
        map[character->y][character->x] = PLAYER_CHAR_RIGHT;
      }
      else
      {
        map[character->y][character->x] = previous_char;
        previous_char = map[character->y][++character->x];
        map[character->y][character->x] = PLAYER_CHAR_RIGHT;
        character->direction = PLAYER_CHAR_RIGHT;
      }
      break;
    case KEY_UP:
      if (movement_restrictions(character->x, character->y - 1, map))
      {
        character->direction = PLAYER_CHAR_UP;
        map[character->y][character->x] = PLAYER_CHAR_UP;
      }
      else
      {
        map[character->y][character->x] = previous_char;
        previous_char = map[--character->y][character->x];
        map[character->y][character->x] = PLAYER_CHAR_UP;
        character->direction = PLAYER_CHAR_UP;
      }
      break;
    case KEY_DOWN:
      if (movement_restrictions(character->x, character->y + 1, map))
      {
        character->direction = PLAYER_CHAR_DOWN;
        map[character->y][character->x] = PLAYER_CHAR_DOWN;
      }
      else
      {
        map[character->y][character->x] = previous_char;
        previous_char = map[++character->y][character->x];
        map[character->y][character->x] = PLAYER_CHAR_DOWN;
        character->direction = PLAYER_CHAR_DOWN;
      }
      break;
    
    /*Sets the character's rotation about itself*/
    /* a -> 90° rotation counterclockwise*/
    case 'a':
      switch (character->direction)
      {
        case PLAYER_CHAR_UP:
          map[character->y][character->x] = PLAYER_CHAR_LEFT;
          character->direction = PLAYER_CHAR_LEFT;
          break;
        case PLAYER_CHAR_LEFT:
          map[character->y][character->x] = PLAYER_CHAR_DOWN;
          character->direction = PLAYER_CHAR_DOWN;
          break;
        case PLAYER_CHAR_DOWN:
          map[character->y][character->x] = PLAYER_CHAR_RIGHT;
          character->direction = PLAYER_CHAR_RIGHT;
          break;
        case PLAYER_CHAR_RIGHT:
          map[character->y][character->x] = PLAYER_CHAR_UP;
          character->direction = PLAYER_CHAR_UP;
          break;
      }
      break;
    /* d -> 90° rotation clockwise*/
    case 'd':
      switch (character->direction)
      {
        case PLAYER_CHAR_UP:
          map[character->y][character->x] = PLAYER_CHAR_RIGHT;
          character->direction = PLAYER_CHAR_RIGHT;
          break;
        case PLAYER_CHAR_RIGHT:
          map[character->y][character->x] = PLAYER_CHAR_DOWN;
          character->direction = PLAYER_CHAR_DOWN;
          break;
        case PLAYER_CHAR_DOWN:
          map[character->y][character->x] = PLAYER_CHAR_LEFT;
          character->direction = PLAYER_CHAR_LEFT;
          break;
        case PLAYER_CHAR_LEFT:
          map[character->y][character->x] = PLAYER_CHAR_UP;
          character->direction = PLAYER_CHAR_UP;
          break;
      }
      break;
    default:
      break;
    }
    display_map (main_window, character, map_height, map_width, map);
  }
}