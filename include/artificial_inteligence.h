#ifndef MOBS_AI_H
#define MOBS_AI_H

void init_is_awake (int number_of_enemies, Awake *is_awake);
void init_awaken_enemies (Character *character, Enemy *enemies, Awake *is_awake, char **map_static_obstacles);
void build_path (Consumables *available, Awake *is_awake, Character *character, char **map, char traveled_path[][MAP_WIDTH], char **map_static_obstacles, Node *place_holder, Enemy *enemies);
void awaken_in_order (Awake *is_awake, Character ch, char **map, Node *place_holder);

#endif
