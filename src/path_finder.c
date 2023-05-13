#include "path_finder.h"
#include "defines.h"
#include "datatypes.h"
#include <stdlib.h>
#include <math.h>

int subtract_to_max (int x, int y)
{
  if (x > y) return x - y;
  else return y - x;
}

/* Calculates the distance from the player to the enemy */
int distance_from_objective (Point objective, Point start)
{
  /* 
    Manhattan distance for one time's calculations, it will be used for some instances refering to 
    the Pathfinder in this module, but the euclindian distance will be preferred */
  return subtract_to_max (objective.y, start.y) + subtract_to_max (objective.x, start.x);
}

void init_queue (Path_queue *path)
{
  path -> nodes = malloc (sizeof (Path_queue) * (MAP_HEIGHT * MAP_WIDTH));
  path -> head = 0;
  path -> tail = -1; /* When the queue is empty there is no tail */
  path -> number_of_nodes = 0;
}

void init_origin_node (Point *objective, Point *start, Node *origin)
{
  origin -> row = start -> y;
  origin -> col = start -> x;

  /* As this is the starting point, the costs are irrelevant except for h, */
  /* h needs to be superior to 10 to dont stop the path loop               */
  origin -> h = (10 * distance_from_objective (*objective, *start));
  origin -> g = 0;
  origin -> f = 0;

  origin -> explored = 0; /* True, already explored */
  origin -> prev = NULL;  /* No previous node exists, this is the origin */
}

/* 
  Initialize a place holder wich the only purpose is to wait for another
  node to take its place. Every other node created will have a f cost lower
  than the place_holder node */
void init_place_holder_node (Node *place_holder)
{
  place_holder -> f = MAP_HEIGHT * MAP_WIDTH; /* Bigher than the bigher possible f */
  place_holder -> explored = 1; /* False, never explored given the nature of the algorithm */
}

int dequeue (Path_queue *path)
{
  if (path -> number_of_nodes != 0)
  {
    path -> head ++; /* When the head surpasses the tail */
    path -> number_of_nodes --;
    return path -> number_of_nodes; /* Sucess, the head has been dequeued */
  }
  else /* When the function is called uppon an empty queue (head == tail) */
  {
    return 0; /* Number of nodes on the empty queue */
  }
}

/* Insert sort's a new node in the queue to make sure it stays a priority queue */
void insert_queue (Path_queue *path, Node node)
{
  int index;
  /* There is no need for verifying if the queue is full since the size 
     is equal to the number of nodes in the full map (walkable or not),
     ence its impossible to reach the end of the queue */

  path -> tail ++;
  for (index = path -> tail; index > path -> head && path -> nodes[index-1].f > node.f; index --)
  {
    path -> nodes[index] = path -> nodes[index - 1];
  }
  path -> nodes[index] = node; //talvez precise do pointer para a node ao inves, but idk;
  path -> number_of_nodes ++;
}

/* Create a new node to insert in the queue */
Node init_new_node (int new_row, int new_col, Point *objective, Node node, Node *prev)
{
  Node new_node;
  new_node.row = new_row;
  new_node.col = new_col;
  new_node.prev = prev; /* Origin (previous node before new_node) */

  /* Calculate g, h and f costs, described in the struct Node */
  new_node.g = (10 * sqrt (pow (node.row - new_row, 2) + pow (node.col - new_col, 2))) + node.g;
  new_node.h = (10 * sqrt (pow (objective -> y - new_row, 2) + pow (objective -> x - new_col, 2)));
  new_node.f = new_node.g + new_node.h;

  new_node.explored = 1;
  return new_node;
}

/* Insert the nodes surrounding the origin in the queue,
   in the first iteraction it inserts only the origin */
Node find_path (Point *objective, char **map, Node *place_holder, Path_queue *path, int *ended_without_path)
{
  Node *prev;
  /* To be used in verifying paths in the loop */
  int current_row;
  int current_col;

  Node current_node;  /* Current origin */
  Node temp;  /* Temporary node */
  /* 
    In case the queue ends without finding a path,
    return this value so the enemy stays in the 
    same position */
  Node starting_position = path -> nodes[path -> head];

  Node node_array[MAP_HEIGHT][MAP_WIDTH];
  /* Initializes the array of nodes with place_holders */
  for (current_row = 0; current_row < MAP_HEIGHT; current_row ++)
  {
    for (current_col = 0; current_col < MAP_WIDTH; current_col ++)
    {
      node_array[current_row][current_col] = *place_holder;
    }
  }

  // Construir os nodos à volta da origin e colocar na *priority* queue;
  do
  {
    prev = &(path -> nodes[path -> head]);
    current_node = path -> nodes[path -> head];
    /* Use the node with the less f cost */
    node_array[current_node.row][current_node.col] = current_node;
    current_node.explored = 0; /* Begins exploring the current_node */
    dequeue (path);

    current_row = current_node.row;
    current_col = current_node.col;

    int current_row_plus = current_row + 1;

    /*
      If the node in the map array is valid to walk by enemies and
      the node in the same position was not yet explored, create a
      new node and verify another condition */
    if (map[current_row_plus][current_col] == FLOOR_CHAR &&
        node_array[current_row_plus][current_col].explored == 1)
    {
      /* Temporary above node */
      temp = init_new_node (current_row_plus, current_col, objective, current_node, prev);

      /* Test the node above */
      if (temp.f < node_array[current_row_plus][current_col].f)
      {
        node_array[current_row_plus][current_col] = temp;
        insert_queue (path, node_array[current_row_plus][current_col]);
      }
    }

    int current_row_less = current_row - 1;

    if (map[current_row_less][current_col] == FLOOR_CHAR &&
        node_array[current_row_less][current_col].explored == 1)
    {
      /* Temporary bellow node */
      temp = init_new_node (current_row_less, current_col, objective, current_node, prev);

      /* Test the node bellow */
      if (temp.f < node_array[current_row_less][current_col].f)
      {
        node_array[current_row_less][current_col] = temp;
        insert_queue (path, node_array[current_row_less][current_col]);
      }
    }

    int current_col_plus = current_col + 1;

    if (map[current_row][current_col_plus] == FLOOR_CHAR &&
        node_array[current_row][current_col_plus].explored == 1)
    {
      /* Temporary right node */
      temp = init_new_node (current_row, current_col_plus, objective, current_node, prev);

      /* Test the node to the right */
      if (temp.f < node_array[current_row][current_col_plus].f)
      {
        node_array[current_row][current_col_plus] = temp;
        insert_queue (path, node_array[current_row][current_col_plus]);
      }
    }

    int current_col_less = current_col -1;

    if (map[current_row][current_col_less] == FLOOR_CHAR &&
        node_array[current_row][current_col_less].explored == 1)
    {
      /* temporary left node */
      temp = init_new_node (current_row, current_col_less, objective, current_node, prev);

      /* Test the node to the left */
      if (temp.f < node_array[current_row][current_col_less].f)
      {
        node_array[current_row][current_col_less] = temp;
        insert_queue (path, node_array[current_row][current_col_less]);
      }
    }
  } while (path -> number_of_nodes != 0 && current_node.h > 10);

  if (path -> number_of_nodes == 0 &&
      current_node.h > 10 &&
      *ended_without_path == 1)
  {
    *prev = starting_position;
    *ended_without_path = 0;
  }

  return *prev;
}
