#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
/*
TipoArma nenhum = {"Espada longa", 1, 1};
TipoArma gun = {"Gun", 5, 5};
TipoArma grenade = {"Arco curto", 7, 10};

Personagem personagem = {"warrior", 0, 0, {nenhum, nenhum, nenhum}, {0,0}};
*/
void movimento (Personagem) {
      while ((ch = getch()) != 'q') {

        switch(ch) {

            /*Define o movimento das várias direções usando as arrow keys*/
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

            /*Define a rotação da personagem sobre si mesma*/
            case 'a':
                /* a -> rotação 90ª no sentido oposto dos ponteiros do relógio*/
                direction = (direction + 3) % 4;
                break;
            case 'd':
                /* d -> rotação 90ª no sentido dos ponteiros do relógio*/
                direction = (direction + 1) % 4;
                break;
            default:
                break;
        }
        clear();

        if (direction == 0) {
            /*direita*/
            mvprintw(y, x, ">");

        } else if (direction == 1) {
            /*baixo*/
            mvprintw(y, x, "v");

        } else if (direction == 2) {
            /*esquerda*/
            mvprintw(y, x, "<");

        } else {
            /*cima*/
            mvprintw(y, x, "^");
        }
        refresh();
    }
}

int main() {

    int ch, x = 0, y = 0, direction = 0;
    /****************************
    *     Direções:             *
    *         0 = direita       *
    *         1 = baixo         *
    *         2 = esquerda      *
    *         3 = cima          *
    *     q -> quit comand      *
    ****************************/
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    return 0;
}