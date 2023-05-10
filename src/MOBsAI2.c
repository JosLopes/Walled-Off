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
  Point objective, future_start;
  /* Initializes the objective as the main character */
  objective.y = character -> y;
  objective.x = character -> x;

  for (index = 0; index < is_awake -> total_size; index++)
  {
    future_start.y = enemies[index].y;
    future_start.x = enemies[index].x;
    /* Distance between the player and the enemy */
    distance = distance_from_objective (objective, future_start);
    if (distance < AWAKE_RANGE && enemies[index].awake == 1)
    {
      is_awake -> enemies_awaken[is_awake -> current_size] = enemies[index];
      is_awake -> current_size ++;
      enemies[index].awake = 0;
    }
  }
}

/* 
  Calculates the closest path to an enemy, for use both in the smart AI and genius AI.
  When an enemy needs to call out for suport, be it by screaming or by reagruping, this 
  function finds the closest enemies who are not awaken yet */
Node closest_enemy (int number_of_enemies, char **map, Node *place_holder, Path_queue *path, Point start, Enemy *enemies, Node origin_node)
{
  int index;
  int count, min_count = MAP_HEIGHT * MAP_WIDTH; /* Counters to find out which path is shorter */
  Node closest_enemy;
  Point objective;

  for (index = 0; index < number_of_enemies; index ++)
  {
    if (enemies[index].awake == 1)
      {
      objective.y = enemies[index].y;
      objective.x = enemies[index].x;
      init_origin_node (&objective, &start, &origin_node);

      init_queue (path);
      insert_queue (path, origin_node);  /* Inserts first node in the queue */
      Node node = find_path (&objective, map, place_holder, path);
      count = node.g; /* Distance from the starting point to the last node */
    
      if (count < min_count)
      {
        closest_enemy = node;
        min_count = count;
      }
    }
  }
  return closest_enemy;
}


void build_path (Awake *is_awake, Character *character, char **map, Node *place_holder, Enemy *enemies)
{
  Node top_node;
  Path_queue path;  /* Path builder */
  Node origin_node;
  /* By default the objective is the main character */
  Point objective, start;

  for (int index = 0; index < is_awake -> current_size; index ++)
  {
    /* 
      If an enemy has group desire (it will only attack once there are a certain
      number of enemies awaken), it searches for that required desire, be it by
      warning out the enemies and starting to fight, if they are smart, or by
      running away to the nearest enemy spawn point, expecting to bring someone to
      fight with them, if they are genius. This, ofcourse, only happens when the
      group desire isn't already achieved by the current awaken enemies or the
      number of enemy's in the game is suficient to reach the desired number */
    if (is_awake -> current_size < is_awake -> total_size % (is_awake -> enemies_awaken[index].tag -> group_desire))
    {
      /* Starting the single starting node */
      start.y = is_awake -> enemies_awaken[index].y;
      start.x = is_awake -> enemies_awaken[index].x;
      
      top_node = closest_enemy (is_awake -> total_size, map, place_holder, &path, start, enemies, origin_node);
    }
    else
    {
      /*Starting the single first node */
      objective.y = is_awake -> enemies_awaken[index].y;
      objective.x = is_awake -> enemies_awaken[index].x;
      /* Starting node */
      start.y = character -> y;
      start.x = character -> x;
      init_origin_node (&objective, &start, &origin_node);

      init_queue (&path);
      insert_queue (&path, origin_node);  /* Inserts first node in the queue */
      top_node = find_path (&objective, map, place_holder, &path);
    }

    while (top_node.prev != NULL)
    {
      map[top_node.row][top_node.col] = '=';
      top_node = *(top_node.prev);
    }

    free (path.nodes);
    path.nodes = NULL;
  }
}