#include "defines.h"
#include "datatypes.h"
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

void init_character (Character *character)
{
  character->x = 0;
  character->y = 0;
  character->life = 100;
  strcpy(character->name, "Player 1");
  character->xp = 2 * character->life;
  character->initial_life = 100 ;

  strcpy(character->weapons[0].name, "Sword");
  character->weapons[0].damage = 22.5;
  character->weapons[0].range = 5.0;
  strcpy(character->weapons[0].special_power, "Damage_Boost");
  character->weapons[0].special_type = DamageBoost;
  character->weapons[0].special_duration = 10;
  character->weapons[0].turns_left = 0;

  // Weapon 2
  strcpy(character->weapons[1].name, "Calhau");
  character->weapons[1].damage = 15;
  character->weapons[1].range = 1.0;
  strcpy(character->weapons[1].special_power, "Healing");
  character->weapons[1].special_type = Healing;
  character->weapons[1].special_duration = 1;
  character->weapons[1].turns_left = 0;

  // Weapon 3
  strcpy(character->weapons[2].name, "Dagger");
  character->weapons[2].damage = 15;
  character->weapons[2].range = 3.0;
  strcpy(character->weapons[2].special_power, "Poison");
  character->weapons[2].special_type = Poison;
  character->weapons[2].special_duration = 30;
  character->weapons[2].turns_left = 0;

//Weapon
  strcpy(character->weapons[3].name, "Ring");
  character->weapons[3].damage = 0;
  character->weapons[3].range = 5.0;
  strcpy(character->weapons[3].special_power, "Teleport");
  character->weapons[3].special_type = Teleport;
  character->weapons[3].special_duration = 1;
  character->weapons[3].turns_left = 0;
}

void init_ncurses(void) {

  initscr();
  /*initialize ncurses*/
  start_color();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);

  // Check if the terminal supports color
  if (has_colors() == FALSE) {
      endwin();
      printf("Your terminal does not support color\n");
      exit(1);
  }
  /*== Create colors ==*/
  /*colors of menu*/
  init_color(1, 148, 0, 300); /*dark purple background*/
  init_color(2, 300, 0, 551); /*color of the selected option*/
  
  /*colors for water*/
  init_color(5, 500, 700, 1000); /*azul claro*/
  init_color(6, 0, 100, 1000); /*azul escuro*/

  /*colors of range of vision*/
  init_color(55, 125, 0, 250); /*roxo escuro*/
  init_color(56, 514, 296, 799); /*roxo PLAYER_VISION_COLOR1    +perto*/
  init_color(57, 413, 202, 694); /*PLAYER_VISION_COLOR2*/
  init_color(58, 276, 132, 464); /*PLAYER_VISION_COLOR3*/
  init_color(59, 183, 89, 308); /*PLAYER_VISION_COLOR4*/
  //**************************************************************************
  init_color(80, 398, 546, 995); /*water*/

  init_color(60, 0, 0, 312); /*OBSCURE_COLOR -> */
  init_color(61, 0, 1000, 0); /*ENEMY_COLOR -> green*/

  /*== Define color pairs ==*/
  /*for map*/
  init_pair(WATER_COLOR, 6, 5);
  init_pair(FLOOR_COLOR, COLOR_WHITE, 55);
  init_pair(ENEMY_COLOR, 61, 55);
  init_pair(WALL_COLOR, 56, 55);
  init_pair(OBSCURE_COLOR, 60, 60);

  /*range of vision*/
  init_pair(PLAYER_VISION_COLOR1, COLOR_YELLOW, 56);
  init_pair(WATER_VISION_COLOR1, 80, 56);
  init_pair(ENEMY_VISION_COLOR1, 61, 56);

  init_pair(PLAYER_VISION_COLOR2, COLOR_YELLOW, 57);
  init_pair(WATER_VISION_COLOR2, 80, 57);
  init_pair(ENEMY_VISION_COLOR2, 61, 57);

  init_pair(PLAYER_VISION_COLOR3, COLOR_YELLOW, 58);
  init_pair(WATER_VISION_COLOR3, 80, 58);
  init_pair(ENEMY_VISION_COLOR3, 61, 58);

  init_pair(PLAYER_VISION_COLOR4, COLOR_YELLOW, 59);
  init_pair(WATER_VISION_COLOR4, 80, 59);
  init_pair(ENEMY_VISION_COLOR4, 61, 59);
  //**************************************************************************

  /*for menu*/
  init_pair(BACKGROUND_COLOR, COLOR_WHITE, 1);
  init_pair(SELECTED_OPTION_COLOR, COLOR_WHITE, 2);
  init_pair(TITLE_COLOR, 2, 1);

  raw();
  noecho();
  curs_set(FALSE);

  /*Enable the use of colors*/
  use_default_colors();
  curs_set(0);
  noecho();
}
