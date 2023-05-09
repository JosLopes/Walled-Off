#include "datatypes.h"
#ifndef MOBS_AI_H
#define MOBS_AI_H

void init_queue (Path_queue *path);
void init_origin_node (Character *objective, Node *origin, Enemy *enemy);
void init_place_holder_node (Node *place_holder);
int dequeue (Path_queue *path);
void insert_queue (Path_queue *path, Node node);
Node init_new_node (int new_row, int new_col, Character *objective, Node node, Node *prev);
Node find_path (Character *objective, char **map, Node *place_holder, Path_queue *path);

#endif