
#include <panel.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include "datatypes.h"
#include "mapgen.h"
#include "defines.h"

/*  é responsável por preencher o mapa com o caractere do chão (FLOOR_CHAR). 
Ela percorre todas as posições do mapa e verifica se o caractere atual não é o caractere da parede (WALL_CHAR) 
ou o caractere do chão (FLOOR_CHAR). Caso não seja nenhum desses, ela preenche a posição com o caractere %,
indicando que aquele espaço ainda não foi preenchido. */

              
void fillMap(int map_height, int map_width, char map[][map_width]) {
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

/*A função generateRooms recebe como parâmetros o mapa do jogo e uma variável numRooms,
que indica quantas salas devem ser geradas no mapa.*/
int generateRooms(int map_height, int map_width, char map[][map_width], Room rooms[], int numRooms) {
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
        }

        /*Se não houver sobreposição, adiciona a sala ao mapa*/
            for (j = roomY; j < roomY + room_Height; j++) {
                for (k = roomX; k < roomX + room_Width; k++) {
                    map[j][k] = FLOOR_CHAR;
                }
            }

            /*Adiciona a sala à lista de salas */
            rooms[i].x = roomX;
            rooms[i].y = roomY;
            rooms[i].width = room_Width;
            rooms[i].height = room_Height; 
    }
    return number_of_non_overlaping_rooms;
}

void generateCorridors(int map_width, char map[][map_width], Room rooms[], int numRooms, int number_of_non_overlaping_rooms) 
{
  int room_number = 0, bridges_placed = 0, bridge_ind = 0, temp;
  Bridge bridges[number_of_non_overlaping_rooms];
  Vector vector;

  for (; room_number < numRooms && bridges_placed < number_of_non_overlaping_rooms; room_number++)
  {
    if (rooms[room_number].is_overlaping == 0)
    {
      bridges[bridges_placed].startingX = (rooms[room_number].width >> 1) + rooms[room_number].x;
      bridges[bridges_placed].startingY = (rooms[room_number].height >> 1) + rooms[room_number].y;
      bridges_placed++;
    }
  }
  
  for (; bridge_ind < bridges_placed && bridge_ind + 1 < bridges_placed; bridge_ind++)
  {
    vector.startingX = bridges[bridge_ind].startingX;
    int endingX      = bridges[bridge_ind+1].startingX;
    vector.startingY = bridges[bridge_ind].startingY;
    int endingY      = bridges[bridge_ind+1].startingY;
    
    // calcula a direção do corredor
    int directionX = endingX - vector.startingX;
    int directionY = endingY - vector.startingY;
    
    // se o corredor estiver na direção negativa em X, inverte os pontos inicial e final
    if (directionX < 0 && directionY < 0) {
      temp = vector.startingX;
      vector.startingX = endingX;
      endingX = temp;

      temp = vector.startingY;
      vector.startingY = endingY;
      endingY = temp;
    }

    if ((directionX > 0 && directionY > 0) || (directionX < 0 && directionY < 0))
    {
      // percorre o corredor, desenhando o chão (FLOOR_CHAR) em cada posição
      for (; vector.startingX <= endingX; vector.startingX++) {
        map[vector.startingY][vector.startingX] = FLOOR_CHAR;
      }
    
      for (; vector.startingY <= endingY; vector.startingY++) {
        map[vector.startingY][vector.startingX] = FLOOR_CHAR;
      }
    } 
    else if (directionX < 0 && directionY > 0) 
    {
      for (; vector.startingX > endingX; vector.startingX--) {
        map[vector.startingY][vector.startingX] = FLOOR_CHAR;
      }
    
      for (; vector.startingY <= endingY; vector.startingY++) {
        map[vector.startingY][vector.startingX] = FLOOR_CHAR;
      }
    }
    else if (directionX > 0 && directionY < 0) 
    {
      for (; vector.startingX <= endingX; vector.startingX++) {
        map[vector.startingY][vector.startingX] = FLOOR_CHAR;
      }
    
      for (; vector.startingY > endingY; vector.startingY--) {
        map[vector.startingY][vector.startingX] = FLOOR_CHAR;
      }
    }
  }
}


void place_player(int map_height, int map_width, char map[][map_width], Character character) {
   
    do {
        character.x = rand() % (map_height-1) +1;
        character.y = rand() % (map_width-1) +1;
    } while (map[character.y][character.x] != FLOOR_CHAR);
    map[character.y][character.x] = PLAYER_CHAR_UP;
}

void place_enemies(int map_height, int map_width, char map[][map_width], int num_goblins, int num_skeletons, int num_orcs) {
    int i, x, y;
    char symbols[] = {'g', 's', 'o'}; // símbolos para representar os diferentes tipos de inimigos
    srand(time(NULL)); // inicializa a semente do gerador de números aleatórios
    // coloca os goblins
    for (i = 0; i < num_goblins; i++) {
        do {
            x = rand() % map_width;
            y = rand() % map_height;
        } while (map[y][x] != FLOOR_CHAR);
        Enemy goblin = {x, y, symbols[0]};
        map[y][x] = goblin.symbol;
    }
    // coloca os esqueletos
    for (i = 0; i < num_skeletons; i++) {
        do {
            x = rand() % map_width;
            y = rand() % map_height;
        } while (map[y][x] != FLOOR_CHAR);
        Enemy skeleton = {x, y, symbols[1]};
        map[y][x] = skeleton.symbol;
    }
    // coloca os orcs
    for (i = 0; i < num_orcs; i++) {
        do {
            x = rand() % map_width;
            y = rand() % map_height;
        } while (map[y][x] != FLOOR_CHAR);
        Enemy orc = {x, y, symbols[2]};
        map[y][x] = orc.symbol;
    }
}

WINDOW *create_window (int height, int width, int startingX, int startingY) {
    WINDOW *local_window;
    
    local_window = newwin (height, width, startingY, startingX);
    wborder (local_window, '|', '|', '-', '-', '*', '*', '*', '*');

    refresh ();
    wrefresh (local_window);

    return local_window;
}

void display_map (WINDOW *main_window, int map_height, int map_width, char map[][map_width]) {
    int i, j;
    
    for (i=0; i < map_width; i++) {
      for (j=0; j < map_height; j++) {
        mvwprintw(main_window, j, i, "%c", map[j][i]);
      }
    }
    wrefresh (main_window);
}