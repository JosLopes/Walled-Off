
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include "datatypes.h"
#include "mapgen.h"
#include "defines.h"
#include "vision.h"
#include <math.h>
#include <string.h>


Weapon* choose_weapon(Character* player) {
  int weapon_index;

  
  clear(); 
  printw("Selecione uma arma:\n");
  for (int i = 0; i < 3; i++) {
    printw("%d - %s\n", i+1, player->weapons[i].name);
  }
  refresh(); 

  
  do {
    weapon_index = getch() - '0'; 
  } while (weapon_index < 1 || weapon_index > 3); 

  
  return &(player->weapons[weapon_index - 1]);
}


float max(float a, float b) {
  return a > b ? a : b;
}

int calculate_enemy_damage(Character* player, Enemy* enemy) {
  float max_hp = player->initial_life;
  float max_xp = player->initial_life * 4;
  float damage = enemy->damage;
  float base_damage = (max_hp + max_xp) * 0.08;
  damage += base_damage;

  return damage;
}

void player_take_damage(Character* player, float damage) {
    float xp = player->xp;
    if (xp > 0) { 
        if (damage > xp) { 
            player->xp = 0;
            player->life -= (damage - xp);
        } else {
            player->xp -= damage;
        }
    } else { 
        player->life -= damage;
    }
}

void enemy_attack(Character* player, Enemy* enemy) {
  float damage = calculate_enemy_damage(player, enemy);
player_take_damage(player, damage);
if (player->life <= 0) {
// Game over
printf("Você morreu!\n");
exit(0);
}
}

void enemy_sees_player(Character* player, Enemy* enemy) {
  int dx = player->x - enemy->x;
  int dy = player->y - enemy->y;
  int distance = sqrt(dx * dx + dy * dy);

  if (distance <= enemy->range) {
    enemy->awake = 1;
  }                           
}



void enemy_take_damage(Character* player, Enemy* enemy) {
  enemy->life -= calculate_enemy_damage(player, enemy);
  if (enemy->life > 0) {
    enemy->awake = 1;
    enemy_attack(player, enemy);
  } else {
    player->xp += (enemy->life)*0.1;
  }
}

void activate_special_power(Character* player) {
  Weapon* weapon = choose_weapon(player);
  if (player->xp < player->initial_life * 4) {
    printf("Você não tem XP suficiente para usar o poder especial\n");
    return;
  }
  printf("Você usou o poder especial da arma %s!\n", weapon->name);
  weapon->damage *= 1.5;
  player->xp = 0;
}

void handle_attack_input(Character* player, Enemy* enemy) {
  char input;
  printf("Pressione ' ' para atacar.\n");
  if (scanf(" %c", &input) != 1) {
    printf("Erro ao ler o input.\n");
    return;
  }
  if (input == ' ') {
    printf("Você atacou!\n");
    Weapon* weapon = choose_weapon(player);
    float xp = player->xp;
    float max_xp = player->initial_life * 4;
    float xp_gain = (float) (enemy->life * 0.1);

    if (xp + xp_gain >= max_xp) {
      xp_gain = max_xp - xp;
      player->xp = max_xp;
    } else {
      player->xp += xp_gain;
    }

    if (xp >= max_xp && strlen(weapon->special_power) > 0) {
      activate_special_power(player);
    }

    if (player->direction == '^' && enemy->y < player->y) {
      enemy_take_damage(player, enemy);
    } else if (player->direction == '>' && enemy->x > player->x) {
      enemy_take_damage(player, enemy);
    } else if (player->direction == '<' && enemy->x < player->x) {
      enemy_take_damage(player, enemy);
    } else if (player->direction == 'v' && enemy->y > player->y) {
      enemy_take_damage(player, enemy);
    }

    if (player->life <= 0) {
      printf("Você morreu!\n");
      exit(0);
    }
    player_take_damage(player, enemy->damage);
    if (enemy->life > 0) {
      enemy_attack(player, enemy);
    } else {
      player->xp += xp_gain;
      printf("Você matou o inimigo e ganhou %g de XP!\n", xp_gain);
    }
  }
}

void handle_special_power_input(Character* player) {
  char input;
  printf("Pressione 'e' para usar o poder especial.\n");
  if (scanf(" %c", &input) != 1) {
    printf("Erro ao ler o input.\n");
    return;
  }
  if (input == 'e') {
    Weapon* weapon = choose_weapon(player);

    if (player->xp >= player->initial_life * 4 && strlen(weapon->special_power) > 0) {
      printf("Você usou o poder especial da arma %s!\n", weapon->name);
      player->xp = 0;
    }
  }
}

void attack(Character* player, Enemy* enemy) {
  while (1) {
    enemy_sees_player(player, enemy);

    if (enemy->awake) {
      handle_attack_input(player, enemy);
      handle_special_power_input(player);
    }
  }
}




void check_enemy_direction(Character* player, Enemy* enemies[], int num_enemies) {
  int px = player->x;
  int py = player->y;
  char direction = player->direction;
  int damage = player->weapons->damage;

  for (int i = 0; i < num_enemies; i++) {
    int ex = enemies[i]->x;
    int ey = enemies[i]->y;

    if (direction == '^' && ex == px && ey < py) {
      // Enemy is above player
      player_take_damage(player, damage);
      return;
    } else if (direction == '>' && ey == py && ex > px) {
      // Enemy is to the right of player
      player_take_damage(player, damage);
      return;
    } else if (direction == '<' && ey == py && ex < px) {
      // Enemy is to the left of player
      player_take_damage(player, damage);
      return;
    } else if (direction == 'v' && ex == px && ey > py) {
      // Enemy is below player
      player_take_damage(player, damage);
      return;
    }
  }
}
