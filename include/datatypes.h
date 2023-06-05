#ifndef MY_HEADER_H
#define MY_HEADER_H

typedef struct {
  int y;
  int x;
} Point;

typedef struct {
    int x, y;
    int width, height;
    int is_overlaping;
} Room;

typedef struct
{
  int midX;
  int midY;
} Non_overlaping_rooms;

typedef struct
{
  int startingX;
  int startingY;
} Vector;


/* Consumables parameters */
typedef struct {
  char identify;
  char name[20];
  char *instruction[1]; /* For display in the bottom panel */
  int impact_life;
  int impact_xp;
  int x, y;
} Consumables;

enum SpecialPowerType {
  DamageBoost,
  Poison,
  Healing,
  Teleport
};

typedef struct {
  char name[20];
  int damage;
  float range;
  char special_power[50];
  enum SpecialPowerType special_type;
  int special_duration; // duração do poder especial em turnos
  int turns_left;
} Weapon;



typedef struct {
  int x, y;
  char direction;
  char name[50];
  float life;
  float initial_life;
  float xp;
  Weapon weapons[3];
  char weapon_special_powers[3][50];
  int current_weapon_index; // novo campo adicionado
} Character;


/* Set of predefined atributes */
typedef struct {
  char inteligence;  /* Initial d(umb), s(mart) or g(enius), also used for display */
  int xp_from_death;  /*XP earned when the player kills the enemy */
  int poison_level;  /* Damage given by poison (long duration) */
  int group_desire;  /* These type of enemys attack in group, this tells how many enemys */
                     /* In a group need to exist before he comes back to fight           */
} Tag;

typedef struct {
  Tag *tag;  /* dumb, smart or genius (evaluates the enemys inteligence) */
  char display;
  char *name[1];
  char *instruction[1]; /* For display in the bottom panel */
  int x, y;  /* Position on the map */
  int life;
  int range;  /* Range for ranged attacks */
  int damage;  /* Damage given to the player*/
  int awake; /* If true, actevily pursues the main character */
  int index;
} Enemy;

typedef struct {
  char display;
  char *name[1];
  char *instruction[1]; /* For display in the bottom panel */
  int life;
  int range;
  int damage;
} Variable_stats;

/*========================================= AI-DATATYPES ========================================*/
/* Node's structure */
typedef struct Path_finder_node {
  int row, col;  /* Position on the map */
  int h, g, f;  /* Integer values for h cost (distance from the goal),
                  for g cost (distance from the origin) and f-cost,
                  the total distance (h cost + g cost)*/
  int explored; /* True or false */
  struct Path_finder_node *prev;  /* Pointer to the prev of this node */
} Node;

/* A queue to store every possible path unexplored */
typedef struct Path_queue {
  Node *nodes;
  int head, tail;  /* Head and tail of the above array */
  int number_of_nodes;
} Path_queue;

typedef struct Awake {
  Enemy *enemies_awaken;
  int total_size;  /* Size of the array awaken (number of total enemies) */
  int current_size;  /* Current number of enemies in the array 'awaken' */
} Awake;

#endif
