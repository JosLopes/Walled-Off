#include "datatypes.h"
#ifndef MOBS_H
#define MOBS_H

int locate_positions (int map_heigth, int map_width, char **map, int enemies_size, Enemy enemies[], int nor_size, Non_overlaping_rooms no_overlpg[]);
Variable_stats *d_enemies_variable_stats ();
Variable_stats *s_enemies_variable_stats ();
Variable_stats *g_enemies_variable_stats ();
Tag *init_enemies (int number_of_enemies, Enemy *enemies, Variable_stats *d_variables, Variable_stats *s_variables, Variable_stats *g_variables, char **map);

/* Defines only used for this module porpouse */
#define D_CHAR 'D'
#define D_MAX_XP 15
#define D_MIN_XP 10
#define D_POISON 1
#define D_GROUP_DESIRE 0
#define D_ENEMIES 2  /* Number of distint enteties */
/* Range from the player required to start acting smarter and choose better paths */
#define D_INTEL_RANGE 300

#define S_CHAR 'S'
#define S_MAX_XP 38
#define S_MIN_XP 24
#define S_POISON 1
#define S_GROUP_DESIRE 2
#define S_ENEMIES 2  /* Number of distint enteties */
/* Range from the player required to start acting smarter and choose better paths */
#define S_INTEL_RANGE 700

#define G_CHAR 'G'
#define G_MAX_XP 80
#define G_MIN_XP 60
#define G_POISON 1
#define G_GROUP_DESIRE 4
#define G_ENEMIES 2  /* Number of distint enteties */
/* Range from the player required to start acting smarter and choose better paths */
#define G_INTEL_RANGE 900

#endif
