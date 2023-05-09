#include "defines.h"
#include "datatypes.h"
#include "MOBsAI.h"
#include <stdlib.h>

void init_is_awake (int number_of_enemies, Awake *is_awake)
{
  is_awake -> enemies_awaken = malloc (sizeof (Enemy) * number_of_enemies);
  is_awake -> total_size = number_of_enemies; 
  is_awake -> current_size = 0; /* Starts with 0 enemies in the awaken array */ 
}

/* 
  When the enemy is in a certain range from the player, it awakens (is added to the
  awaken array, inicating that it will pursue the player to kill him) */
void init_awaken_enemies (Character *character, Enemy *enemies, Awake *is_awake)
{
  int index, distance;
  Point objective;
  /* Initializes the objective as the main character */
  objective.y = character -> y;
  objective.x = character -> x;

  for (index = 0; index < is_awake -> total_size; index++)
  {
    /* Distance between the player and the enemy */
    distance = distance_from_objective (&objective, enemies[index]);
    if (distance < AWAKE_RANGE && enemies[index].awake == 1)
    {
      is_awake -> enemies_awaken[is_awake -> current_size] = enemies[index];
      is_awake -> current_size ++;
      enemies[index].awake = 0;
    }
  }
}

/* 
  Calculates the shortest path to an enemy, for use both in the smart AI and genius AI.
  When an enemy needs to call out for suport, be it by screaming or by reagruping, this 
  function finds the closest enemies who are not awaken yet
Node closest_enemy ()
{
  int index;
  int count = 0, max_count = -1; /* Counters to find out which path is shorter 
  Node shortest_enemy;
  Point objective;

  for (index = 0; index < number_of_enemies; index ++)
  {
    if (enemies[index].awaken == 1)
      {
      objective.y = enemies[index].y;
      objective.x = enemies[index].x;

      Node node = find_path (&objective, map, place_holder, path);
      while (node.prev != NULL)
      {
        count ++;
        node = *(node.prev);
      }
    
      if (count > max_count)
      {
        shortest_enemy = node;
        max_count = count;
      }
    }
  }
  return shortest_enemy;
}
*/

void build_path (Awake *is_awake, Character *character, char **map, Node *place_holder)
{
  /* By default the objective is the main character */
  Point objective;
  objective.y = character -> y;
  objective.x = character -> x;

  for (int index = 0; index < is_awake -> current_size; index ++)
  {
    /* Starting the single first node */
    Node origin_node;
    init_origin_node (&objective, &origin_node, &(is_awake -> enemies_awaken[index]));

    Path_queue path;  /* Path builder */
    init_queue (&path);
    insert_queue (&path, origin_node);  /* Inserts first node in the queue */

    /* 
      If an enemy has group desire (it will only attack once there are a certain
      number of enemies awaken), it searches for that required desire, be it by
      warning out the enemies and starting to fight, if they are smart, or by
      running away to the nearest enemy spawn point, expecting to bring someone to
      fight with them, if they are genius. This, ofcourse, only happens when the
      group desire isn't already achieved by the current awaken enemies or the
      number of enemy's in the game is suficient to reach the desired number 
    if (is_awake -> current_size < is_awake -> enemies_awaken[index].tag -> group_desire)
    {
      Node node = closest_enemy ();
    }
    else
    {
      Node node = find_path (objective, map, place_holder, path);
    }
    */

    Node node = find_path (&objective, map, place_holder, &path);

    while (node.prev != NULL)
    {
      map[node.row][node.col] = '=';
      node = *(node.prev);
    }

    free (path.nodes);
    path.nodes = NULL;
  }
}