#include <ncurses.h>
#include "datatypes.h"
#ifndef COMBAT_H
#define COMBAT_h

Weapon *choose_weapon(Character *character);
float calculate_enemy_damage(Character *character, Awake *is_awake);
void character_take_damage(Character *character, Enemy *enemy, float total_damage);
void enemy_attack(Character *character, Enemy *enemy, Awake *is_awake);
void enemy_sees_character(Character *character, Enemy *enemy);
void remove_dead_enemy(int index, Awake *is_awake, Enemy *enemies, char **map);
void enemy_take_damage(Character *character, Awake *is_awake, char **map);
void handle_attack_input(Character *character, Enemy *enemy, Awake *is_awake, char **map);
void handle_special_power_input(Character *character);
void update_game(Character *character);
void attack(Character *character, Enemy *enemy, Awake *is_awake, char **map, int ch);



#endif
