#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
/*
Weapon nenhum = {"None", 1, 1};
Weapon gun = {"Gun", 5, 5};
Weapon grenade = {"Grenade", 7, 10};

Character character = {"warrior", 100, 0, {{"None", 1, 1}, {"None", 1, 1}, {"None", 1, 1 }}, {0,0}};
*/
    /****************************
    *     Direções:             *
    *         0 = direita       *
    *         1 = baixo         *
    *         2 = esquerda      *
    *         3 = cima          *
    *     q -> quit comand      *
    ****************************/

void movement (Character p) {

  int ch, x, y, direction = 0;
  x = p.current_position.x;
  y = p.current_position.y;
  while ((ch = getch()) != 'q') {
    switch(ch) {
      /*Define the movement of the various directions using the arrow keys*/
      case KEY_LEFT:
          if (x > 0) x--;
          direction = 2;
          break;
      case KEY_RIGHT:
          if (x < COLS-1) x++;
          direction = 0;
          break;
      case KEY_UP:
          if (y > 0) y--;
          direction = 3;
          break;
      case KEY_DOWN:
          if (y < LINES-1) y++;
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

    if (direction == 0) {
        /*right*/
        mvprintw(y, x, ">");

    } else if (direction == 1) {
        /*down*/
        mvprintw(y, x, "v");

    } else if (direction == 2) {
        /*left*/
        mvprintw(y, x, "<");

    } else {
        /*up*/
        mvprintw(y, x, "^");
    }
    refresh();
    p.current_position.x = x;
    p.current_position.y = y;
  }
}

int main() {
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  movement(character);
  endwin();

  return 0;
}