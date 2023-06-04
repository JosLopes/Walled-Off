

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


 void choose_weapon(Character *character)
{
  int current_weapon_index; /* Armazena o índice da arma escolhida pelo jogador */

  clear();
  printw("Selecione uma arma:\n");
  for (int i = 0; i < 4; i++) /* Usa um loop for para imprimir as três armas disponíveis para o jogador escolher */
  {
    printw("%d - %s\n", i + 1, character->weapons[i].name);
  }
  refresh();

  do /* Recebe um input e verifica se é válido. A variável current_weapon_index é atualizada com o valor de current_weapon_index da struct Character: */
  {
    current_weapon_index = getch() - '0';
  } while (current_weapon_index < 1 || current_weapon_index > 4);

  character->current_weapon_index = current_weapon_index -1 ;
}

float calculate_enemy_damage(Character *character, Awake *is_awake)//mudar o resto das instâncias
{
  float max_hp = character->initial_life;
  float max_xp = character->initial_life * 2;
  float base_damage = (max_hp + max_xp) * 0.00;

  float total_damage = 0.0;

  for (int i = 0; i < is_awake->current_size; i++)
  {
    Enemy *enemy = &(is_awake->enemies_awaken[i]);
    float damage = enemy->damage + base_damage;
    total_damage += damage;
  }

  return total_damage;
}


/*checks if the character has any xp left. If the character has any xp left and the damage taken is greater than the xp, the function deducts
the xp from the damage and subtracts the remaining damage from the character's life attribute.
This means that if the damage is greater than the character's xp, the xp will be completely depleted, and the remaining damage will reduce the character's life.
If the character has xp left and the damage taken is less than or equal to the xp, the function simply deducts the damage from the character's xp.
If the character has no xp left, the function simply subtracts the damage from the character's life attribute.*/

void character_take_damage(Character *character,Enemy *enemy, float total_damage)
{
float distance = sqrt(pow(enemy->x - character->x, 2)+ pow( enemy->y - character->y,2));

  float xp = character->xp;
if (distance <= enemy->range){
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

void enemy_attack(Character *character,Enemy *enemy, Awake *is_awake)
{
  float damage = calculate_enemy_damage(character, is_awake);
  character_take_damage(character, enemy ,damage);
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
    enemy->awake = 0;
  }
}
/*swapp the dead enemy with the last enemy in the is_awake->enemies_awaken array. This is done by copying the contents of the last element
in the array into the position of the dead enemy. The size of the is_awake->enemies_awaken array is then decremented to reflect the removal of the dead enemy.
After that, checks whether the enemies array is not NULL. If it is not NULL, it means that the enemy is also present in the enemies array,
so the function proceeds to remove it from there as well.
To do this, it overwrites the enemy in the enemies array with the last enemy in the array and updates its index to reflect its new position in the array.*/
void remove_dead_enemy(int index, Awake *is_awake, Enemy *enemies, char **map)
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

  /* Remove the enemy from the map */
  int enemyX = is_awake->enemies_awaken[index].x;
  int enemyY = is_awake->enemies_awaken[index].y;
  map[enemyY][enemyX] = FLOOR_CHAR; // Replace with the appropriate empty character or fill it with a special character
}

void enemy_take_damage(Character *character, Enemy *enemy, Awake *is_awake, char **map)
{
  int weapon_damage = character->weapons[character->current_weapon_index].damage;
  float max_xp = character->initial_life * 4;

  enemy->life -= weapon_damage;

  if (enemy->life > 0)
  {
    enemy->awake = 0;
    enemy_attack(character, enemy, is_awake);
  }
  else
  {
    int enemy_index = enemy->index;
    remove_dead_enemy(enemy_index, is_awake, NULL, map);
    character->xp += enemy->tag->xp_from_death;
  }

  if (character->xp >= max_xp)
  {
    character->xp = max_xp;
  }
}



void handle_attack_input(Character *character , Awake *is_awake, char **map)
{
   for (int i = 0; i < is_awake->current_size; i++)
  {
    Enemy *enemy = &(is_awake->enemies_awaken[i]);
int dx = character->x - enemy->x;
  int dy = character->y - enemy->y;
  int distance = sqrt(dx * dx + dy * dy);

    
        
        float weapon_range = character->weapons[(character->current_weapon_index) ].range;

        if (distance <= weapon_range)
        {
            // Check the direction of the character and determine if the enemy is in front
            if (character->direction == PLAYER_CHAR_UP && enemy->y < character->y && enemy->x == character->x)
            {
                enemy_take_damage(character, enemy,is_awake, map);

            }
            else if (character->direction == PLAYER_CHAR_RIGHT && enemy->x > character->x && enemy->y == character->y)
            {
                enemy_take_damage(character,enemy, is_awake, map);
              
            }
            else if (character->direction ==  PLAYER_CHAR_LEFT && enemy->x < character->x && enemy->y == character->y)
            {
                enemy_take_damage(character,enemy,  is_awake, map);
                
            }
            else if (character->direction == PLAYER_CHAR_DOWN && enemy->y > character->y && enemy->x == character->x)
            {
                enemy_take_damage(character,enemy, is_awake, map);
                
            }

            // If enemy's life is above 0, enemy attac,ks character
            if (enemy->life > 0)
            {
                enemy_attack(character, enemy, is_awake);
            }
            else
            {
                printf("Você matou o inimigo e ganhou %d de XP!\n", enemy->tag->xp_from_death);
            }
        }
  }
    
}

void handle_special_power_input(Character *character, Enemy *enemy, char **map)
{
  Weapon *weapon = &(character->weapons[character->current_weapon_index]); // Get the currently selected weapon
  
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
        enemy-> life -=5;
        weapon->special_duration = character->weapons[2].special_duration;

    break;
      
    case Teleport:
{
  int targetX, targetY;
  do {
    // Generate random coordinates within the map range
    targetX = rand() % MAP_WIDTH;
    targetY = rand() % MAP_HEIGHT;
  } while (map[targetY][targetX] != FLOOR_CHAR); // Keep generating until a valid floor position is found

  // Set the previous position to FLOOR_CHAR
  map[character->y][character->x] = FLOOR_CHAR;

  printf("Você foi teleportado para a posição (%d, %d)\n", targetX, targetY);
  character->x = targetX;
  character->y = targetY;

  // Set the new position to the character's direction
  map[targetY][targetX] = character->direction;

  weapon->special_duration = character->weapons[3].special_duration;
}

        break;
      default:
        printf("Poder especial inválido!\n");
        return;
    }

    weapon->turns_left = weapon->special_duration; // Initialize the "turns left" field with the duration of the special power
    character->xp -= character->initial_life * 0.3;
  }
  else
  {
    printf("Você não tem XP suficiente ou a arma não possui poder especial!\n");
  }
}
void update_power(Character *character, Enemy *enemy)
{
  for (int i = 0; i < 4; i++) // Assuming there are three weapons
  {
    Weapon *weapon = &(character->weapons[i]);
    
    if (weapon->turns_left > 0)
    {
      weapon->turns_left--;
      
      if (weapon->turns_left == 0)
      {
        // The special power has reached zero
        printf("O poder especial da arma %s acabou!\n", weapon->name);
        
        switch (weapon->special_type)
        {
          case DamageBoost:
            weapon->damage /= 1.5;
            break;
          case Healing:
            character->life+= 0;
            break;
          case Poison:
          enemy->life+=0;

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

void attack(Character *character, Enemy *enemy, Awake *is_awake, char **map, int ch)
{
 
    enemy_sees_character(character, enemy);
     update_power(character, enemy);
if (ch == 'r'){
   choose_weapon(character);
  handle_attack_input(character, is_awake, map);
}
 if (ch == 'e'){
  
   handle_attack_input(character,is_awake, map);
}
if (ch =='i'){
        handle_special_power_input(character, enemy, map);

}
  
}
