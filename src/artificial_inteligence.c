#include "MOBs.h"
#include "defines.h"
#include "datatypes.h"
#include "path_finder.h"
#include <stdlib.h>

/**
 * a104541-José António Fernandes Alves Lopes
 * Initializes the array is_awake for awaken enemies
 */
void init_is_awake (int number_of_enemies, Awake *is_awake)
{
  is_awake -> enemies_awaken = malloc (sizeof (Enemy) * number_of_enemies);
  is_awake -> total_size = number_of_enemies; 
  is_awake -> current_size = 0; /* Starts with 0 enemies in the awaken array */ 
}

/**
 * a104541-José António Fernandes Alves Lopes
 * When the enemy is in a certain range from the player, it awakens (is added to the
 * awaken array, inicating that it will pursue the player to kill him)
 */
void init_awaken_enemies (Character *character, Enemy *enemies, Awake *is_awake, char **map_static_obstacles)
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
    if (distance < AWAKE_RANGE && enemies[index].awake == 1) // && current_size < total_size
    {
      is_awake -> enemies_awaken[is_awake -> current_size] = enemies[index];
      is_awake -> current_size ++;
      enemies[index].awake = 0;
      /* This enemy is no longer an obstacle as it searches for the player */
      map_static_obstacles[enemies[index].y][enemies[index].x] = FLOOR_CHAR; 
    }
  }
}

/** 
 * a104541-José António Fernandes Alves Lopes
 * Calculates the closest path to an enemy, for use both in the smart AI and genius AI.
 * When an enemy needs to call out for suport, be it by screaming or by reagruping, this 
 * function finds the closest enemies who are not awaken yet
 */
Node closest_enemy (char **map, char **map_static_obstacles, Node *place_holder, Path_queue *path, Point start, Enemy *enemies, Awake *is_awake, Node origin_node)
{
  int ended_without_path = 0;  /* dont check diferent paths */
  int index;
  int count, min_count = MAP_HEIGHT * MAP_WIDTH; /* Counters to find out which path is shorter */
  Point objective;
  /* The closest enemy to wake up */
  int choosen_one = -1;

  for (index = 0; index < is_awake -> total_size; index ++)
  {
    ended_without_path = 0;
    if (enemies[index].awake == 1)
    {
      objective.y = enemies[index].y;
      objective.x = enemies[index].x;
      init_origin_node (&objective, &start, &origin_node);

      init_queue (path);
      insert_queue (path, origin_node);  /* Inserts first node in the queue */
      Node node = find_path (&objective, map, place_holder, path, &ended_without_path);
      count = node.f; /* f distance from the starting point to the last node */

      if (count < min_count && ended_without_path == 0)
      {
        min_count = count;
        choosen_one = index;
      }

      /* Cleans the path */
      while (path -> nodes != NULL)
      {
        Node *temp = path -> nodes;
        free (path -> nodes);
        path -> nodes = temp -> prev;
      }
    }
  }

  /* Awakens the closest enemy, if it was found */
  if (choosen_one != -1)
  {
    is_awake -> enemies_awaken[is_awake -> current_size] = enemies[choosen_one];
    is_awake -> current_size ++;
    enemies[choosen_one].awake = 0;
    /* This enemy is no longer an obstacle as it searches for the player */
    map_static_obstacles[enemies[index].y][enemies[index].x] = FLOOR_CHAR;
  }

  return origin_node;
}

/**
 * a104541-José António Fernandes Alves Lopes
 * Takes the path build from the pathfinder and builds it backwards,
 * from the previous  called objective (now suposed starting point) to 
 * the previous start (now suposed objective)
 */
void display_enemy_path (Consumables *available, Node top_node, char **map, char traveled_path[][MAP_WIDTH], Enemy *enemy)
{
  int old_y = enemy -> y, old_x = enemy -> x;
  char next_char = ' ';

  if (map[top_node.row][top_node.col] == FLOOR_CHAR ||
      map[top_node.row][top_node.col] == STATIC_OBS_CHAR) next_char = FLOOR_CHAR;
  if (map[top_node.row][top_node.col] == FOOD_CHAR) next_char = FOOD_CHAR;
  if (map[top_node.row][top_node.col] == POTION_CHAR) next_char = POTION_CHAR;

  if (top_node.prev != NULL && next_char != ' ')
  {
    /* Takes the enemy out of the map */
    map[old_y][old_x] = next_char;
    /* Changing the position of the enemy */
    enemy -> y = top_node.row;
    enemy -> x = top_node.col;
    /* Displaying the character of the enemy in the map */
    map[enemy -> y][enemy -> x] = enemy -> display;

    /* Corrects the position of the food/potion */
    if (next_char != FLOOR_CHAR)
    {
      for (int index = 0; index > -1; index++)
      {
        if (enemy -> y == available[index].y &&
            enemy -> x == available[index].x)
        {
          available[index].x = old_x;
          available[index].y = old_y;
          index = -2;
        }
      }
    }

    /* Make it so the enemy is only visible if it is in an area previously explored by the player */
    if (traveled_path[enemy -> y][enemy -> x] != UNDISCOVERED_PATH_CHAR)
    {
      traveled_path[enemy -> y][enemy -> x] = enemy -> display;
    }
    if (traveled_path[old_y][old_x] != UNDISCOVERED_PATH_CHAR)
    {
      traveled_path[old_y][old_x] = next_char;
    }
  }
}

void simple_free (Path_queue path)
{
  /* Cleans the path */
  while (path.nodes != NULL)
  {
    Node *temp = path.nodes;
    free (path.nodes);
    path.nodes = temp -> prev;
  }
}

int calculate_distance (Enemy en, Character ch, char **map, Node *place_holder)
{
  Node top_node;
  Path_queue path;  /* Path builder */
  Node origin_node;
  /* By default the objective is the main character */
  Point objective, start;
  int ended_without_path = 1;

  /*Starting the single first node */
  objective.y = en.y;
  objective.x = en.x;
  /* Starting node */
  start.y = ch.y;
  start.x = ch.x;

  /* Initializes the path */
  init_origin_node (&objective, &start, &origin_node);
  init_queue (&path);
  insert_queue (&path, origin_node);  /* Inserts first node in the queue */

  /* Calculates the distance from the player to the enemy */
  top_node = find_path (&objective, map, place_holder, &path, &ended_without_path);
  int distance_from_player = top_node.g;

  simple_free (path);
  
  return distance_from_player;
}

void awaken_in_order (Awake *is_awake, Character ch, char **map, Node *place_holder)
{
  Enemy aux;
  int index, comp_index, distance_from_player;

  for (index = 0; index < is_awake -> current_size; index++)
  {
    aux = is_awake -> enemies_awaken[index];
    distance_from_player = calculate_distance (aux, ch, map, place_holder);
    for (comp_index = index; comp_index > 0 &&
         calculate_distance (is_awake -> enemies_awaken[comp_index -1], ch, map, place_holder) > distance_from_player;
         comp_index--)
         is_awake -> enemies_awaken[comp_index] = is_awake -> enemies_awaken[comp_index -1];
      is_awake -> enemies_awaken[comp_index] = aux;
  }
}

void build_obstacles (char **map, Node *node)
{
  if (node != NULL)
  {
    while (node -> prev != NULL)
    {
      if (map[node -> row][node -> col] != FOOD_CHAR &&
          map[node -> row][node -> col] != POTION_CHAR)
        map[node -> row][node -> col] = STATIC_OBS_CHAR;
      node = node -> prev;
    }
  }
}

void destroy_obstacles (char **map, Node *node_saver, int size)
{
  Node *top_node;

  for (int index = 0; index < size; index++)
  {
    top_node = node_saver[index].prev;
    while (top_node != NULL && top_node -> prev != NULL)
    {
      if (map[top_node -> row][top_node -> col] == STATIC_OBS_CHAR)
        map[top_node -> row][top_node -> col] = FLOOR_CHAR;
      top_node = top_node -> prev;
    }
  }
}

/**
 * a104541 - José António Fernandes Alves Lopes
 * Conects all of the AI related functions to build paths for the enemies
 */
void build_path (Consumables *available, Awake *is_awake, Character *character, char **map, char traveled_path[][MAP_WIDTH], char **map_static_obstacles, Node *place_holder, Enemy *enemies)
{
  Node top_node;
  Path_queue path;  /* Path builder */
  Node node_saver[4]; /* Can save 4 paths at maximum */
  Node origin_node;
  /* By default the objective is the main character */
  Point objective, start;
  int group_desire = 0;
  int ended_without_path = 1;
  int distance_from_player = 0;
  int ns_index = 0;

  for (int index = 0; index < is_awake -> current_size; index ++)
  {
    /**
     * If an enemy has group desire (it will only attack once there are a certain
     * number of enemies awaken), it searches for that required desire, be it by
     * warning out the enemies and starting to fight, if they are smart, or by
     * running away to the nearest enemy spawn point, expecting to bring someone to
     * fight with them, if they are genius. This, ofcourse, only happens when the
     * group desire isn't already achieved by the current awaken enemies or the
     * number of enemy's in the game is suficient to reach the desired number
     */
    group_desire = is_awake -> enemies_awaken[index].tag -> group_desire;

    if (is_awake -> current_size <  group_desire &&
        is_awake -> total_size >= group_desire)
    {
      /* Starting the single starting node */
      start.y = is_awake -> enemies_awaken[index].y;
      start.x = is_awake -> enemies_awaken[index].x;
      
      /* The node to be used as the first in the future constructed path */
      top_node = closest_enemy (map, map_static_obstacles, place_holder, &path, start, enemies, is_awake, origin_node);
    }
    /**
     * If the enemy isn't dumb, it searches for a path to trap the player,
     * if it doesnt dinf anything, it just acts like a dumb enemy, and goes
     * straight for the player without thinking about other enemies positions
     */
    else
    {
      /*Starting the single first node */
      objective.y = is_awake -> enemies_awaken[index].y;
      objective.x = is_awake -> enemies_awaken[index].x;
      /* Starting node */
      start.y = character -> y;
      start.x = character -> x;
      
      /* Initializes the path */
      init_origin_node (&objective, &start, &origin_node);
      init_queue (&path);
      insert_queue (&path, origin_node);  /* Inserts first node in the queue */

      /* Calculates the distance from the player to the enemy */
      top_node = find_path (&objective, map, place_holder, &path, &ended_without_path);
      distance_from_player = top_node.g;

      simple_free (path);

      switch (is_awake -> enemies_awaken[index].tag -> inteligence)
      {
      case D_CHAR:
        if (distance_from_player <= D_INTEL_RANGE && ns_index < 5)
        {
          /* Resets the path to find a smart way to get to the player */
          init_queue (&path);
          insert_queue (&path, origin_node);  /* Inserts first node in the queue */
          top_node = find_path (&objective, map, place_holder, &path, &ended_without_path);

          if (ended_without_path == 1)
          {
            build_obstacles (map, &top_node);
            node_saver[ns_index ++] = top_node;
          }
          else
          {
            simple_free (path);
          }
        }
        /** 
         * If there is no path to trap the player, this version of the map with
         * no enemies will be able to find something. The path will be checked later
         * for colisions enemy to enemy
         */
        if (ended_without_path == 0 || distance_from_player > D_INTEL_RANGE || ns_index > 4)
        {
          /* Resets path if it was not found but the player is still on land */
          init_queue (&path);
          insert_queue (&path, origin_node);  /* Inserts first node in the queue */
          top_node = find_path (&objective, map_static_obstacles, place_holder, &path, &ended_without_path);
          ended_without_path = 1;
        
          simple_free (path);
        }
        break;
      
      case S_CHAR:
        if (distance_from_player <= S_INTEL_RANGE && ns_index < 5)
        {
          /* Resets the path to find a smart way to get to the player */
          init_queue (&path);
          insert_queue (&path, origin_node);  /* Inserts first node in the queue */
          top_node = find_path (&objective, map, place_holder, &path, &ended_without_path);

          if (ended_without_path == 1)
          {
            build_obstacles (map, &top_node);
            node_saver[ns_index ++] = top_node;
          }
          else
          {
            simple_free (path);
          }
        }
        /** 
         * If there is no path to trap the player, this version of the map with
         * no enemies will be able to find something. The path will be checked later
         * for colisions enemy to enemy
         */
        if (ended_without_path == 0 || distance_from_player > S_INTEL_RANGE || ns_index > 4)
        {
          /* Resets path if it was not found but the player is still on land */
          init_queue (&path);
          insert_queue (&path, origin_node);  /* Inserts first node in the queue */
          top_node = find_path (&objective, map_static_obstacles, place_holder, &path, &ended_without_path);
          ended_without_path = 1;
        
          simple_free (path);
        }
        break;
      
      case G_CHAR:
        if (distance_from_player <= G_INTEL_RANGE && ns_index < 5)
        {
          /* Resets the path to find a smart way to get to the player */
          init_queue (&path);
          insert_queue (&path, origin_node);  /* Inserts first node in the queue */
          top_node = find_path (&objective, map, place_holder, &path, &ended_without_path);

          if (ended_without_path == 1)
          {
            build_obstacles (map, &top_node);
            node_saver[ns_index ++] = top_node;
          }
          else
          {
            simple_free (path);
          }
        }
        /**
         * If there is no path to trap the player, this version of the map with
         * no enemies will be able to find something. The path will be checked later
         * for colisions enemy to enemy
         */
        if (ended_without_path == 0 || distance_from_player > G_INTEL_RANGE || ns_index > 4)
        {
          /* Resets path if it was not found but the player is still on land */
          init_queue (&path);
          insert_queue (&path, origin_node);  /* Inserts first node in the queue */
          top_node = find_path (&objective, map_static_obstacles, place_holder, &path, &ended_without_path);
          ended_without_path = 1;
        
          simple_free (path);
        }
        break;
      /* For every run, at least one of the above is choosen */
      default:
        break;
      }
    }
    display_enemy_path (available, top_node, map, traveled_path, &(is_awake -> enemies_awaken[index]));
  }

  destroy_obstacles (map, node_saver, ns_index);
}
