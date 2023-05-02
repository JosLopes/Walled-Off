#include "mapgen.h"
#include "datatypes.h"
#include "defines.h"
#include "movement.h"
#include "vision.h"
#include "MOBs.h"
#include <ncurses.h>
#include <time.h>
#include <stdlib.h>

void init_character(Character *character)
{
  character -> x = 0;
  character -> y = 0;
  character -> life = 100;
}

void init_ncurses() {

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
  raw();
  noecho();
  curs_set(FALSE);

  /*Enable the use of colors*/
  use_default_colors();
  curs_set(0);
  noecho();

}

int main () {


  WINDOW *main_window;
  srand(time(NULL));

  char map[MAP_HEIGHT][MAP_WIDTH];
  char traveled_path[MAP_HEIGHT][MAP_WIDTH];
  Room rooms[MAX_ROOMS];
  int number_of_non_overlaping_rooms;
  Character character;
  int numRooms = rand() % (MAX_ROOMS - 35) + 10;

  Variable_stats *dumb_variables;
  Variable_stats *smart_variables;
  Variable_stats *genius_variables;


  init_ncurses();
  clear();
  refresh();


  /* Create main window */
  main_window = create_window (MAP_HEIGHT, MAP_WIDTH, 2, 2);
  refresh();
  wrefresh (main_window);

  /* Initialize character and map */
  init_character (&character);
  fillMap (MAP_HEIGHT, MAP_WIDTH, map);
  fillTraveledPath (MAP_HEIGHT, MAP_WIDTH, traveled_path);
  number_of_non_overlaping_rooms = generateRooms (MAP_HEIGHT, MAP_WIDTH, map, rooms, numRooms);

  int enemies_size = number_of_non_overlaping_rooms * 3;
  Enemy *enemies = malloc (sizeof (Enemy) * enemies_size);

  Non_overlaping_rooms not_overlpg[number_of_non_overlaping_rooms];
  /* Initializes a struct array that stores all non overlaping rooms */
  init_non_overlaping_rooms (rooms, numRooms, not_overlpg, number_of_non_overlaping_rooms);

  /* Initializes the placement for all enemies, returning how many where placed in the map */
  int new_enemies_size;
  new_enemies_size = locate_positions (MAP_HEIGHT, MAP_WIDTH, map, enemies_size, enemies, number_of_non_overlaping_rooms, not_overlpg);
  Tag *tag = malloc (sizeof (Tag) * new_enemies_size);
  dumb_variables = d_enemies_variable_stats ();
  smart_variables = s_enemies_variable_stats ();
  genius_variables = g_enemies_variable_stats ();
  init_enemies (new_enemies_size, enemies, tag, dumb_variables, smart_variables, genius_variables, MAP_HEIGHT, MAP_WIDTH, map);

  generateCorridors (MAP_WIDTH, map, not_overlpg, number_of_non_overlaping_rooms);
  place_player (MAP_HEIGHT, MAP_WIDTH, map, &character);
  display_map (main_window, &character, MAP_HEIGHT, MAP_WIDTH, map, traveled_path);

  wrefresh (main_window);
  attron(COLOR_PAIR(WATER_COLOR));
  mvwaddch(main_window, 0, 0, 'w');
  attroff(COLOR_PAIR(WATER_COLOR));

  /* Enable keyboard input and non-blocking input mode */
  /* Wrefresh(main_window) */
  keypad(main_window, TRUE); /*enable the interpretation of special keys*/
  movement (&character, MAP_HEIGHT, MAP_WIDTH, map, traveled_path, main_window);

  // cleanup and exit
  free (enemies);
  enemies = NULL;
  delwin (main_window);
  endwin ();
    
  return 0;
}