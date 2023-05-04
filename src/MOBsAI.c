
/* Node's structure */
typedef struct Path_finder_node {
  int row, coll;  /* Position on the map */
  int h, g, f;  /* Integer values for h cost (distance from the enemy),
                  for g cost (distance from the player) and f-cost,
                  the total distance (h cost + g cost)*/
  Path_finder_node prev*;  /* Pointer to the father of this node */
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
  path -> tail = -1;
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
int queue (Path_queue *path, Path_finder_node node)
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

int find_path (Enemy *enemy, Character *character, char map**)
{
// to be defined
}