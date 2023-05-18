#include "defines.h"
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

char *choices[] = {
  "START",
  "HOW TO PLAY",
  "QUIT"};
int n_choices = sizeof(choices) / sizeof(char *);

char *instructions[]={
  "                                                                                                  ",
  "    88888888888                                                                    88888888888    ",   
  "  888888888888888                                                                888888888888888  ",   
  "  888888888888888                                                                888888888888888  ",   
  "  88   88888   88   8  8  8888  8     8    88888  8888    8888  8     8888  8 8  88   88888   88  ",      
  "  88   88888   88   8888  8  8   8 8 8       8    8  8    8 _8  8     8 _8   8   88   88888   88  ", 
  "  888888888888888   8  8  8888    8 8        8    8888    8     8888  8  8   8   888888888888888  ",   
  "  888888888888888                                                                888888888888888  ",   
  "  888888888888888                                                                888888888888888  ",   
  "    88 88 88 88                                                                    88 88 88 88    ", 
  "    88 88 88 88                                                                    88 88 88 88    ",
  "                                                                                                  ",            
  "                                                                                                  ",
  "  Explore the dungeon!                                                                            ",
  "  The map will start hidden and you discover it as you walk. Walk through the dungeon,            ",
  " using the arrow keys. The map is randomly generated each time you play. You'll find              ",
  " different enemies, obstacles, and treasures as you explore.                                      ",
  "                                                                                                  ",
  "  Manage your resources                                                                           ",
  "  In a roguelike game, resources are scarce and there are no respawns . Keep an eye on            ",
  " your health, XP and inventory to make sure you have everything you need to survive.              ",
  "                                                                                                  ",
  "  Progression                                                                                     ",
  "  As you explore the dungeon and defeat enemies, you'll gain XP. This will improve your           ",
  " shield making you more powerful and better equipped to face the challenges ahead.                ",
  "                                                                                                  ",
  "  Each game is unique and challenging in its own way. Explore, strategize, and above all,         ",
  " have fun!                                                                                        ",
  "                                                                                                  ",
  "                                                                                                  "};
int n_instructions = sizeof(instructions)/ sizeof(char *);

/************************************************
* Function that prints the menu on the screen
************************************************/

char *titulo[] = {

"  88        88    888      88       88       8888888   88888    ",
"  88   88   88   88 88     88       88       88        88   88  ",
"   88 8888 88   88   88    88       88       888       88    8  ",
"    888  888   888888888   88       88       88        88   88  ",
"     88  88   88       88  8888888  8888888  8888888   88888    ",

"                    88888    8888888  8888888                   ",
"                   88   88   88       88                        ",
"                   88   88   8888     8888                      ",
"                   88   88   88       88                        ",
"                    88888    88       88                        "};

int n_titulo = sizeof(titulo)/ sizeof(char *);

void print_menu(WINDOW *menu_win, int highlight)
{
  int x, y, i;

  x = (MAP_WIDTH-12)/2;
  y = MAP_HEIGHT/2 ;

  /* Set the background color of the menu window */
  wbkgd(menu_win, COLOR_PAIR(BACKGROUND_COLOR));

  /* Draw a box around the menu */
  box(menu_win, 0, 0);

  /* Print titulo*/
  int x1 = (MAP_WIDTH - 65)/2;
  int y1 = 2;
  wattron(menu_win, COLOR_PAIR(TITLE_COLOR));
  for (i = 0; i < n_titulo; ++i)
  {
    /* Highlight the current choice */
    mvwprintw(menu_win, y1, x1, "%s", titulo[i]);
    y1++;
  }
  wattroff(menu_win, COLOR_PAIR(TITLE_COLOR));

  /* Print each option */
  for (i = 0; i < n_choices; ++i)
  {
    if (highlight == i + 1)
    { /* Highlight the current choice */
      wattron(menu_win, COLOR_PAIR(SELECTED_OPTION_COLOR));
      mvwprintw(menu_win, y, x, "%s", choices[i]);
      wattroff(menu_win, COLOR_PAIR(SELECTED_OPTION_COLOR));
    }
    else
    { /* Don't highlight other choices */
      wattron(menu_win, COLOR_PAIR(BACKGROUND_COLOR));
      mvwprintw(menu_win, y, x, "%s", choices[i]);
      wattroff(menu_win, COLOR_PAIR(BACKGROUND_COLOR));
    }
    ++y;
  }
  /* Update the menu display */
  wrefresh(menu_win);
}

/*****************************************************
 * Display instructions when "HOW TO PLAY" is selected 
*****************************************************/
void print_instructions (WINDOW *menu_win)
{
  werase(menu_win); /*clear the window*/
  int x = 1;
  int y = 1;

  /* Set the background color of the instuctions window */
  wbkgd(menu_win, COLOR_PAIR(BACKGROUND_COLOR));

  /* Draw a box */
  box(menu_win, 0, 0);


 /* Print the instructions */
  wattron(menu_win, COLOR_PAIR(BACKGROUND_COLOR));
  for (int i = 0; i < n_instructions; ++i){
    mvwprintw(menu_win, y, x, "%s", instructions[i]);
    ++y;
  }
  wattroff(menu_win, COLOR_PAIR(BACKGROUND_COLOR));

  /* Display the "Back to menu" button */
  wattron(menu_win, COLOR_PAIR(SELECTED_OPTION_COLOR));
  mvwprintw(menu_win, MAP_HEIGHT-2, MAP_WIDTH-20, "BACK TO MENU"); /*Print on bottom right corner*/
  wattroff(menu_win, COLOR_PAIR(SELECTED_OPTION_COLOR));

  /* Update the menu display */
  wrefresh(menu_win);

  while (1)
  {
    switch (wgetch(menu_win))
    {
    case 10: /*KEY_ENTER*/
      /* User selected "Back to menu" */
      werase(menu_win); /*clear the window*/
      print_menu(menu_win, 2); /*print the menu with the option "HOW TO PLAY" highlight*/
      return;
    default:
      /* Ignore other keys */
      break;
    }
  }
}

/*********************************************************
 * function that is activated when press ENTER_KEY and 
 * selects the way the game should go acording the selected
 * option
*********************************************************/
void select_way (WINDOW *menu_win, int choice, int *in_game, int *in_menu){
  /* Handle choice */
  switch (choice)
  {
  case 1:
    /* Quit the menu, start game */
    *in_menu = 1;
    *in_game = 0;
    break;
  case 2:
    /* Print instructions */
    print_instructions(menu_win);
    break;
  case 3:
    /* Quit game */
    *in_menu = 1;
    *in_game = 1;
  }
}

/*********************************************************
* Main function
*********************************************************/
int start_menu (void)
{
  WINDOW *menu_win;
  int highlight = 1; /*opção selecionada*/
  int choice = 0; 
  int in_game = 1;
  int in_menu = 0;
  int startx = 0;
  int starty = 0;

  /* Create menu window */
  menu_win = newwin(MAP_HEIGHT, MAP_WIDTH, starty, startx);
  keypad(menu_win, TRUE);

  /* Print menu */
  print_menu(menu_win, highlight);

  /* Loop through menu */
  while (in_menu == 0)
  {
    switch (wgetch(menu_win))
    {
    case KEY_UP:
      if (highlight == 1){ /* case first option and KEY_UP pass for the last option */
        highlight = n_choices;
      }
      else{
        highlight--;
      }
      break;
    case KEY_DOWN:
      if (highlight == n_choices){ /* case last option and KEY_DOWN pass for the first option */  
        highlight = 1;
      }
      else {
        highlight++;
      }
      break;
    case 10: /*KEY_ENTER*/
    /* define the selected option */
      choice = highlight;
      select_way(menu_win, choice, &in_game, &in_menu);
      break;
    default:
      refresh();
      break;
    }
    print_menu(menu_win, highlight);
  }

  /* Cleanup and exit */
  werase (menu_win); /* clear the window  */
  wrefresh (menu_win); /* Refresh */
  delwin (menu_win);  /* Delete */
  endwin ();

  return in_game; /* Continues to the game if it equals to 0 */
}