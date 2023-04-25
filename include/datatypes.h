#ifndef MY_HEADER_H
#define MY_HEADER_H

typedef struct {
    int x, y;
    int width, height;
    int is_overlaping;
} Room;

typedef struct
{
  int startingX;
  int startingY;
} Bridge;

typedef struct
{
  int startingX, startingY;
  int pathX, pathY;
} Vector;


typedef struct {
    int x, y;
    char symbol;
} Enemy;

/* Weapon parameters*/
typedef struct {
  char name[20];
  int damage;
  float range;
} Weapon;

/* Caracter parameters */
typedef struct {
  int x, y;
  char direction;
  char name[50];
  int life;
  int xp;
  Weapon weapons[3];
} Character;

#endif
