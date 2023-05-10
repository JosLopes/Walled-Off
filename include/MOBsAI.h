#include "datatypes.h"
#ifndef MOBS_AI_H
#define MOBS_AI_H

int subtract_to_max (int x, int y);
int distance_from_objective (Point objective, Point future_start);
void init_queue (Path_queue *path);
void init_origin_node (Point *objective, Point *future_start, Node *origin);
void init_place_holder_node (Node *place_holder);
int dequeue (Path_queue *path);
void insert_queue (Path_queue *path, Node node);
Node init_new_node (int new_row, int new_col, Point *character, Node node, Node *prev);
Node find_path (Point *objective, char **map, Node *place_holder, Path_queue *path);

#endif