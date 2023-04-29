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

int main ()
{
  /* WINDOWS to be used */
  WINDOW *main_window;

  /* Guarantees a new sequence of "random" numbers */
  srand(time(NULL));

  /* Map related initializations */
  char map[MAP_HEIGHT][MAP_WIDTH];  /* Playable map */

  int numRooms = rand() % (MAX_ROOMS - 35) + 10;
  Room rooms[numRooms];
  int number_of_non_overlaping_rooms;

  /* Main Character related initializations */
  Character character;

  init_ncurses(); /* Initializes ncurses and its functionalities */
  clear();
  refresh();

  /* Creates main window (playable window) from mapgen.c */
  main_window = create_window (MAP_HEIGHT, MAP_WIDTH, 2, 2);

  fillMap (MAP_HEIGHT, MAP_WIDTH, map);
  number_of_non_overlaping_rooms = generateRooms (MAP_HEIGHT, MAP_WIDTH, map, rooms, numRooms);
  Non_overlaping_rooms not_overlpg[number_of_non_overlaping_rooms];

  /* Mob's (enemies) related initializations */
  int max_number_of_enemies = number_of_non_overlaping_rooms * 3; /* Max number of enemies */

  /* Initializes a struct array that stores all non overlaping rooms */
  init_non_overlaping_rooms (rooms, numRooms, not_overlpg, number_of_non_overlaping_rooms);
  
  Enemy *enemies = malloc (sizeof (Enemy) * max_number_of_enemies); /* Storage for every enemy */
  Tag *tag = malloc (sizeof (Tag) * max_number_of_enemies);

    /* Initializes the placement for all enemies, returning how many where placed in the map from the max value "number_of_enemies" */
  int number_of_enemies = locate_positions (MAP_HEIGHT, MAP_WIDTH, map, max_number_of_enemies, enemies, number_of_non_overlaping_rooms, not_overlpg);
  
  /* Initializes variable stats for each type of enemmy */
  Variable_stats *dumb_variables = d_enemies_variable_stats ();
  Variable_stats *smart_variables = s_enemies_variable_stats ();
  Variable_stats *genius_variables = g_enemies_variable_stats ();
  /* Initializes all the enemies stats, including pre-defined */
  init_enemies (number_of_enemies, enemies, tag, dumb_variables, smart_variables, genius_variables, MAP_WIDTH, map);

  /* Finishes the building of the map */
  generateCorridors (MAP_WIDTH, map, not_overlpg, number_of_non_overlaping_rooms);

  /* Initializes main character, placing it in the map */
  init_character (&character);
  place_player (MAP_HEIGHT, MAP_WIDTH, map, &character);

  /* Display the map on the main_window */
  display_map (main_window, &character, MAP_HEIGHT, MAP_WIDTH, map);
  wrefresh (main_window); /* Refresh main_window */

  /* I dont know hahahahha */
  attron(COLOR_PAIR(WATER_COLOR));
  mvwaddch(main_window, 0, 0, 'w');
  attroff(COLOR_PAIR(WATER_COLOR));

  /* Enable keyboard input for special keys */
  keypad(main_window, TRUE);

  /*=================================== End of initialization ===================================*/
  
  /* GAME LOOP */
  /* Input character read as an integer */
  int ch;

  while ((ch = wgetch(main_window)) != 'q')
  {
    /* Basic movement */
    movement (&character, MAP_WIDTH, map, main_window, ch);

    /* Introducing vision */
    vision(main_window, &character, MAP_HEIGHT, MAP_WIDTH, map);

    /* At the end of every loop, refresh main_window */
    display_map (main_window, &character, MAP_HEIGHT, MAP_WIDTH, map);
  }

  /* cleanup and exit */
  free (enemies); /* fazer uma função void free */
  enemies = NULL;

  delwin (main_window);
  endwin ();
    
  return 0;
}