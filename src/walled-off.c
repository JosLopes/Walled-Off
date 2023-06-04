#include "menu.h"
#include <ncurses.h>
#include <time.h>
#include <stdlib.h>
#include "mapgen.h"
#include "datatypes.h"
#include "defines.h"
#include "movement.h"
#include "vision.h"
#include "MOBs.h"
#include "path_finder.h"
#include "artificial_inteligence.h"
#include "combat.h"
#include "display.h"
#include "combat.h"
#include "consumables.h"
#include "health.h"

void init_character(Character *character)
{
  character -> x = 0;
  character -> y = 0;
  character -> life = 80;
  character -> initial_life = 100;
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

int main ()
{
  /* WINDOWS to be used */
  WINDOW *main_window;
  WINDOW *display_win;
  WINDOW *instructions_win;


  /* Guarantees a new sequence of "random" numbers */
  srand(time(NULL));

  /* Map related initializations */
  int current_line;
  char **map = malloc (sizeof (char *) * MAP_HEIGHT);
  char **map_static_obstacles = malloc (sizeof (char *) * MAP_HEIGHT);
  for (current_line = 0; current_line < MAP_HEIGHT; current_line ++)
  {
    map[current_line] = malloc (sizeof (char) * MAP_WIDTH);
    map_static_obstacles[current_line] = malloc (sizeof (char) * MAP_WIDTH);
  }
  /* To display the traveled path */
  char traveled_path[MAP_HEIGHT][MAP_WIDTH];
  fillTraveledPath (MAP_HEIGHT, MAP_WIDTH, traveled_path);

  int numRooms = rand() % (MAX_ROOMS - 35) + 10;
  Room *rooms = malloc (sizeof (Room) * numRooms); // Free this memory later !!!
  int number_of_non_overlaping_rooms;

  /* Main Character related initializations */
  Character character;

  /* AI initialization */
  /* Place holder to occupie the array of nodes as a temporary node */
  Node *place_holder = malloc (sizeof (Node));
  init_place_holder_node (place_holder);

  init_ncurses(); /* Initializes ncurses and its functionalities */
  clear();
  refresh();
  /* Start the menu screen */
  int start_the_game = start_menu ();

  if (start_the_game == 0)
  {
    /* Creates main window (playable window) from mapgen.c */
    main_window = create_window (MAP_HEIGHT, MAP_WIDTH, 0, 0);
    
    fillMap (MAP_HEIGHT, MAP_WIDTH, map);
    number_of_non_overlaping_rooms = generateRooms (MAP_HEIGHT, MAP_WIDTH, map, rooms, numRooms);
    Non_overlaping_rooms *not_overlpg = malloc (sizeof (Non_overlaping_rooms) * number_of_non_overlaping_rooms); // Free this memory later !!!

    /* Mob's (enemies) related initializations */
    int max_number_of_enemies = number_of_non_overlaping_rooms * 3; /* Max number of enemies */

    /* Initializes a struct array that stores all non overlaping rooms */
    init_non_overlaping_rooms (rooms, numRooms, not_overlpg, number_of_non_overlaping_rooms);
    
    Enemy *enemies = malloc (sizeof (Enemy) * max_number_of_enemies); /* Storage for every enemy */

    /* Initializes the placement for all enemies, returning how many where placed in the map */
    /* from the max value "number_of_enemies"                                                */
    int number_of_enemies = locate_positions (MAP_HEIGHT, MAP_WIDTH, map, max_number_of_enemies, enemies, number_of_non_overlaping_rooms, not_overlpg);
    /* Awaken enemies for use in the pathfinding algorithm    */
    /* When an enemy is awaken, it pursues the enemy actively */
    Awake *is_awake = malloc (sizeof (Awake));
    init_is_awake (number_of_enemies, is_awake);

    /* Finishes the building of the map */
    generateCorridors (map, not_overlpg, number_of_non_overlaping_rooms);

    /* Initializes variable stats for each type of enemmy */
    Variable_stats *dumb_variables = d_enemies_variable_stats ();
    Variable_stats *smart_variables = s_enemies_variable_stats ();
    Variable_stats *genius_variables = g_enemies_variable_stats ();
    /* Initializes all the enemies stats, including pre-defined */
    Tag *tag = init_enemies (number_of_enemies, enemies, dumb_variables, smart_variables, genius_variables, map);

    /* Foods and potions */
    Consumables *consumables = consumablesHeap();
    /* Array of available consumables that store the consumables generated */
    /* Use this random number to determine the number of foods/potions in the map (it needs to be > 5) */
    int number_of_consumables = number_of_enemies / 3;
    Consumables *available = malloc (sizeof (Consumables) * number_of_consumables);
    /* Placing consumables */
    place_foods_and_potions (map, number_of_consumables, consumables, available);
    
    /* Initializes a map with fixed obstacles */
    for (int index_y = 0; index_y < MAP_HEIGHT; index_y ++)
    {
      for (int index_x = 0; index_x < MAP_WIDTH; index_x ++)
      {
        map_static_obstacles[index_y][index_x] = map[index_y][index_x];
      }
    }
    
    /* Initializes main character, placing it in the map */
    init_character (&character);
    place_player (*not_overlpg, map, &character);

    /* Display the map on the main_window */
    display_map (main_window, &character, map, MAP_WIDTH, traveled_path);
    wrefresh (main_window); /* Refresh main_window */

    /* Creats the windows of instructions and parameters*/
    instructions_win = newwin(9, MAP_WIDTH, MAP_HEIGHT, 0);
    display_win = newwin(MAP_HEIGHT, 40, 0, MAP_WIDTH);

    char previous_char = FLOOR_CHAR;  /* Char before the character got placed in the map */

    start_display ();
    print_displays (display_win, &character, is_awake, traveled_path);   
    refresh();  
    wrefresh(display_win);
    start_instructions ();
    print_instructions_win (instructions_win, &character, available, is_awake, traveled_path, &previous_char, number_of_consumables);  
    wrefresh(instructions_win);
    refresh();

    /* Enable keyboard input for special keys */
    keypad(main_window, TRUE);

    /*=================================== End of Initialization ===================================*/
    
    /* GAME LOOP */
    int ch;  /* Input character read as an integer */

    /* Initialize count_water variable */
    int *count_water = (int*)malloc(sizeof(int));
    *count_water = 0;

    while ((ch = wgetch(main_window)) != 'q')
    {
      /* Basic movement */
      movement (&character, map, ch, &previous_char);

      /* Introducing vision */
      vision_color (main_window, &character, map, MAP_WIDTH, traveled_path);

      food_and_potions (&character, available, &previous_char, number_of_consumables);

      water_damage(map, &character, &previous_char, count_water);
      
      if (previous_char != WATER_CHAR)
      {
        /* Initializes more enemies, if necessary, to the is_awaken struct */
        init_awaken_enemies (&character, enemies, is_awake, map_static_obstacles);
        build_path (is_awake, &character, map, traveled_path, map_static_obstacles, place_holder, enemies);
      }

      /* At the end of every loop, refresh main_window, display_win and instructions_win */
      display_map (main_window, &character, map, MAP_WIDTH, traveled_path);
      print_instructions_win (instructions_win, &character, available, is_awake,traveled_path, &previous_char, number_of_consumables);
      werase(instructions_win);
      print_displays (display_win, &character, is_awake, traveled_path);
      werase (display_win);
    }


    /* cleanup and exit */
    free (enemies); /* fazer uma função void free */
    enemies = NULL;

    for (current_line = 0; current_line < MAP_HEIGHT; current_line ++)
    {
      free (map[current_line]);
      map[current_line] = NULL;
    }
    free (map);
    map = NULL;

    /* Free tag */
    free (tag);
    tag = NULL;

    /* Free not_overlpg */
    free (not_overlpg);
    not_overlpg = NULL;

    free (rooms);
    rooms = NULL;

    free (place_holder);
    place_holder = NULL;
  
    delwin (main_window);
  }

  endwin ();
    
  return 0;
}