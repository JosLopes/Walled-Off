
/* Node's structure */
typedef struct Path_finder_node {
  int row, col;  /* Position on the map */
  int h, g, f;  /* Integer values for h cost (distance from the goal),
                  for g cost (distance from the origin) and f-cost,
                  the total distance (h cost + g cost)*/
  int explored; /* True or false */
  Path_finder_node prev*;  /* Pointer to the prev of this node */
} Path_finder_node;

/* A queue to store every possible path unexplored */
typedef struct Path_queue {
  Path_finder_node *nodes;
  int head, tail;  /* Head and tail of the above array */
  int number_of_nodes;
} Path_queue;

void init_queue (Path_queue *path)
{
  int index;
  path -> *nodes = malloc (sizeof (Path_queue) * (MAP_HEIGHT * MAP_WIDTH));
  path -> head = 0;
  path -> tail = -1; /* When the queue is empty there is no tail */
  path -> number_of_nodes = 0;
}

void init_origin_node (Path_finder_node *origin, Enemy *enemy)
{
  origin -> row = enemy -> y;
  origin -> col = enemy -> x;

  /* As this is the starting point, the costs are irrelevant, so they are defaulted to 0 */
  origin -> h = 0;
  origin -> g = 0;
  origin -> f = 0;

  origin -> explored = 0; /* True, already explored */
  origin -> prev = NULL;  /* No previous node exists, this is the origin */
}

/* 
  Initialize a place holder wich the only purpose is to wait for another
  node to take its place. Every other node created will have a f cost lower
  than the place_holder node */
void init_place_holder_node (Path_finder_node *place_holder)
{
  place_holder -> f = MAP_HEIGHT * MAP_WIDTH; /* Bigher than the bigher possible f */
  place_holder -> explored = 1; /* False, never explored given the nature of the algorithm */
}

int dequeue (Path_queue *path)
{
  if (path -> head < path -> tail)
  {
    path -> head ++;
    path -> number_of_nodes --;
    return number_of_nodes; /* Sucess, the head has been dequeued */
  }
  else /* When the function is called uppon an empty queue (head == tail) */
  {
    return 0; /* Number of nodes on the empty queue */
  }
}

/* Insert sort's a new node in the queue to make sure it stays a priority queue */
void insert_queue (Path_queue *path, Path_finder_node node)
{
  int index;
  /* There is no need for verifying if the queue is full since the size 
     is equal to the number of nodes in the full map (walkable or not),
     ence its impossible to reach the end of the queue */

  path -> tail ++;
  for (index = path -> tail; path -> nodes[index].f > node.f && index > path -> head; index --)
  {
    path -> nodes[index + 1] = path -> nodes[index];
  }
  path -> nodes[index] = node; //talvez precise do pointer para a node ao inves, but idk;
  path -> number_of_nodes ++;
}

/* Create a new node to insert in the queue */
Path_finder_node init_new_node (int new_row, int new_col, Character *character, Path_finder_node prev)
{
  Path_finder_node *new_node = malloc (sizeof (Path_finder_node));
  new_node -> row = new_row;
  new_node -> col = new_col;
  new_node -> prev = &prev; /* Origin (previous node before new_node) */

  /* Calculate g, h and f costs, described in the struct Path_finder_node */
  new_node -> g = (10 * sqrt (pow (prev -> row - new_row) + pow (prev -> col - new_col))) + prev -> g;
  new_node -> h = (10 * sqrt (pow (character -> y - new_row) + pow (charachter -> x - new_col)));
  new_node -> f = new_node -> g + new_node -> h;

  new_node -> explored = 1;
  return *new_node;
}

/* Insert the nodes surrounding the origin in the queue,
   in the first iteraction it inserts only the origin */
int find_path (Enemy *enemy, Character *character, char map**)
{
  int index;

  /* To be used in verifying paths in the loop */
  int current_row;
  int current_col;

  Path_queue *path = malloc (sizeof (Path_queue));
  init_queue (path); /* Initializes the queue */

  /* Starting the single first node */
  Path_finder_node *origin_node = malloc (sizeof (Path_finder_node));
  init_origin_node (origin_node, enemy);
  insert_queue (path, *origin_node);  /* Inserts first node in the queue */

  Path_finder_node *current_node = malloc (sizeof (Path_finder_node));  /* Current origin */

  /* Place holder to occupie the array of nodes */
  Path_finder_node *place_holder = malloc (sizeof (Path_finder_node));
  init_place_holder_node (place_holder);

  Path_finder_node **node_array = malloc (sizeof (Path_finder_node *) * MAP_HEIGHT);
  for (index = 0; index < MAP_WIDTH; index ++)
  {
    node_array[i] = malloc (sizeof (Path_finder_node) * MAP_WIDTH);
    node_array[i] = *place_holder;
  }

  // Construir os nodos à volta da origin e colocar na *priority* queue;
  do
  {
    *current_node = path -> nodes[head];
    /* Use the node with the less f cost */
    node_array[current_node -> row][current_node -> col] = *current_node;
    current_node -> explored = 0; /* Begins exploring the current_node */

    current_row = current_node -> row;
    current_col = current_node -> col;

    /* 
      If the node in the map array is valid to walk by enemies and
      the node in the same position was not yet explored, create a
      new node and verify another condition */
    if (map[current_row +1][current_col] == FLOOR_CHAR &&
        node_array[current_row +1][current_col].explored == 1)
    {
      node_array[current_row +1][current_col] = init_new_node (current_row +1, current_col, character, *current_node);
      //verificar se f é menor que o existente, se sim, troca;
      insert_queue (path, node_array[current_row +1][current_col]);
    }

  } while (path -> number_of_nodes != 0);
}