#include "mapgen.h"
#include "datatypes.h"
#include "defines.h"
#include "mapgen.h"
#include "movement.h"
#include "vision.h"
#include <ncurses.h>
#include <time.h>
#include <stdlib.h>

void init_character(Character *character)
{
  character -> x = 0;
  character -> y = 0;
}

int main () {


  WINDOW *main_window;
  char map[MAP_HEIGHT][MAP_WIDTH];
  Room rooms[MAX_ROOMS];
  int number_of_non_overlaping_rooms;
  srand(time(NULL));
  Character character;
  int numRooms = rand() % (MAX_ROOMS - 35) + 10;
  int num_goblins = rand() % 10;
  int num_orcs = rand() % 10;
  int num_skeletons = rand() % 10;


  /*initializes the curses library and sets up terminal I/O*/
  initscr();
  raw ();
  noecho ();
  curs_set(FALSE); /*Hides the cursor*/                                                                                                                                                  ////////////////
  
  if (has_colors() == FALSE) {
        endwin();
        printf("Your terminal does not support color\n");
        exit(1);
  }
  start_color();
  /*init_pair(index, foreground, background);*/
  init_pair(WATER_COLOR, COLOR_CYAN, COLOR_BLUE); 
  init_pair(PLAYER_VISION_COLOR, COLOR_YELLOW ,COLOR_RED);
  init_pair(FLOOR_COLOR, COLOR_WHITE, COLOR_BLACK);

      // Enable the use of colors
  use_default_colors();
  curs_set(0);
  noecho();
  clear();
  refresh ();

  /* Create main window */
  main_window = create_window (MAP_HEIGHT, MAP_WIDTH, 2, 2);
  wrefresh (main_window);

  /* Initialize character and map */
  init_character (&character);
  fillMap (MAP_HEIGHT, MAP_WIDTH, map);
  number_of_non_overlaping_rooms = generateRooms (MAP_HEIGHT, MAP_WIDTH, map, rooms, numRooms);
  generateCorridors (MAP_WIDTH, map, rooms, numRooms, number_of_non_overlaping_rooms);
  place_player (MAP_HEIGHT, MAP_WIDTH, map, &character);
  place_enemies (MAP_HEIGHT, MAP_WIDTH, map, num_goblins, num_skeletons, num_orcs);
  display_map (main_window, &character, MAP_HEIGHT, MAP_WIDTH, map);

  wrefresh (main_window);

  /* Enable keyboard input and non-blocking input mode */
  nodelay(main_window, TRUE);
  keypad(main_window, TRUE); /*enable the interpretation of special keys*/
  movement (&character, MAP_HEIGHT, MAP_WIDTH, map, main_window);

  // cleanup and exit
  delwin(main_window);
  endwin ();
    
  return 0;
}