
/* Node's structure */
typedef struct Path_finder_node {
  int row, col;  /* Position on the map */
  int h, g, f;  /* Integer values for h cost (distance from the goal),
                  for g cost (distance from the origin) and f-cost,
                  the total distance (h cost + g cost)*/
  Path_finder_node prev*;  /* Pointer to the prev of this node */
} Path_finder_node;

/* A queue to store every possible path unexplored */
typedef struct Path_queue {
  Path_finder_node *nodes;
  int head, tail;  /* Head and tail of the above array */
  int number_of_nodes;
} Path_queue;

int init_queue (Path_queue *path)
{
  path -> *nodes = malloc (sizeof (Path_queue) * (MAP_HEIGHT * MAP_WIDTH));
  path -> head = 0;
  path -> tail = -1; /* When the queue is empty there is no tail */
  path -> number_of_nodes = 0;
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

void init_origin_node (Path_finder_node *origin, Enemy *enemy)
{
  origin -> row = enemy -> y;
  origin -> col = enemy -> x;

  /* As this is the starting point, the costs are irrelevant, so they are defaulted to 0 */
  origin -> h = 0;
  origin -> g = 0;
  origin -> f = 0;

  origin -> prev = NULL;  /* No previous node exists, this is the origin */
}

/* Create a new node to insert in the queue */
Path_finder_node *init_new_node (int new_row, int new_col, Character *character, Path_finder_node prev)
{
  Path_finder_node *new_node = malloc (sizeof (Path_finder_node));
  new_node -> row = new_row;
  new_node -> col = new_col;
  new_node -> prev = &prev; /* Origin (previous node before new_node) */

  /* Calculate g, h and f costs, described in the struct Path_finder_node */
  new_node -> g = (10 * sqrt (pow (prev -> row - new_row) + pow (prev -> col - new_col))) + prev -> g;
  new_node -> h = (10 * sqrt (pow (character -> y - new_row) + pow (charachter -> x - new_col)));
  new_node -> f = new_node -> g + new_node -> h;
}

/* Insert the nodes surrounding the origin in the queue,
   in the first iteraction it inserts only the origin */
int find_path (Enemy *enemy, Character *character, char map**)
{
  Path_queue *path = malloc (sizeof (Path_queue));

  /* Starting the single first node */
  Path_finder_node *origin_node = malloc (sizeof (Path_finder_node));
  init_origin_node (origin_node, enemy);

  // Construir os nodos à volta da origin e colocar na *priority* queue;
  init_new_node (,character ,);
}