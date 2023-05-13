
#include <panel.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include <math.h>
#include "datatypes.h"
#include "mapgen.h"
#include "defines.h"
#include "vision.h"
#include "MOBs.h"

/*  é responsável por preencher o mapa com o caractere do chão (FLOOR_CHAR). 
Ela percorre todas as posições do mapa e verifica se o caractere atual não é o caractere da parede (WALL_CHAR) 
ou o caractere do chão (FLOOR_CHAR). Caso não seja nenhum desses, ela preenche a posição com o caractere %,
indicando que aquele espaço ainda não foi preenchido. */

              
void fillMap(int map_height, int map_width, char **map) {
    int i, j;

    for (i = 0; i < map_height; i++) {
        for (j = 0; j < map_width; j++) {
          map[i][j] = FIRE_CHAR;
        }
    }

    /*Preenche as bordas do mapa com paredes*/
    for (i = 0; i < map_width; i++) {
        map[0][i] = WALL_CHAR;
        map[map_height - 1][i] = WALL_CHAR;
    }

    for (i = 0; i < map_height; i++) {
        map[i][0] = WALL_CHAR;
        map[i][map_width - 1] = WALL_CHAR;
    }
}

void fillTraveledPath(int map_height, int map_width, char traveled_path[][map_width]) {
    int i, j;

    for (i = 0; i < map_height; i++) {
        for (j = 0; j < map_width; j++) {
          traveled_path[i][j] = UNDISCOVERED_PATH_CHAR;
        }
    }
    /*Preenche as bordas do mapa com paredes*/
    for (i = 0; i < map_width; i++) {
        traveled_path[0][i] = WALL_CHAR;
        traveled_path[map_height - 1][i] = WALL_CHAR;
    }

    for (i = 0; i < map_height; i++) {
        traveled_path[i][0] = WALL_CHAR;
        traveled_path[i][map_width - 1] = WALL_CHAR;
    }
}

/*A função generateRooms recebe como parâmetros o mapa do jogo e uma variável numRooms,
que indica quantas salas devem ser geradas no mapa.*/
int generateRooms(int map_height, int map_width, char **map, Room rooms[], int numRooms) {
    int i, j, k, number_of_non_overlaping_rooms = 0;

    /*O ciclo for cria as salas aleatoriamente. A cada iteração, uma nova sala é gerada. 
    O número de iterações é definido pela variável numRooms.*/
    for (i = 0; i < numRooms; i++) {
        /*Define as dimensões da sala*/
        int room_Width = rand() % (MAX_ROOM_WIDTH - MIN_ROOM_WIDTH + 1) + MIN_ROOM_WIDTH;
        int room_Height = rand() % (MAX_ROOM_HEIGHT - MIN_ROOM_HEIGHT + 1) + MIN_ROOM_HEIGHT;

        /* Escolhe uma posição aleatória para a sala*/
        int roomX = rand() % (map_width - room_Width - 1) + 1;
        int roomY = rand() % (map_height - room_Height - 1) + 1;

        /* Verifica se a sala se sobrepõe a outra sala existente*/
        int overlap = 0;
        for (j = 0; j < i && overlap == 0; j++) {
            if (roomX < rooms[j].x + rooms[j].width &&
                roomX + room_Width > rooms[j].x &&
                roomY < rooms[j].y + rooms[j].height &&
                roomY + room_Height > rooms[j].y) {
                rooms[i].is_overlaping = 1;
                overlap = 1;
            }
        }

        if (overlap == 0){ 
          rooms[i].is_overlaping = 0;
          number_of_non_overlaping_rooms ++;

          /*Se não houver sobreposição, adiciona a sala ao mapa*/
          for (j = roomY; j < roomY + room_Height; j++) {
              for (k = roomX; k < roomX + room_Width; k++) {
                  map[j][k] = FLOOR_CHAR;
                  map[roomY][k] = WALL_CHAR;
                  map[roomY + room_Height][k] = WALL_CHAR;
              }
          map[j][roomX] = WALL_CHAR;
          map[j][roomX + room_Width] = WALL_CHAR;
          }
        } else {
        /*Se não houver sobreposição, adiciona a sala ao mapa*/
          for (j = roomY; j < roomY + room_Height; j++) {
            for (k = roomX; k < roomX + room_Width; k++) {
              map[j][k] = FLOOR_CHAR;
            }
          }
        }

        /* Atualiza colocando aleatóriamente walls */

        /*Adiciona a sala à lista de salas */
        rooms[i].x = roomX;
        rooms[i].y = roomY;
        rooms[i].width = room_Width;
        rooms[i].height = room_Height; 
    }
    return number_of_non_overlaping_rooms;
}

void init_non_overlaping_rooms (Room rooms[], int numRooms, Non_overlaping_rooms no_overlpg[], int nor_size)
{
  int room_number = 0, overlaping_list = 0;

  for (; room_number < numRooms && overlaping_list < nor_size; room_number++)
  {
    if (rooms[room_number].is_overlaping == 0)
    {
      no_overlpg[overlaping_list].midX = (rooms[room_number].width >> 1) + rooms[room_number].x;
      no_overlpg[overlaping_list].midY = (rooms[room_number].height >> 1) + rooms[room_number].y;
      overlaping_list ++;
    }
  }
}

void generateCorridors (char **map, Non_overlaping_rooms *no_overlpg, int nor_size) 
{
  int bridge_ind = 0, temp;
  Vector vector;
  
  for (; bridge_ind + 1 < nor_size; bridge_ind++)
  {
    vector.startingX = no_overlpg[bridge_ind].midX;
    int endingX = no_overlpg[bridge_ind+1].midX;
    vector.startingY = no_overlpg[bridge_ind].midY;
    int endingY = no_overlpg[bridge_ind+1].midY;
    
    // calcula a direção do corredor
    int directionX = endingX - vector.startingX;
    int directionY = endingY - vector.startingY;

    /*se o corredor estiver na direção negativa em X, inverte os pontos inicial e final*/
    if (directionX < 0 && directionY < 0) 
    {
      temp = vector.startingX;
      vector.startingX = endingX;
      endingX = temp;

      temp = vector.startingY;
      vector.startingY = endingY;
      endingY = temp;
    }

    if ((directionX > 0 && directionY > 0) || 
        (directionX < 0 && directionY < 0))
    {
      // percorre o corredor, desenhando o chão (FLOOR_CHAR) em cada posição
      for (; vector.startingX <= endingX; vector.startingX++) {
        if (map[vector.startingY][vector.startingX] == FIRE_CHAR ||
            map[vector.startingY][vector.startingX] == WALL_CHAR)
        {
          map[vector.startingY][vector.startingX] = FLOOR_CHAR;
        }
      }
    
      for (; vector.startingY <= endingY; vector.startingY++) {
        if (map[vector.startingY][vector.startingX] == FIRE_CHAR ||
            map[vector.startingY][vector.startingX] == WALL_CHAR)
        {
          map[vector.startingY][vector.startingX] = FLOOR_CHAR;
        }
      }
    } 
    else if ((directionX < 0 || directionX == 0) && (directionY > 0 || directionY == 0)) 
    {
      for (; vector.startingX > endingX; vector.startingX--) {
        if (map[vector.startingY][vector.startingX] == FIRE_CHAR ||
            map[vector.startingY][vector.startingX] == WALL_CHAR)
        {
        map[vector.startingY][vector.startingX] = FLOOR_CHAR;
        }
      }
    
      for (; vector.startingY <= endingY; vector.startingY++) {
        if (map[vector.startingY][vector.startingX] == FIRE_CHAR ||
            map[vector.startingY][vector.startingX] == WALL_CHAR)
        {
        map[vector.startingY][vector.startingX] = FLOOR_CHAR;
        }
      }
    }
    else if ((directionX > 0 || directionX == 0) && (directionY < 0 || directionY == 0))
    {
      for (; vector.startingX <= endingX; vector.startingX++) {
        if (map[vector.startingY][vector.startingX] == FIRE_CHAR ||
            map[vector.startingY][vector.startingX] == WALL_CHAR)
        {
        map[vector.startingY][vector.startingX] = FLOOR_CHAR;
        }
      }
    
      for (; vector.startingY > endingY; vector.startingY--) {
        if (map[vector.startingY][vector.startingX] == FIRE_CHAR ||
            map[vector.startingY][vector.startingX] == WALL_CHAR)
        {
        map[vector.startingY][vector.startingX] = FLOOR_CHAR;
        }
      }
    }
  }
}

void place_player(Non_overlaping_rooms room, char **map, Character *character) {
    character -> x = room.midX;
    character -> y = room.midY;
    map[character -> y][character -> x] = PLAYER_CHAR_UP;
}


WINDOW *create_window (int height, int width, int startingX, int startingY) {
  WINDOW *local_window;
    
  local_window = newwin (height, width, startingY, startingX);
  wborder (local_window, '|', '|', '-', '-', '*', '*', '*', '*');
  if(has_colors() == TRUE) {
    start_color();

    /*create colors*/
    init_color(55, 125, 0, 250); /*roxo escuro*/

    init_color(56, 514, 296, 799); /*roxo PLAYER_VISION_COLOR1    +perto*/
    init_color(57, 413, 202, 694); /*PLAYER_VISION_COLOR2*/
    init_color(58, 276, 132, 464); /*PLAYER_VISION_COLOR3*/
    init_color(59, 183, 89, 308); /*PLAYER_VISION_COLOR4*/

    init_color(60, 0, 0, 312); /*OBSCURE_COLOR*/

    init_color(61, 0, 1000, 0); /*ENEMY_COLOR*/

    init_color(5, 500, 700, 1000); /*azul claro*/
    init_color(6, 0, 100, 1000); /*azul escuro*/
    /*Define color pairs*/
    init_pair(WATER_COLOR, 6, 5);
    init_pair(PLAYER_VISION_COLOR1, COLOR_YELLOW, 56);
    init_pair(PLAYER_VISION_COLOR2, COLOR_YELLOW, 57);
    init_pair(PLAYER_VISION_COLOR3, COLOR_YELLOW, 58);
    init_pair(PLAYER_VISION_COLOR4, COLOR_YELLOW, 59);
    init_pair(FLOOR_COLOR, COLOR_WHITE, 55);
    init_pair(ENEMY_COLOR, 61, 55);
    init_pair(WALL_COLOR, 56, 55);
    init_pair(OBSCURE_COLOR, 60, 60);
    }

  refresh ();
  wrefresh (local_window);

  return local_window;
}

/****************************************************************
*   Function that defines the colors according to the different
* character that is displaying on the screen.
****************************************************************/
void map_colors (WINDOW *main_window, int map_width, int j, int i, char traveled_path[][map_width]){
  switch (traveled_path[j][i])
  {
  case UNDISCOVERED_PATH_CHAR:
    wattron(main_window,COLOR_PAIR(OBSCURE_COLOR)); 
    mvwaddch(main_window, j, i, traveled_path[j][i]); 
    wattroff(main_window,COLOR_PAIR(OBSCURE_COLOR));
    break;
  case FIRE_CHAR:
    wattron(main_window,COLOR_PAIR(WATER_COLOR)); 
    mvwaddch(main_window, j, i, traveled_path[j][i]); 
    wattroff(main_window,COLOR_PAIR(WATER_COLOR)); 
    break;
  case WALL_CHAR:      
    wattron(main_window,COLOR_PAIR(WALL_COLOR)); 
    mvwaddch(main_window, j, i, traveled_path[j][i]); 
    wattroff(main_window,COLOR_PAIR(WALL_COLOR));
    break;
  case FLOOR_CHAR:
    wattron(main_window,COLOR_PAIR(FLOOR_COLOR)); 
    mvwaddch(main_window, j, i, traveled_path[j][i]); 
    wattroff(main_window,COLOR_PAIR(FLOOR_COLOR));
    break;
  default:
  /*DEFENIR INIMIGOS*/
    wattron(main_window,COLOR_PAIR(ENEMY_COLOR)); 
    mvwaddch(main_window, j, i, traveled_path[j][i]); 
    wattroff(main_window,COLOR_PAIR(ENEMY_COLOR));
    break;
  }
}

/*****************************************************
*   Function that displays the map on the screen
******************************************************/
void display_map (WINDOW *main_window, Character *character, char **map, int map_width, char traveled_path[][map_width]) 
{
  int range = sets_range(character->life);
  int x_min = fmax(character->x - range, 0), x_max = fmin(character->x + range, MAP_WIDTH - 1);
  int y_min = fmax(character->y - range, 0), y_max = fmin(character->y + range, MAP_HEIGHT - 1);
  
  /* Redraw map */
  for (int i = 0; i < MAP_WIDTH; i++) {
    for (int j = 0; j < MAP_HEIGHT; j++) {
      
      /*for the range of vision of the character*/
      if (i >= x_min && j > y_min && i <= x_max && j < y_max)
      {
        vision_color(main_window, character, map, MAP_WIDTH, traveled_path);
      }
      else
      {
        map_colors(main_window, MAP_WIDTH, j, i, traveled_path);
      }
    }
  }
  wrefresh(main_window); /*refresh the window to display changes*/
}