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

  /*Define color pairs*/
  init_pair(WATER_COLOR, COLOR_CYAN, COLOR_BLUE);
  init_pair(PLAYER_VISION_COLOR, COLOR_YELLOW, COLOR_RED);
  init_pair(FLOOR_COLOR, COLOR_WHITE, COLOR_BLACK);

  /*Enable the use of colors*/
  use_default_colors();
  curs_set(0);
  noecho();

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
  number_of_non_overlaping_rooms = generateRooms (MAP_HEIGHT, MAP_WIDTH, map, rooms, numRooms);
  generateCorridors (MAP_WIDTH, map, rooms, numRooms, number_of_non_overlaping_rooms);
  place_player (MAP_HEIGHT, MAP_WIDTH, map, &character);
  place_enemies (MAP_HEIGHT, MAP_WIDTH, map, num_goblins, num_skeletons, num_orcs);
  display_map (main_window, &character, MAP_HEIGHT, MAP_WIDTH, map);

  wrefresh (main_window);
  attron(COLOR_PAIR(WATER_COLOR));
  mvwaddch(main_window, 0, 0, 'w');
  attroff(COLOR_PAIR(WATER_COLOR));

  /* Enable keyboard input and non-blocking input mode */
  nodelay(main_window, TRUE);
  keypad(main_window, TRUE); /*enable the interpretation of special keys*/
  movement (&character, MAP_HEIGHT, MAP_WIDTH, map, main_window);

  // cleanup and exit
  delwin(main_window);
  endwin ();
    
  return 0;
}