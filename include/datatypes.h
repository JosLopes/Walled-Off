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

typedef struct tag {
  char inteligence;
  int life;
  int range; /* Range for ranged attacks */
  int xp_from_death; /*XP earned when the player kills the enemy */
  int screaming_range; /* Range from where it can call allies to fight with him*/
  int poison_level; /* Damage given by poison (long duration) */
  int group_desire; /* These type of enemys attack in group, this tells how many enemys */
                    /* In a group need to exist before he comes back to fight           */
} tag;

typedef struct Enemy {
  tag tag; /* dumb, smart or genius (evaluates the enemys inteligence) */
  char name[10];
  char display_symbol; /* Map character representing the enemy */
  int x, y; /* Position on the map */
  int damage; /* Damage given to the player*/
} Enemy;

/* Weapon parameters*/
typedef struct {
  char name[20];
  int damage;
  float range;
} Weapon;

/* Consumables parameters */
typedef struct {
  char name[20];
  int impact;
  int x, y;
} Consumables;

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
