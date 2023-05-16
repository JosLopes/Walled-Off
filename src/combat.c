

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



Weapon *choose_weapon(Character *player)
{
  int weapon_index;/*armazena o índice da arma escolhida pelo jogador*/

  clear();
  printw("Selecione uma arma:\n");
  for (int i = 0; i < 3; i++)/*Usa um loop for para imprimir as três armas disponíveis para o jogador escolher*/
  {
    printw("%d - %s\n", i + 1, player->weapons[i].name);
  }
  refresh();

  do/*recebe um input e verifica se é válida.
   A variável weapon_index é atualizada com o valor numérico do input convertido de ASCII:*/
  {
    weapon_index = getch() - '0';
  } while (weapon_index < 1 || weapon_index > 3);

  return &(player->weapons[weapon_index - 1]);
}

float max(float a, float b)/*calcula o máximo entre dois valores*/
{
  return a > b ? a : b;
}

int calculate_enemy_damage(Character *player, Enemy *enemy)
{
  float max_hp = player->initial_life;
  float max_xp = player->initial_life * 4;
  float damage = enemy->damage;
  float base_damage = (max_hp + max_xp) * 0.08;
  damage += base_damage;

  return damage;
}

/*checks if the character has any xp left. If the character has any xp left and the damage taken is greater than the xp, the function deducts 
the xp from the damage and subtracts the remaining damage from the character's life attribute. 
This means that if the damage is greater than the character's xp, the xp will be completely depleted, and the remaining damage will reduce the character's life.
If the character has xp left and the damage taken is less than or equal to the xp, the function simply deducts the damage from the character's xp.
If the character has no xp left, the function simply subtracts the damage from the character's life attribute.*/


void player_take_damage(Character *player, float damage)
{
  float xp = player->xp;
  if (xp > 0)
  {
    if (damage > xp)
    {
      player->xp = 0;
      player->life -= (damage - xp);
    }
    else
    {
      player->xp -= damage;
    }
  }
  else
  {
    player->life -= damage;
  }
}

void enemy_attack(Character *player, Enemy *enemy)
{
  float damage = calculate_enemy_damage(player, enemy);
  player_take_damage(player, damage);
  if (player->life <= 0)
  {
    // Game over
    printf("Você morreu!\n");
    exit(0);
  }
}
/*the function calculates the horizontal distance between the player and the enemy (dx) and the vertical distance (dy).
 Then calculate the distance between the two characters (distance),If the distance is less than or equal to 5 (which can be thought of as a "sight radius"), 
 then the awake flag of the enemy is set to 1. This means that the enemy is now "awake" and will begin to pursue the player character.*/
void enemy_sees_player(Character *player, Enemy *enemy)
{
  int dx = player->x - enemy->x;
  int dy = player->y - enemy->y;
  int distance = sqrt(dx * dx + dy * dy);

  if (distance <= 5)
  {
    enemy->awake = 1;
  }
}
/*swapp the dead enemy with the last enemy in the is_awake->enemies_awaken array. This is done by copying the contents of the last element 
in the array into the position of the dead enemy. The size of the is_awake->enemies_awaken array is then decremented to reflect the removal of the dead enemy.
After that, checks whether the enemies array is not NULL. If it is not NULL, it means that the enemy is also present in the enemies array, 
so the function proceeds to remove it from there as well. 
To do this, it overwrites the enemy in the enemies array with the last enemy in the array and updates its index to reflect its new position in the array.*/
void remove_dead_enemy(int index, Awake *is_awake, Enemy *enemies)
{
  /* Swap the dead enemy with the last enemy in the array */
  is_awake->enemies_awaken[index] = is_awake->enemies_awaken[is_awake->current_size - 1];
  is_awake->current_size--;
  /* If the enemy is also in the 'enemies' array, remove it from there too */
  if (enemies != NULL)
  {
    enemies[is_awake->enemies_awaken[index].index] = enemies[is_awake->current_size];
    enemies[is_awake->current_size].index = is_awake->enemies_awaken[index].index;
  }
}

void enemy_take_damage(Character *player, Enemy *enemy, Awake *is_awake, Enemy *enemies)
{
  enemy->life -= calculate_enemy_damage(player, enemy);
  if (enemy->life > 0)/*If the enemy's life is greater than 0 after taking damage, it means the enemy is still alive, so its awake flag 
  is set to 1 and it will attack the player by calling enemy_attack function.*/
  {
    enemy->awake = 1;
    enemy_attack(player, enemy);
  }
  else/*If the enemy's life is less than or equal to 0 after taking damage, it means the enemy is dead, so the remove_dead_enemy function is 
  called to remove the enemy from the is_awake array and, if enemies array is not NULL, also remove it from there too. 
  The player's XP is then incremented by the amount of XP gained from killing this enemy (enemy->tag->xp_from_death).*/
  {
    remove_dead_enemy(enemy->index, is_awake, enemies);
    player->xp += enemy->tag->xp_from_death;
  }
}
/*The function first calls the choose_weapon function to get the currently equipped weapon of the player. If the player does not have enough XP to 
activate the special power (which requires at least the initial life value multiplied by 4), a message is printed and the function returns.
 Otherwise, a switch statement is used to identify the type of special power associated with the selected weapon and modify the weapon's damage and special duration fields accordingly.*/
void activate_special_power(Character *player)
{
  Weapon *weapon = choose_weapon(player);
  if (player->xp < player->initial_life * 4)
  {
    printf("Você não tem XP suficiente para usar o poder especial\n");
    return;
  }

  switch (weapon->special_type)
  {
  case DamageBoost:
    printf("Você usou o poder especial de aumento de dano da arma %s!\n", weapon->name);
    weapon->damage *= 1.5;
    weapon->special_duration = 3; 
    break;
  case Fire:
    printf("Você usou o poder especial de fogo da arma %s!\n", weapon->name);
    weapon->damage *= 2;
    weapon->special_duration = 2; 
    break;
  case Poison:
    printf("Você usou o poder especial de veneno da arma %s!\n", weapon->name);
    weapon->damage *= 1.2;
    weapon->special_duration = 4; 
    break;
  default:
    printf("Poder especial inválido!\n");
    return;
  }

  weapon->turns_left = weapon->special_duration; 
  player->xp = 0;
}

void handle_attack_input(Character *player, Enemy *enemy, Awake *is_awake, Enemy *enemies)
{
  char input;
  printf("Pressione ' ' para atacar.\n");
  if (scanf(" %c", &input) != 1)
  {
    printf("Erro ao ler o input.\n");
    return;
  }
  if (input == ' ')
  {
    printf("Você atacou!\n");
    Weapon *weapon = choose_weapon(player);
    float xp = player->xp;
    float max_xp = player->initial_life * 4;
    float xp_gain = (float)(enemy->life * 0.1);

    if (xp + xp_gain >= max_xp)
    {
      xp_gain = max_xp - xp;
      player->xp = max_xp;
    }
    else
    {
      player->xp += xp_gain;
    }

    if (xp >= max_xp && strlen(weapon->special_power) > 0)
    {
      activate_special_power(player);
    }
    /*The function checks the direction of the player and determines whether the enemy is in front of the player. If the enemy is in front, 
    the function calls the enemy_take_damage function to deduct the enemy's life based on the player's attack power. 
    If the enemy's life is still above 0, then the function calls the enemy_attack function to allow the enemy to counter-attack the player.*/
    if (player->direction == '^' && enemy->y < player->y)
    {
      enemy_take_damage(player, enemy, is_awake, enemies);
    }
    else if (player->direction == '>' && enemy->x > player->x)
    {
      enemy_take_damage(player, enemy, is_awake, enemies);
    }
    else if (player->direction == '<' && enemy->x < player->x)
    {
      enemy_take_damage(player, enemy, is_awake, enemies);
    }
    else if (player->direction == 'v' && enemy->y > player->y)
    {
      enemy_take_damage(player, enemy, is_awake, enemies);
    }
    /*If the player's life is reduced to 0 or below, then the game ends, and the function exits. Otherwise, the function calls 
    the player_take_damage function to deduct the player's life based on the enemy's attack power. 
    If the player's life is still above 0, the function continues to check if the enemy's life is reduced to 0 or below. 
    If it is, the function adds the XP gain to the player's XP and prints a message informing the player of the XP gain.*/
    if (player->life <= 0)
    {
      printf("Você morreu!\n");
      exit(0);
    }
    player_take_damage(player, enemy->damage);
    if (enemy->life > 0)
    {
      enemy_attack(player, enemy);
    }
    else
    {
      player->xp += xp_gain;
      printf("Você matou o inimigo e ganhou %g de XP!\n", xp_gain);
    }
  }
}
void handle_special_power_input(Character *player)
{
  char input;
  printf("Pressione 'e' para usar o poder especial.\n");
  if (scanf(" %c", &input) != 1)
  {
    printf("Erro ao ler o input.\n");
    return;
  }
  if (input == 'e')
  {
    Weapon *weapon = choose_weapon(player);

    if (player->xp >= player->initial_life * 4 && strlen(weapon->special_power) > 0)
    {
      switch (weapon->special_type)
      {
      case DamageBoost:
        printf("Você já está com o poder especial de aumento de dano ativo na arma %s!\n", weapon->name);
        break;
      case Fire:
        printf("Você usou o poder especial de fogo da arma %s!\n", weapon->name);
        // futuramente adicionar código para causar dano de fogo adicional aqui
        weapon->turns_left = weapon->special_duration; // inicializa o campo "turnos restantes" com a duração do poder especial
        break;
      case Poison:
        printf("Você usou o poder especial de veneno da arma %s!\n", weapon->name);
        // futuramente adicionar código para envenenar o inimigo aqui
        weapon->turns_left = weapon->special_duration; 
        break;
      default:
        printf("Poder especial inválido!\n");
        return;
      }

      player->xp = 0;
    }
  }
}

void attack(Character *player, Enemy *enemy, Awake *is_awake, Enemy *enemies)
{
  while (1)
  {
    enemy_sees_player(player, enemy);

    if (enemy->awake)
    {
      int range = sets_range(player->life);
      float weapon_range = player->weapons[player->current_weapon_index].range;
      if (weapon_range >= range)
      {
        handle_attack_input(player, enemy, is_awake, enemies);
        handle_special_power_input(player);
      }
    }
  }
}
/*The function first gets the coordinates of the player and their current direction, as well as the damage of the current weapon they are using. 
It then iterates through the array of enemies and for each enemy, it calculates the distance between the enemy and the player.
If the distance is within the "awake range", it checks if the enemy is located in the direction the player is facing.
If the enemy is in the same row or column as the player and in the correct direction, the function calls the player_take_damage function, 
passing in the damage of the player's weapon.
If an enemy is not in the awake range, or not located in the direction the player is facing, the function does not take any action and continues checking the remaining enemies in the array.*/
void check_enemy_direction(Character *player, Enemy *enemies[], int num_enemies, int awake_range)
{
  int px = player->x;
  int py = player->y;
  char direction = player->direction;
  int damage = player->weapons->damage;

  for (int i = 0; i < num_enemies; i++)
  {
    int ex = enemies[i]->x;
    int ey = enemies[i]->y;

    // Check distance between player and enemy
    int distance = sqrt(pow(ex - px, 2) + pow(ey - py, 2));
    if (distance <= awake_range)
    {
      if (direction == '^' && ex == px && ey < py)
      {
        // Enemy is above player
        player_take_damage(player, damage);
        return;
      }
      else if (direction == '>' && ey == py && ex > px)
      {
        // Enemy is to the right of player
        player_take_damage(player, damage);
        return;
      }
      else if (direction == '<' && ey == py && ex < px)
      {
        // Enemy is to the left of player
        player_take_damage(player, damage);
        return;
      }
      else if (direction == 'v' && ex == px && ey > py)
      {
        // Enemy is below player
        player_take_damage(player, damage);
        return;
      }
    }
  }
}
