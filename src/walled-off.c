#include "mapgen.h"
#include "datatypes.h"
#include <ncurses.h>
#include <time.h>
#include <stdlib.h>

int main () {
    WINDOW *main_window;
    char map[MAP_HEIGHT][MAP_WIDTH];
    Room rooms[MAX_ROOMS];
    int number_of_non_overlaping_rooms;
    srand(time(NULL));
    int numRooms = rand() % (MAX_ROOMS - 35) + 10;
    int num_goblins = rand() % 10;
    int num_orcs = rand() % 10;
    int num_skeletons = rand() % 10;

    initscr ();
    start_color();
    raw ();
    noecho ();
    
    main_window = create_window (MAP_HEIGHT, MAP_WIDTH, 2, 2);

    fillMap (MAP_HEIGHT, MAP_WIDTH, map);
    number_of_non_overlaping_rooms = generateRooms (MAP_HEIGHT, MAP_WIDTH, map, rooms, numRooms);
    generateCorridors (MAP_WIDTH, map, rooms, numRooms, number_of_non_overlaping_rooms);
    place_player (MAP_HEIGHT, MAP_WIDTH, map);
    place_enemies (MAP_HEIGHT, MAP_WIDTH, map, num_goblins, num_skeletons, num_orcs);
    display_map (main_window, MAP_HEIGHT, MAP_WIDTH, map);

    wrefresh (main_window);

    getch ();
    endwin ();
    
    return 0;
}