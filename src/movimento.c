#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include "datatypes.h"
#include "defines.h"
#include "movement.h"

/* Movement function */
/****************************
 *     Directions:           *
 *         0 = right         *
 *         1 = down          *
 *         2 = left          *
 *         3 = up            *
 *     q -> quit comand      *
 ****************************/

void movement(Character character, WINDOW *main_window){
  char per = 'V';
  int ch, direction = 0;

  while ((ch = getch()) != 'q')
  {
    switch (ch)
    {
    /*Define the movement of the various directions using the arrow keys*/
    case KEY_LEFT:
      if (character.x > 0)
      {
        character.x--;
        direction = 2;
      }
      break;
    case KEY_RIGHT:
      if (character.x < COLS - 1)
      {
        character.x++;
        direction = 0;
      }
      break;
    case KEY_UP:
      if (character.y > 0)
      {
        character.y--;
        direction = 3;
      }
      break;
    case KEY_DOWN:
      if (character.y < LINES - 1)
      {
        character.y++;
        direction = 1;
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
      per = '>';
    }
    else if (direction == 1)
    {
      /*down*/
      per = 'v';
    }
    else if (direction == 2)
    {
      /*left*/
      per = '<';
    }
    else
    {
      /*up*/
      per = '^';
    }
    mvwprintw(main_window, character.y, character.x, "%c", per);
    wrefresh(main_window);
  }
}