#include <ncurses.h>
#include "datatypes.h"
#ifndef COMBAT_H
#define COMBAT_h


Weapon* choose_weapon(Character* player);
float max(float a, float b);
int calculate_enemy_damage(Character* player, Enemy* enemy);
void player_take_damage(Character* player, float damage);
void enemy_attack(Character* player, Enemy* enemy);
void enemy_take_damage(Character* player, Enemy* enemy);
void activate_special_power(Character* player);
void handle_attack_input(Character* player, Enemy* enemy);
void handle_special_power_input(Character* player);
void attack(Character* player, Enemy* enemy);
void enemy_sees_player(Character* player, Enemy* enemy);
void check_enemy_direction(Character* player, Enemy* enemies[], int num_enemies);

#endif 
