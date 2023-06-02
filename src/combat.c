

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


Weapon *choose_weapon(Character *character)
{
  int current_weapon_index; /* Armazena o índice da arma escolhida pelo jogador */

  clear();
  printw("Selecione uma arma:\n");
  for (int i = 0; i < 3; i++) /* Usa um loop for para imprimir as três armas disponíveis para o jogador escolher */
  {
    printw("%d - %s\n", i + 1, character->weapons[i].name);
  }
  refresh();

  do /* Recebe um input e verifica se é válido. A variável current_weapon_index é atualizada com o valor de current_weapon_index da struct Character: */
  {
    current_weapon_index = getch() - '0';
  } while (current_weapon_index < 1 || current_weapon_index > 3);

  return &(character->weapons[current_weapon_index - 1]);
}

float calculate_enemy_damage(Character *character, Enemy *enemy)
{
  float max_hp = character->initial_life;
  float max_xp = character->initial_life * 4;//alterar isto
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

void character_take_damage(Character *character, float damage)
{
  float xp = character->xp;
  if (xp > 0)
  {
    if (damage > xp)
    {
      character->xp = 0;
      character->life -= (damage - xp);
    }
    else
    {
      character->xp -= damage;
    }
  }
  else
  {
    character->life -= damage;
  }
}

void enemy_attack(Character *character, Enemy *enemy)
{
  float damage = calculate_enemy_damage(character, enemy);
  character_take_damage(character, damage);
  if (character->life <= 0)
  {
    // Game over
    printf("Você morreu!\n");
    exit(0);
  }
}
/*the function calculates the horizontal distance between the character and the enemy (dx) and the vertical distance (dy).
 Then calculate the distance between the two characters (distance),If the distance is less than or equal to 5 (which can be thought of as a "sight radius"),
 then the awake flag of the enemy is set to 1. This means that the enemy is now "awake" and will begin to pursue the character character.*/
void enemy_sees_character(Character *character, Enemy *enemy)
{
  int dx = character->x - enemy->x;
  int dy = character->y - enemy->y;
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

void enemy_take_damage(Character *character, Enemy *enemy, Awake *is_awake, Enemy *enemies)
{
  int weapon_damage = character->weapons[character->current_weapon_index - 1].damage;

  enemy->life -= weapon_damage;

  if (enemy->life > 0) /*If the enemy's life is greater than 0 after taking damage, it means the enemy is still alive, so its awake flag
   is set to 1 and it will attack the character by calling enemy_attack function.*/
  {
    enemy->awake = 1;
    enemy_attack(character, enemy);
  }
  else /*If the enemy's life is less than or equal to 0 after taking damage, it means the enemy is dead, so the remove_dead_enemy function is
   called to remove the enemy from the is_awake array and, if enemies array is not NULL, also remove it from there too.
   The character's XP is then incremented by the amount of XP gained from killing this enemy (enemy->tag->xp_from_death).*/
  {
    remove_dead_enemy(enemy->index, is_awake, enemies);
    character->xp += enemy->tag->xp_from_death;
  }
}
/*The function first calls the choose_weapon function to get the currently equipped weapon of the character. If the character does not have enough XP to
activate the special power (which requires at least the initial life value multiplied by 4), a message is printed and the function returns.
 Otherwise, a switch statement is used to identify the type of special power associated with the selected weapon and modify the weapon's damage and special duration fields accordingly.*/
void activate_special_power(Character *character)
{
  Weapon *weapon = choose_weapon(character);// choose weapon
  if (character->xp < character->initial_life * 2)
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
  character->xp = 0;
}

void handle_attack_input(Character *character, Enemy *enemy, Awake *is_awake, Enemy *enemies)
{
  char input;
  input = getch();

  if (input == 'e')
  {
    Weapon *weapon = choose_weapon(character);
    float xp = character->xp;
    float max_xp = character->initial_life * 4;
    float xp_gain = (float)(enemy->life * 0.1);

    if (xp + xp_gain >= max_xp)
    {
      xp_gain = max_xp - xp;
      character->xp = max_xp;
    }
    else
    {
      character->xp += xp_gain;
    }

    if (xp >= max_xp && strlen(weapon->special_power) > 0)
    {
      activate_special_power(character);
    }
    /*The function checks the direction of the character and determines whether the enemy is in front of the character. If the enemy is in front,
    the function calls the enemy_take_damage function to deduct the enemy's life based on the character's attack power.
    If the enemy's life is still above 0, then the function calls the enemy_attack function to allow the enemy to counter-attack the character.*/
    if (character->direction == '^' && enemy->y < character->y)
    {
      enemy_take_damage(character, enemy, is_awake, enemies);
    }
    else if (character->direction == '>' && enemy->x > character->x)
    {
      enemy_take_damage(character, enemy, is_awake, enemies);
    }
    else if (character->direction == '<' && enemy->x < character->x)
    {
      enemy_take_damage(character, enemy, is_awake, enemies);
    }
    else if (character->direction == 'v' && enemy->y > character->y)
    {
      enemy_take_damage(character, enemy, is_awake, enemies);
    }
    /*If the character's life is reduced to 0 or below, then the game ends, and the function exits. Otherwise, the function calls
    the character_take_damage function to deduct the character's life based on the enemy's attack power.
    If the character's life is still above 0, the function continues to check if the enemy's life is reduced to 0 or below.
    If it is, the function adds the XP gain to the character's XP and prints a message informing the character of the XP gain.*/
    if (character->life <= 0)
    {
      printf("Você morreu!\n");
      exit(0);
    }
    character_take_damage(character, enemy->damage);
    if (enemy->life > 0)
    {
      enemy_attack(character, enemy);
    }
    else
    {
      character->xp += xp_gain;
      printf("Você matou o inimigo e ganhou %g de XP!\n", xp_gain);
    }
  }
}
void handle_special_power_input(Character *character)
{
  char input;

  input = getch();

  if (input == 'i')
  {
    Weapon *weapon = choose_weapon(character);

    if (character->xp >= character->initial_life * 4 && strlen(weapon->special_power) > 0)
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

      character->xp = 0;
    }
  }
}

void attack(Character *character, Enemy *enemy, Awake *is_awake, Enemy *enemies)
{
 
    enemy_sees_character(character, enemy);

    if (enemy->awake)
    {
      int range = sets_range(character->life);
      float weapon_range = character->weapons[character->current_weapon_index - 1].range;
      if (weapon_range <= range)
      {
        handle_attack_input(character, enemy, is_awake, enemies);
        handle_special_power_input(character);
      }
    }
  
}
/*The function first gets the coordinates of the character and their current direction, as well as the damage of the current weapon they are using.
It then iterates through the array of enemies and for each enemy, it calculates the distance between the enemy and the character.
If the distance is within the "awake range", it checks if the enemy is located in the direction the character is facing.
If the enemy is in the same row or column as the character and in the correct direction, the function calls the character_take_damage function,
passing in the damage of the character's weapon.
If an enemy is not in the awake range, or not located in the direction the character is facing, the function does not take any action and continues checking the remaining enemies in the array.*/
void check_enemy_direction(Character *character, Enemy *enemies[], int num_enemies, int awake_range)
{
  int px = character->x;
  int py = character->y;
  char direction = character->direction;
  int damage = character->weapons->damage;

  for (int i = 0; i < num_enemies; i++)
  {
    int ex = enemies[i]->x;
    int ey = enemies[i]->y;

    // Check distance between character and enemy
    int distance = sqrt(pow(ex - px, 2) + pow(ey - py, 2));
    if (distance <= awake_range)
    {
      if (direction == '^' && ex == px && ey < py)
      {
        // Enemy is above character
        character_take_damage(character, damage);
        return;
      }
      else if (direction == '>' && ey == py && ex > px)
      {
        // Enemy is to the right of character
        character_take_damage(character, damage);
        return;
      }
      else if (direction == '<' && ey == py && ex < px)
      {
        // Enemy is to the left of character
        character_take_damage(character, damage);
        return;
      }
      else if (direction == 'v' && ex == px && ey > py)
      {
        // Enemy is below character
        character_take_damage(character, damage);
        return;
      }
    }
  }
}
