#ifndef MOBS_AI_2_H
#define MOBS_AI_2_H

void init_is_awake (int number_of_enemies, Awake *is_awake);
void init_awaken_enemies (Character *character, Enemy *enemies, Awake *is_awake);
void build_path (Awake *is_awake, Character *character, char **map, Node *place_holder, Enemy *enemies);

#endif