#ifndef DEFINES_H
#define DEFINES_H

/* Max range for the vision */
#define AWAKE_RANGE 4

#define MAP_HEIGHT 50
#define MAP_WIDTH 100
#define MAX_ROOMS 50
#define MIN_ROOM_HEIGHT 6
#define MAX_ROOM_HEIGHT 15
#define MIN_ROOM_WIDTH 6
#define MAX_ROOM_WIDTH 16
#define WALL_CHAR '#'
#define FLOOR_CHAR '.'
#define FIRE_CHAR '~'
#define UNDISCOVERED_PATH_CHAR ' '

#define PLAYER_CHAR_LEFT '<'
#define PLAYER_CHAR_RIGHT '>'
#define PLAYER_CHAR_UP '^'
#define PLAYER_CHAR_DOWN 'V'
#define PLAYER_VISION 3

#define PLAYER_VISION_COLOR1 1
#define PLAYER_VISION_COLOR2 2
#define PLAYER_VISION_COLOR3 3
#define PLAYER_VISION_COLOR4 4
#define WATER_COLOR 5
#define FLOOR_COLOR 6
#define ENEMY_COLOR 7
#define WALL_COLOR 8
#define OBSCURE_COLOR 9

/* For menu */
#define BACKGROUND_COLOR 20
#define SELECTED_OPTION_COLOR 21
#define TITLE_COLOR 22

#endif