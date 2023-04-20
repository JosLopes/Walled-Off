#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
// #include "datatypes.h"

/* Weapon parameters*/
typedef struct
{
  char name[20];
  int damage;
  float range;
} Weapon;
/* Position of caracter */
typedef struct
{
  int x;
  int y;
} Position;
/* Caracter parameters */
typedef struct
{
  char name[50];
  int life;
  int xp;
  Weapon weapons[3];
  Position current_position;
} Character;

Character p = {"warrior", 100, 0, {{"None", 1, 1}, {"None", 1, 1}, {"None", 1, 1}}, {0, 0}};
void vision(int x, int y, char per)
{

  int raio = 3;
  //  Create a 20x20 grid of characters
  int height = 1000, width = 1000;

  init_pair(1, COLOR_WHITE, COLOR_BLACK);
  init_pair(2, COLOR_GREEN, COLOR_BLACK);
  init_pair(3, COLOR_BLUE, COLOR_BLACK);

  /*...Define um mapa vazio...*/
  char grid[height][width];
  // Fill the grid with spaces
  for (int i = 0; i < height; i++)
  {
    for (int j = 0; j < width; j++)
    {
      grid[i][j] = ' ';
    }
  }
  // Check the radius around the center character and turn those cells white
  for (int i = x - raio; i <= x + raio; i++)
  {
    for (int j = y - raio; j <= y + raio; j++)
    {
      if (i == x && j == y)
      {
        grid[i][j] = per;
      }
      else if (i >= 0 && i < height && j >= 0 && j < width)
      {
        if ((i - x) * (i - x) + (j - y) * (j - y) <= raio * raio){
          /*Color isn't working*/
          attron(COLOR_PAIR(3) | A_BOLD);
          bkgdset(COLOR_PAIR(3));
          grid[i][j] = '.';
          attroff(COLOR_PAIR(3) | A_BOLD);
        }
      }
    }
  }
  // Print the grid to the screen
  for (int i = 0; i < height; i++)
  {
    for (int j = 0; j < width; j++)
    {
      mvprintw(i, j, "%c", grid[i][j]);
    }
  }
}

/* Movement function */
/****************************
 *     Direções:             *
 *         0 = direita       *
 *         1 = baixo         *
 *         2 = esquerda      *
 *         3 = cima          *
 *     q -> quit comand      *
 ****************************/
void movement(Character p){
  char per = 'V';
  int ch, x, y, direction = 0;
  x = p.current_position.x;
  y = p.current_position.y;
  while ((ch = getch()) != 'q')
  {
    switch (ch)
    {
    /*Define the movement of the various directions using the arrow keys*/
    case KEY_LEFT:
      if (y > 0)
        y--;
      direction = 2;
      break;
    case KEY_RIGHT:
      if (y < COLS - 1)
        y++;
      direction = 0;
      break;
    case KEY_UP:
      if (x > 0)
        x--;
      direction = 3;
      break;
    case KEY_DOWN:
      if (x < LINES - 1)
        x++;
      direction = 1;
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
    clear();

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
    vision(x, y, per);
    refresh();
    p.current_position.x = x;
    p.current_position.y = y;
  }
}

int main()
{
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  movement(p);
  endwin();

  return 0;
}