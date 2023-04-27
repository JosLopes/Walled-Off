#include "mapgen.h"
#include "datatypes.h"
#include "defines.h"
#include "mapgen.h"
#include "movement.h"
#include <ncurses.h>
#include <time.h>
#include <stdlib.h>

int main () {
  WINDOW *main_window;
  char map[MAP_HEIGHT][MAP_WIDTH];
  Room rooms[MAX_ROOMS];
  int number_of_non_overlaping_rooms;
  srand(time(NULL));
  Character character;
  int numRooms = rand() % (MAX_ROOMS - 35) + 10;
  int enemys_size = sizeof (Enemy) * number_of_non_overlaping_rooms * 3;
  int new_size;
  Enemy *enemys = malloc (enemys_size);

  /*initializes the curses library and sets up terminal I/O*/
  if (initscr() == NULL)
  {
    fprintf(stderr, "Error initializing ncurses.\n");
    exit(EXIT_FAILURE);
  }
  start_color();
  raw ();
  noecho ();
  curs_set(FALSE); /*Hides the cursor*/                                                                                                                                                  ////////////////
  
  /* Create main window */
  main_window = create_window (MAP_HEIGHT, MAP_WIDTH, 2, 2);

  /* Initialize character and map */
  init_character (&character);
  fillMap (MAP_HEIGHT, MAP_WIDTH, map);
  number_of_non_overlaping_rooms = generateRooms (MAP_HEIGHT, MAP_WIDTH, map, rooms, numRooms);

  Non_overlaping_rooms not_overlpg[number_of_non_overlaping_rooms];
  /* Initializes a struct array that stores all non overlaping rooms */
  init_non_overlaping_rooms (rooms, numRooms, no_overlpg, number_of_non_overlaping_rooms);
  /* Initializes the placement for all enemies, returning how many where placed in the map */
  new_size = locate_positions (MAP_HEIGHT, MAP_WIDTH, map, enemys_size, enemys, number_of_non_overlaping_rooms, Non_overlaping_rooms no_overlpg);
  
  Tag tag[new_size];

  init_tag (&tag, intel, life, range, max_xp, min_xp, scream_range, poison, group_desire);
  init_enemy (&enemy, tag, name, symbol, damage);
  
  generateCorridors (MAP_WIDTH, map, not_overlpg, number_of_non_overlaping_rooms);
  place_player (MAP_HEIGHT, MAP_WIDTH, map, &character);
  display_map (main_window, MAP_HEIGHT, MAP_WIDTH, map);

  wrefresh (main_window);

  /* Enable keyboard input and non-blocking input mode */
  /*  wrefresh(main_window);*/
  keypad(main_window, TRUE); /*enable the interpretation of special keys*/
  movement (&character, MAP_HEIGHT, MAP_WIDTH, map, main_window);

  // cleanup and exit
  delwin(main_window);
  endwin ();
    
  return 0;
}