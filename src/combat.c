

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



/*
* a103999 - Ivo Filipe Mendes Vieira
*This function displays a menu for the player to choose a weapon for the character. It takes input from the
*player and updates the character's current weapon based on the chosen index.
*/

void choose_weapon(Character *character)
{
  int current_weapon_index; 

  clear();
  printw("Selecione uma arma:\n");
  for (int i = 0; i < 4; i++) 
  {
    printw("%d - %s\n", i + 1, character->weapons[i].name);
  }
  refresh();

  do 
  {
    current_weapon_index = getch() - '0';
  } while (current_weapon_index < 1 || current_weapon_index > 4);

  character->current_weapon_index = current_weapon_index - 1;
}
/*
* a103999 - Ivo Filipe Mendes Vieira
*This function calculates the damage inflicted by an enemy based on the character's maximum health points (hp),
*maximum experience points (xp), and a base damage formula.
*/
float calculate_enemy_damage(Character *character, Enemy *enemy) // mudar o resto das instâncias
{
  float max_hp = character->initial_life;
  float max_xp = character->initial_life * 2;
  float base_damage = (max_hp + max_xp) * 0.04;

  float total_damage = 0.0;

  float damage = enemy->damage + base_damage;
  total_damage += damage;

  return total_damage;
}

/*
* a103999 - Ivo Filipe Mendes Vieira
*This function calculates the damage taken by the character based on the total damage inflicted by an enemy.
*It considers the character's xp and subtracts it from the damage if it is greater than zero.
*Otherwise, it subtracts the damage directly from the character's life.
*/

void character_take_damage(Character *character, Enemy *enemy, float total_damage)
{
  float distance = sqrt(pow(enemy->x - character->x, 2) + pow(enemy->y - character->y, 2));

  float xp = character->xp;
  if (distance <= enemy->range)
  {
    if (xp > 0)
    {
      if (total_damage > xp)
      {
        character->xp = 0;
        character->life -= (total_damage - xp);
      }
      else
      {
        character->xp -= total_damage;
      }
    }
    else
    {
      character->life -= total_damage;
    }
  }
}
/*
* a103999 - Ivo Filipe Mendes Vieira
*This function initiates an enemy attack by calculating the enemy's damage and calling character_take_damage()
*to apply the damage to the character. If the character's life reaches zero or below, the game ends.
*/
void enemy_attack(Character *character, Enemy *enemy)
{
  float damage = calculate_enemy_damage(character, enemy);
  character_take_damage(character, enemy, damage);
  if (character->life <= 0)
  {
    // Game over
    printf("Você morreu!\n");
    exit(0);
  }
}

/*
*This function removes a dead enemy from the game. It replaces the enemy's position on the map with a floor
*character, shifts the remaining enemies in the array to fill the gap,
*and decrements the current size of the enemies array.
*/
void remove_dead_enemy(Awake *is_awake, int index, char **map)
{
  
  map[is_awake->enemies_awaken[index].y][is_awake->enemies_awaken[index].x] = FLOOR_CHAR;

 
  for (int i = index; i < is_awake->current_size - 1; i++)
  {
    is_awake->enemies_awaken[i] = is_awake->enemies_awaken[i + 1];
  }

  
  is_awake->current_size--;
}
/*
* a103999 - Ivo Filipe Mendes Vieira
*This function applies damage to an enemy based on the character's current weapon's damage. 
*If the enemy's life reaches zero, it calls remove_dead_enemy() and increments the character's xp.
*/
void enemy_take_damage(Character *character, Enemy *enemy, Awake *is_awake, char **map)
{
  int weapon_damage = character->weapons[character->current_weapon_index].damage;
  float max_xp = character->initial_life * 2;

  enemy->life -= weapon_damage;

  if (enemy->life > 0)
  {
    enemy->awake = 0;
  }
  else
  {
    int enemy_index = enemy->index;
    remove_dead_enemy(is_awake, enemy_index, map);
    character->xp += enemy->tag->xp_from_death;
  }

  if (character->xp >= max_xp)
  {
    character->xp = max_xp;
  }
}
/*
* a103999 - Ivo Filipe Mendes Vieira
*This function handles the player's attack input. It checks if an enemy is within range and calls 
*enemy_take_damage() to attack the enemy. If the enemy's life is still above zero, 
*it calls enemy_attack() to initiate the enemy's counterattack.
*/
void handle_attack_input(Character *character, Awake *is_awake, char **map)
{
  for (int i = 0; i < is_awake->current_size; i++)
  {
    Enemy *enemy = &(is_awake->enemies_awaken[i]);
    int dx = character->x - enemy->x;
    int dy = character->y - enemy->y;
    int distance = sqrt(dx * dx + dy * dy);

    float weapon_range = character->weapons[(character->current_weapon_index)].range;

    if (distance <= weapon_range)
    {
      
      if (character->direction == PLAYER_CHAR_UP && enemy->y < character->y && enemy->x == character->x)
      {
        enemy_take_damage(character, enemy, is_awake, map);
      }
      else if (character->direction == PLAYER_CHAR_RIGHT && enemy->x > character->x && enemy->y == character->y)
      {
        enemy_take_damage(character, enemy, is_awake, map);
      }
      else if (character->direction == PLAYER_CHAR_LEFT && enemy->x < character->x && enemy->y == character->y)
      {
        enemy_take_damage(character, enemy, is_awake, map);
      }
      else if (character->direction == PLAYER_CHAR_DOWN && enemy->y > character->y && enemy->x == character->x)
      {
        enemy_take_damage(character, enemy, is_awake, map);
      }

      
      if (enemy->life > 0)
      {
        enemy_attack(character, enemy);
      }
      else
      {
        printf("Você matou o inimigo e ganhou %d de XP!\n", enemy->tag->xp_from_death);
      }
    }
  }
}
/*
*This function handles the player's input to use a special power associated with the current weapon. 
*It checks if the player has enough xp and the weapon has a special power. If conditions are met, 
*it applies the special power, such as increasing damage, healing the character, poisoning the enemy, or teleporting the character.
*/
void handle_special_power_input(Character *character, Enemy *enemy, char **map)
{
  Weapon *weapon = &(character->weapons[character->current_weapon_index]); 

  if (character->xp >= character->initial_life * 0.3 && strlen(weapon->special_power) > 0)
  {
    if (weapon->turns_left > 0)
    {
      printf("Você já está com o poder especial ativo na arma %s!\n", weapon->name);
      return;
    }

    printf("Você usou o poder especial da arma %s!\n", weapon->name);

    switch (weapon->special_type)
    {
    case DamageBoost:
      printf("O dano da arma foi aumentado em 1.5 vezes!\n");
      weapon->damage *= 1.5;
      weapon->special_duration = character->weapons[0].special_duration;
      break;
    case Healing:
      printf("Vida boa!\n");
      int healingAmount = character->initial_life * 0.5;
      int maxLife = character->initial_life;
      if (character->life + healingAmount > maxLife)
      {
        healingAmount = maxLife - character->life;
      }
      character->life += healingAmount;
      printf("Você foi curado em %d pontos de vida!\n", healingAmount);
      weapon->special_duration = character->weapons[1].special_duration;
      break;
    case Poison:
      printf("O inimigo foi envenenado e sofrerá dano a cada turno!\n");
      enemy->life -= 15;
      weapon->special_duration = character->weapons[2].special_duration;

      break;

    case Teleport:
    {
      int targetX, targetY;
      map[character->y][character->x] = FLOOR_CHAR;
      do
      {
        
        targetX = rand() % MAP_WIDTH;
        targetY = rand() % MAP_HEIGHT;
      } while (map[targetY][targetX] != FLOOR_CHAR); 

      printf("Você foi teleportado para a posição (%d, %d)\n", targetX, targetY);
      character->x = targetX;
      character->y = targetY;

      
      map[targetY][targetX] = character->direction;

      weapon->special_duration = character->weapons[3].special_duration;
    }

    break;
    default:
      printf("Poder especial inválido!\n");
      return;
    }

    weapon->turns_left = weapon->special_duration; 
    character->xp -= character->initial_life * 0.3;
  }
  else
  {
    printf("Você não tem XP suficiente ou a arma não possui poder especial!\n");
  }
}

/*
* a103999 - Ivo Filipe Mendes Vieira
*This function updates the duration of any active special powers of the character's weapons. 
*If a special power's duration reaches zero, it reverts the associated effects.
*/
void update_power(Character *character, Enemy *enemy)
{
  for (int i = 0; i < 4; i++) 
  {
    Weapon *weapon = &(character->weapons[i]);

    if (weapon->turns_left > 0)
    {
      weapon->turns_left--;

      if (weapon->turns_left == 0)
      {
        
        printf("O poder especial da arma %s acabou!\n", weapon->name);

        switch (weapon->special_type)
        {
        case DamageBoost:
          weapon->damage /= 1.5;
          break;
        case Healing:
          character->life += 0;
          break;
        case Poison:
          enemy->life += 0;

          break;
        case Teleport:

          break;
        default:
          break;
        }
      }
    }
  }
}
/*
* a103999 - Ivo Filipe Mendes Vieira
*This function handles the overall attack process. It calls update_power() to update active special powers, 
*and based on the player's input (ch), it calls other functions such as choose_weapon(),
*handle_attack_input(), or handle_special_power_input().
*/
void attack(Character *character, Enemy *enemy, Awake *is_awake, char **map, int ch)
{

  update_power(character, enemy);
  if (ch == 'r')
  {
    choose_weapon(character);
    handle_attack_input(character, is_awake, map);
  }
  if (ch == 'e')
  {

    handle_attack_input(character, is_awake, map);
  }
  if (ch == 'i')
  {
    handle_special_power_input(character, enemy, map);
  }
}
