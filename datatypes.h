#ifndef MY_HEADER_H
#define MY_HEADER_H

/* A Point in a map */
typedef struct point {
  int x;
  int y;
} Point;

/* Rectangle for use in the random */
/* generator for map rooms         */
typedef struct {
  Point dow_left_point;
  Point up_right_point;
} Rectangle;

/* Circle for use in the random */
/* generator for map rooms      */
typedef struct {
  Point center;
  int radius;
} Circle;

/* Hallway for use in the random generator */
/* to conect diferent rooms                */
typedef struct {
  Point start;
  Point end;
} Hallway;

/* Position of caracter */
typedef struct {
    float x;
    float y;
} Posicao;

/* weapon parameters*/
typedef struct {
    char nome[20];
    int dano;
    float alcance;
} Arma;

/* caracter parameters */
typedef struct {
    char nome[50];
    int vida;
    int xp;
    Arma armas[3];;
    Posicao posicao_atual;
} Personagem;

#endif
