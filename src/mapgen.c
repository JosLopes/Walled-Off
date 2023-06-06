
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

/*  
 * a103999 - Ivo Filipe Mendes Vieira
 * This function is responsible for filling the map with the floor character (FLOOR_CHAR).
 * It iterates through all positions of the map and checks if the current character is not
 * the wall character (WALL_CHAR) or the floor character (FLOOR_CHAR). If it is neither of
 * those, it fills the position with the character '%', indicating that the space has not
 * been filled yet.
 */           
void fillMap(int map_height, int map_width, char **map) {
    int i, j;

    for (i = 0; i < map_height; i++) {
        for (j = 0; j < map_width; j++) {
          map[i][j] = WATER_CHAR;
        }
    }

    /*Fills the borders of the map with walls*/
    for (i = 0; i < map_width; i++) {
        map[0][i] = WALL_CHAR;
        map[map_height - 1][i] = WALL_CHAR;
    }

    for (i = 0; i < map_height; i++) {
        map[i][0] = WALL_CHAR;
        map[i][map_width - 1] = WALL_CHAR;
    }
}

/*
 * a103999 - Ivo Filipe Mendes Vieira
 * This function is responsible for filling the traveled path map with the undiscovered path
 * character (UNDISCOVERED_PATH_CHAR). It iterates through all positions of the map and sets
 * the character to UNDISCOVERED_PATH_CHAR, indicating that the path at that position has not
 * been traversed yet. The function also fills the borders of the map with walls.
 */
void fillTraveledPath(int map_height, int map_width, char traveled_path[][map_width]) {
    int i, j;

    for (i = 0; i < map_height; i++) {
        for (j = 0; j < map_width; j++) {
          traveled_path[i][j] = UNDISCOVERED_PATH_CHAR;
        }
    }
    
    for (i = 0; i < map_width; i++) {
        traveled_path[0][i] = WALL_CHAR;
        traveled_path[map_height - 1][i] = WALL_CHAR;
    }

    for (i = 0; i < map_height; i++) {
        traveled_path[i][0] = WALL_CHAR;
        traveled_path[i][map_width - 1] = WALL_CHAR;
    }
}

/**
 * a103999 - Ivo Filipe Mendes Vieira
 * a104541 - José António Fernandes Alves Lopes
 * This function generates rooms on the game map based on the given parameters. 
 * It returns the number of non-overlapping rooms generated.
 */
int generateRooms(int map_height, int map_width, char **map, Room rooms[], int numRooms) {
    int i, j, k, number_of_non_overlapping_rooms = 0;

    /* Iterates to create rooms randomly. A new room is generated in each iteration.
       The number of iterations is determined by the variable numRooms. */
    for (i = 0; i < numRooms; i++) {
        /* Define the dimensions of the room */
        int room_Width = rand() % (MAX_ROOM_WIDTH - MIN_ROOM_WIDTH + 1) + MIN_ROOM_WIDTH;
        int room_Height = rand() % (MAX_ROOM_HEIGHT - MIN_ROOM_HEIGHT + 1) + MIN_ROOM_HEIGHT;

        /* Choose a random position for the room */
        int roomX = rand() % (map_width - room_Width - 1) + 1;
        int roomY = rand() % (map_height - room_Height - 1) + 1;

        /* Check if the room overlaps with any existing rooms */
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

        if (overlap == 0) {
            rooms[i].is_overlaping = 0;
            number_of_non_overlapping_rooms++;

            /* If there is no overlap, add the room to the map */
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
            /* If there is overlap, add the room to the map without walls */
            for (j = roomY; j < roomY + room_Height; j++) {
                for (k = roomX; k < roomX + room_Width; k++) {
                    map[j][k] = FLOOR_CHAR;
                }
            }
        }

        /* Add the room to the rooms list */
        rooms[i].x = roomX;
        rooms[i].y = roomY;
        rooms[i].width = room_Width;
        rooms[i].height = room_Height;
    }
    return number_of_non_overlapping_rooms;
}

/**
 * a104541 - José António Fernandes Alves Lopes
 * a103999 - Ivo Filipe Mendes Vieira
 * This function initializes the list of non-overlapping rooms by extracting the midpoint coordinates of
 * each non-overlapping room from the generated rooms array.
 * The extracted coordinates are stored in the no_overlpg array.
 */
void init_non_overlaping_rooms (Room rooms[], int numRooms, Non_overlaping_rooms no_overlpg[], int nor_size) {
  int room_number = 0, overlapping_list = 0;

  for (; room_number < numRooms && overlapping_list < nor_size; room_number++) {
    if (rooms[room_number].is_overlaping == 0) {
      no_overlpg[overlapping_list].midX = (rooms[room_number].width >> 1) + rooms[room_number].x;
      no_overlpg[overlapping_list].midY = (rooms[room_number].height >> 1) + rooms[room_number].y;
      overlapping_list++;
    }
  }
}

/**
 * a104541 - José António Fernandes Alves Lopes
 * This function generates corridors between non-overlapping rooms on the game map. It takes the map, 
 * the array of non-overlapping rooms, and the number of non-overlapping rooms as parameters. 
 */
void generateCorridors(char **map, Non_overlaping_rooms *no_overlpg, int nor_size) {
  int bridge_ind = 0, temp;
  Vector vector;

  for (; bridge_ind + 1 < nor_size; bridge_ind++) {
    vector.startingX = no_overlpg[bridge_ind].midX;
    int endingX = no_overlpg[bridge_ind + 1].midX;
    vector.startingY = no_overlpg[bridge_ind].midY;
    int endingY = no_overlpg[bridge_ind + 1].midY;

    // Calculate the direction of the corridor
    int directionX = endingX - vector.startingX;
    int directionY = endingY - vector.startingY;

    /* If the corridor is in the negative X direction, reverse the starting and ending points */
    if (directionX < 0 && directionY < 0) {
      temp = vector.startingX;
      vector.startingX = endingX;
      endingX = temp;

      temp = vector.startingY;
      vector.startingY = endingY;
      endingY = temp;
    }

    if ((directionX > 0 && directionY > 0) || (directionX < 0 && directionY < 0)) {
      // Traverse the corridor, drawing the floor (FLOOR_CHAR) at each position
      for (; vector.startingX <= endingX; vector.startingX++) {
        if (map[vector.startingY][vector.startingX] == WATER_CHAR ||
            map[vector.startingY][vector.startingX] == WALL_CHAR) {
          map[vector.startingY][vector.startingX] = FLOOR_CHAR;
        }
      }

      for (; vector.startingY <= endingY; vector.startingY++) {
        if (map[vector.startingY][vector.startingX] == WATER_CHAR ||
            map[vector.startingY][vector.startingX] == WALL_CHAR) {
          map[vector.startingY][vector.startingX] = FLOOR_CHAR;
        }
      }
    } else if ((directionX < 0 || directionX == 0) && (directionY > 0 || directionY == 0)) {
      for (; vector.startingX > endingX; vector.startingX--) {
        if (map[vector.startingY][vector.startingX] == WATER_CHAR ||
            map[vector.startingY][vector.startingX] == WALL_CHAR) {
          map[vector.startingY][vector.startingX] = FLOOR_CHAR;
        }
      }

      for (; vector.startingY <= endingY; vector.startingY++) {
        if (map[vector.startingY][vector.startingX] == WATER_CHAR ||
            map[vector.startingY][vector.startingX] == WALL_CHAR) {
          map[vector.startingY][vector.startingX] = FLOOR_CHAR;
        }
      }
    } else if ((directionX > 0 || directionX == 0) && (directionY < 0 || directionY == 0)) {
      for (; vector.startingX <= endingX; vector.startingX++) {
        if (map[vector.startingY][vector.startingX] == WATER_CHAR ||
            map[vector.startingY][vector.startingX] == WALL_CHAR) {
          map[vector.startingY][vector.startingX] = FLOOR_CHAR;
        }
      }

      for (; vector.startingY > endingY; vector.startingY--) {
        if (map[vector.startingY][vector.startingX] == WATER_CHAR ||
            map[vector.startingY][vector.startingX] == WALL_CHAR) {
          map[vector.startingY][vector.startingX] = FLOOR_CHAR;
        }
      }
    }
  }
}

/**
 * a104541 - José António Fernandes Alves Lopes
 * Places the player on the map
 */
void place_player(Non_overlaping_rooms room, char **map, Character *character) {
    character -> x = room.midX;
    character -> y = room.midY;
    map[character -> y][character -> x] = PLAYER_CHAR_UP;
}

WINDOW *create_window (int height, int width, int startingX, int startingY) {
  WINDOW *local_window;
    
  local_window = newwin (height, width, startingY, startingX);
  wborder (local_window, '|', '|', '-', '-', '*', '*', '*', '*');

  refresh ();
  wrefresh (local_window);

  return local_window;
}

/**
 * a104188 - Ana Cerqueira
 * Function that defines the colors according to the different
 * character that is displaying on the screen.
 */
void map_colors (WINDOW *main_window, int map_width, int j, int i, char traveled_path[][map_width]){
  switch (traveled_path[j][i])
  {
  case UNDISCOVERED_PATH_CHAR:
    wattron(main_window,COLOR_PAIR(OBSCURE_COLOR)); 
    mvwaddch(main_window, j, i, traveled_path[j][i]); 
    wattroff(main_window,COLOR_PAIR(OBSCURE_COLOR));
    break;
  case WATER_CHAR:
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

/**
 * a104188 - Ana Cerqueira
 * Function that displays the map on the screen
 */
void display_map (WINDOW *main_window, Character *character, char **map, int map_width, char traveled_path[][map_width]) 
{
  //int range = sets_range(character->life);
  int range = 5;
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
