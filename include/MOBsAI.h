#include "datatypes.h"
#ifndef MOBS_AI_H
#define MOBS_AI_H

void init_place_holder_node (Node *place_holder);
void build_path (Awake *is_awaken, Character *character, char **map, Node *place_holder);

#endif