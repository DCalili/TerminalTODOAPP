#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>
#define TASK_SIZE 20

// Author: Dcalili
// Date: 2024-11-28
//

void print_menu(WINDOW *win, int highlight, const char *choices[],int n_choices){
  int startx = 1, starty = 1;
  for(int i = 0; i < n_choices; i++){
    if(i == highlight){
      wattron(win, A_REVERSE | COLOR_PAIR(1));
    }
    mvwprintw(win, starty, startx, "%s", choices[i]);
    wattroff(win,A_REVERSE | COLOR_PAIR(1));
    startx+=15;
  }
  wrefresh(win);

}

void print_add_menu(WINDOW *win){
  char input[TASK_SIZE-1];
  wclear(win); 
  box(win, 0, 0);
  wrefresh(win);
  bool valid_input = FALSE;
  while(!valid_input){
    wclear(win);
    box(win, 0,0);
    mvwprintw(win, 1, 1, "ADD TASK:");
    wrefresh(win);

    echo();
    wgetnstr(win, input, TASK_SIZE);
    noecho();

    int len = strlen(input);

    if(len >= TASK_SIZE){
      valid_input = FALSE;
    } else{
      valid_input = TRUE;
    }
    wrefresh(win);

  }
  
  wclear(win);
  wrefresh(win);
  delwin(win);
}

void print_tasks_undone(WINDOW *win, int num_tasks){
  wclear(win); 
  box(win, 0, 0);
  wrefresh(win);
  if(num_tasks == 0){
      mvwprintw(win,1,1,"No tasks yet...");
  }

  wrefresh(win);
}

void print_tasks_done(WINDOW *win, int num_tasks){
  wclear(win); 
  box(win, 0, 0);
  wrefresh(win);
  if(num_tasks == 0){
      mvwprintw(win,1,1,"No tasks yet...");
  } else{
      mvwprintw(win,1,1,"XESQUEDELE");
  }

  wrefresh(win);
}



int main(void){
  noecho();
  initscr();
  curs_set(0);  
  start_color();
  init_pair(1, COLOR_GREEN, COLOR_MAGENTA);
  init_pair(2, COLOR_GREEN, COLOR_BLACK);
  cbreak();
  keypad(stdscr, TRUE);
  refresh();


  const char *menu[] = {
    "TASKS",
    "COMPLETED"
  };

  int highlight = 0;
  int n_choices = 2;
  int key;
  int choice = -1;
  WINDOW *menu_win = newwin(3, 27, 1, 1);
  WINDOW *add_win = newwin(3, 40, 1, 30);
  WINDOW *tasks_win = newwin(20,27,4,1);
  box(menu_win, 0, 0);
  print_tasks_undone(tasks_win,0);
  while(1){
    print_menu(menu_win, highlight ,menu, n_choices);
    wrefresh(menu_win);
    noecho();
    key = getch();
    switch (key) {
      case KEY_LEFT:
          if(highlight > 0){
            highlight--;
            print_tasks_undone(tasks_win, 0);
          }   break;
      case  KEY_RIGHT:
          if(highlight < n_choices -1){
            highlight++;
            print_tasks_done(tasks_win, 1);
          } break;
      case 10:
        choice = highlight;
        if(choice == 0){
          print_add_menu(add_win);  
        } else{
          printw("Tchau");
        }
        break;
      case 'q':
        endwin();
        return 0;
    
    }
  }

  return 0;
}


