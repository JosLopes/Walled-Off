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

/* Position of caracter */
typedef struct {
  float x;
  float y;
} Position;

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
  char name[50];
  int life;
  int xp;
  Weapon weapons[3];
  Position current_position;
} Character;

#endif
