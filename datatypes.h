/* A Point in a map */
typedef struct point {
  int x;
  int y;
} Point;

/* Rectangle for use in the random */
/* generator for map rooms         */
typedef struct {
  Point dow_left_point;
  Point up_right_point;
} Rectangle;

/* Circle for use in the random */
/* generator for map rooms      */
typedef struct {
  Point center;
  int radius;
} Circle;

/* Hallway for use in the random generator */
/* to conect diferent rooms                */
typedef struct {
  Point start;
  Point end;
} Hallway;