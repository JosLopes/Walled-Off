#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <math.h>
#include "datatypes.h"
#include "defines.h"
#include "movement.h"
#include "mapgen.h"
#include "MOBs.h"

/*
* a104188 - Ana Cerqueira
* Function that sets the range according to the character life
*/
int sets_range (int life){

  int range;

  if(life>70){
    range = 5;
  }
  else if(life>50 && life<70){
    range = 4;
  }
  else if(life>30 && life<50){
    range = 3;
  }
  else {
    range = 2;
  }
  return range;
}

int euclidean_distance (Point start, Point finish)
{
  /* Calculates the euclidean distance between two points */
  return sqrt(pow(start.y - finish.y, 2) + pow(start.x - finish.x, 2));
}

/* 
*  Explores different rays and verifies a set of conditions to give a restricted vision for
*  the player 
*/
void ray_cast (int map_width, char traveled_path[][map_width], char **map, int row, int col)
{
  int total_circ_rad;  /* Used to limit the radius of the vision circle surrounding the player */
  double increment = M_PI/ (double) 300;
  int destination_row = row;
  double scale;

  /* A static point for the player character */
  Point player;
  player.y = row;
  player.x = col;

  /* A variable point for his vision */
  Point vision;

  /*
    Explores all the angles from -π/2 to π/2, where the cos()
    is positive and the sin() is negative or postive, to find
    enoumerous y = mx equations that represent lines of sight
    to the right side of the character, when seen from the top */
  for (double radius = (double) (-1) * (M_PI/ (double) 2); radius < (M_PI/ (double) 2); radius += increment)
  {
    /* scale to discover the row in realtion to the col */
    scale = tan (radius);

    total_circ_rad = 0;
    if (scale >= 0)
    {
      while (map[row][col] != WALL_CHAR && total_circ_rad < VISION_RANGE)
      {
        if (row < destination_row) row ++;
        else
        {
          col ++;
          /* The location of y in realtion to x     */
          /* Resets the row value to acert the line */
          destination_row = player.y;
          destination_row += scale * (col - player.x);
        }
        traveled_path[row][col] = map[row][col];
        /* Calculates the distance from the player to the current vision point, */
        /* i.e, to the current node that is being uncovered by the vision using */
        /* the Manhattan distance                                               */
        vision.y = row; vision.x = col;
        total_circ_rad = euclidean_distance (player, vision);
      }
    }
    else
    {
      while (map[row][col] != WALL_CHAR && total_circ_rad < VISION_RANGE)
      {
        if (row > destination_row) row --;
        else
        {
          col ++;
          /* The location of y in realtion to x     */
          /* Resets the row value to acert the line */
          destination_row = player.y;
          destination_row += scale * (col - player.x);
        }
        traveled_path[row][col] = map[row][col];
        vision.y = row; vision.x = col;
        total_circ_rad = euclidean_distance (player, vision);
      }
    }
    /* Restes every value */
    destination_row = player.y;
    row = player.y;
    col = player.x;
  }

  /* 
    Explores all the angles from π/2 to 3(π/2), where the cos()
    is negative and the sin() is negative or postive, to find
    enoumerous y = mx equations that represent lines of sight,
    this time for the left side of the main character        */
  for (double radius = (M_PI/ (double) 2); radius < ((double) 3 * (M_PI / (double) 2)); radius += increment)
  {
    /* scale to discover the row in realtion to the col */
    scale = tan (radius);

    total_circ_rad = 0;
    if (scale >= 0)
    {
      while (map[row][col] != WALL_CHAR && total_circ_rad < VISION_RANGE)
      {
        if (row > destination_row) row --;
        else
        {
          col --;
          /* The location of y in realtion to x     */
          /* Resets the row value to acert the line */
          destination_row = player.y;
          destination_row += scale * (col - player.x);
        }
        traveled_path[row][col] = map[row][col];
        vision.y = row; vision.x = col;
        total_circ_rad = euclidean_distance (player, vision);
      }
    }
    else
    {
      while (map[row][col] != WALL_CHAR && total_circ_rad < VISION_RANGE)
      {
        if (row < destination_row) row ++;
        else
        {
          col --;
          /* The location of y in realtion to x     */
          /* Resets the row value to acert the line */
          destination_row = player.y;
          destination_row += scale * (col - player.x);
        }
        traveled_path[row][col] = map[row][col];
        vision.y = row; vision.x = col;
        total_circ_rad = euclidean_distance (player, vision);
      }
    }
    /* Restes every value */
    destination_row = player.y;
    row = player.y;
    col = player.x;
  }

  /* Ensures that the vertical equacion x = 0 is buiilt */
  int reverse = row;  /* Its used to build the downword (upword in game) line */
  total_circ_rad = 0;
  while (map[row][col] != WALL_CHAR && total_circ_rad < VISION_RANGE)
  {
    row ++;
    traveled_path[row][col] = map[row][col];
    total_circ_rad ++;
  }

  total_circ_rad = 0;
  /* Displays the player */
  traveled_path[reverse][col] = map[reverse][col];
  while (map[reverse][col] != WALL_CHAR && total_circ_rad < VISION_RANGE)
  {
    reverse --;
    traveled_path[reverse][col] = map[reverse][col];
    total_circ_rad ++;
  }
}


/*
* a104188 - Ana Cerqueira
*   Function that defines and print the range of vision around
* the character and the different colors that are displayed. 
*   Displays the vision of the character on the screen according
* some restrictions like step up above the enemies or eliminate
* the charactes that define the differents lands.
*   Save the path that the character have discovered already on 
* the array traveled_path so it can be displayed on the screen.
*/
void vision_color (WINDOW *main_window, Character *character, char **map, int map_width ,char traveled_path[][map_width])
{
  int y, x;
  y = character -> y;
  x = character -> x;

  /* add the viwed places to the list traveled_path to make they apper on the screen */
  ray_cast (map_width, traveled_path, map, y, x);

  for (int x = 0; x < MAP_WIDTH; x++)
  {
    for (int y = 0; y < MAP_HEIGHT; y++)
    {
      int dist = sqrt(pow(x - character ->x, 2) + pow(y - character->y, 2));

      /*case character position*/
      if (x == character->x && y == character->y)
      {
        wattron(main_window, COLOR_PAIR(PLAYER_VISION_COLOR1)); 
        mvwaddch(main_window, y, x, traveled_path[y][x]); 
        wattroff(main_window, COLOR_PAIR(PLAYER_VISION_COLOR1));
      }
      else if (traveled_path[y][x] == WALL_CHAR ||
               traveled_path[y][x] == UNDISCOVERED_PATH_CHAR ||
               traveled_path[y][x] == WATER_CHAR)
      {
        map_colors(main_window, map_width, y, x, traveled_path);
      }
      else
      {
        /*sets the colors according to the distance to the character*/
        switch (dist)
        {
          case 5:
            switch (traveled_path[y][x])
            {
            case FLOOR_CHAR:
              wattron(main_window, COLOR_PAIR(PLAYER_VISION_COLOR4)); 
              mvwaddch(main_window, y, x, traveled_path[y][x]); 
              wattroff(main_window, COLOR_PAIR(PLAYER_VISION_COLOR4));
              break;
            default:
              wattron(main_window,COLOR_PAIR(ENEMY_VISION_COLOR4)); 
              mvwaddch(main_window, y, x, traveled_path[y][x]); 
              wattroff(main_window,COLOR_PAIR(ENEMY_VISION_COLOR4));
              break;
            }
            break;

          case 4:
            switch (traveled_path[y][x])
            {
            case FLOOR_CHAR:
              wattron(main_window, COLOR_PAIR(PLAYER_VISION_COLOR3)); 
              mvwaddch(main_window, y, x, traveled_path[y][x]); 
              wattroff(main_window, COLOR_PAIR(PLAYER_VISION_COLOR3));
              break;
            default:
              wattron(main_window,COLOR_PAIR(ENEMY_VISION_COLOR3)); 
              mvwaddch(main_window, y, x, traveled_path[y][x]); 
              wattroff(main_window,COLOR_PAIR(ENEMY_VISION_COLOR3));
              break;
            }
            break;

          case 3:
            switch (traveled_path[y][x])
            {
            case FLOOR_CHAR:
              wattron(main_window, COLOR_PAIR(PLAYER_VISION_COLOR3)); 
              mvwaddch(main_window, y, x, traveled_path[y][x]); 
              wattroff(main_window, COLOR_PAIR(PLAYER_VISION_COLOR3));
              break;
            default:
              wattron(main_window,COLOR_PAIR(ENEMY_VISION_COLOR3)); 
              mvwaddch(main_window, y, x, traveled_path[y][x]); 
              wattroff(main_window,COLOR_PAIR(ENEMY_VISION_COLOR3));
              break;
            }
            break;

          case 2:
            switch (traveled_path[y][x])
            {
            case FLOOR_CHAR:
              wattron(main_window, COLOR_PAIR(PLAYER_VISION_COLOR2)); 
              mvwaddch(main_window, y, x, traveled_path[y][x]); 
              wattroff(main_window, COLOR_PAIR(PLAYER_VISION_COLOR2));
              break;
            default:
              wattron(main_window,COLOR_PAIR(ENEMY_VISION_COLOR2)); 
              mvwaddch(main_window, y, x, traveled_path[y][x]); 
              wattroff(main_window,COLOR_PAIR(ENEMY_VISION_COLOR2));
              break;
            }
            break;

          case 1:
            switch (traveled_path[y][x])
            {
            case FLOOR_CHAR:
              wattron(main_window, COLOR_PAIR(PLAYER_VISION_COLOR1)); 
              mvwaddch(main_window, y, x, traveled_path[y][x]); 
              wattroff(main_window, COLOR_PAIR(PLAYER_VISION_COLOR1));
              break;
            default:
              wattron(main_window,COLOR_PAIR(ENEMY_VISION_COLOR1)); 
              mvwaddch(main_window, y, x, traveled_path[y][x]); 
              wattroff(main_window,COLOR_PAIR(ENEMY_VISION_COLOR1));
              break;
            }
            break;

          default:
            map_colors(main_window, map_width, y, x, traveled_path);
            break;
        }
      }
    }
  }
}
