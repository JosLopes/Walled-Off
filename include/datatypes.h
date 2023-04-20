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
} Position;

/* Weapon parameters*/
typedef struct {
  char name[20];
  int damage;
  float range;
} Weapon;

Weapon none = {"None", 1, 1};
Weapon gun = {"Gun", 5, 5};
Weapon grenade = {"Grenade", 7, 10};

/* Caracter parameters */
typedef struct {
  char name[50];
  int life;
  int xp;
  Weapon weapons[3];;
  Position current_position;
} Character;

#endif
