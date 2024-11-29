#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>

#define MAX_TASKS 10
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

void print_add_menu(char tasks[MAX_TASKS][TASK_SIZE], int *num_tasks){
  char input[TASK_SIZE +1];
  WINDOW *win = newwin(3, 40, 1, 45);

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

  input[TASK_SIZE -1] = '\0';
  if(strlen(input) > 0){
    strcpy(tasks[*num_tasks], input);
    (*num_tasks)++;
  }
  
  wclear(win);
  wrefresh(win);
  delwin(win);
}

void print_tasks_undone(WINDOW *win, int num_tasks,char tasks[MAX_TASKS][TASK_SIZE], int highlight){
  wclear(win); 
  box(win, 0, 0);
  wrefresh(win);
  if(num_tasks == 0){
      mvwprintw(win,1,1,"No tasks yet...");
  } else{
    for(int i = 0; i< num_tasks; i++){
      if(highlight == i){
        wattron(win, A_REVERSE | COLOR_PAIR(2));
      }
      wattron(win, COLOR_PAIR(2));
      mvwprintw(win, 1+i, 1,"[ ] - ");
      wattroff(win, COLOR_PAIR(2));
      mvwprintw(win, 1+i, 8, "%s", tasks[i]);
      wattroff(win, A_REVERSE | COLOR_PAIR(2));
    }
  }

  wrefresh(win);
}

void print_tasks_done(WINDOW *win, int num_tasks, char tasks_done[MAX_TASKS][TASK_SIZE],int highlight){
  wclear(win); 
  box(win, 0, 0);
  wrefresh(win);
  if(num_tasks == 0){
      mvwprintw(win,1,1,"No tasks yet...");
  }  else{
    for(int i = 0; i< num_tasks; i++){
      if(highlight == i){
        wattron(win, A_REVERSE | COLOR_PAIR(3));
      }
      wattron(win, COLOR_PAIR(2));
      mvwprintw(win, 1+i, 1,"[X] - ");
      wattroff(win, COLOR_PAIR(2));
      mvwprintw(win, 1+i, 8, "%s", tasks_done[i]);
      wattroff(win, A_REVERSE | COLOR_PAIR(3));
    }
  }

  

  wrefresh(win);
  }

void mark_task(char tasks[MAX_TASKS][TASK_SIZE], char tasks_done[MAX_TASKS][TASK_SIZE], int highlight2, int *num_tasks_done, int *num_tasks) {
    if (highlight2 < 0 || highlight2 >= *num_tasks) {
        return;
    }

    strcpy(tasks_done[*num_tasks_done], tasks[highlight2]);
    (*num_tasks_done)++;

    for (int i = highlight2; i < *num_tasks - 1; i++) {
        strcpy(tasks[i], tasks[i + 1]);
    }
    (*num_tasks)--;
}




void dismark_task(char tasks_done[MAX_TASKS][TASK_SIZE], char tasks[MAX_TASKS][TASK_SIZE], int highlight3, int *num_tasks, int *num_tasks_done) {
    if (highlight3 < 0 || highlight3 >= *num_tasks_done) { 
        return;
    }

    strcpy(tasks[*num_tasks], tasks_done[highlight3]);
    (*num_tasks)++;

    for (int i = highlight3; i < *num_tasks_done - 1; i++) {
        strcpy(tasks_done[i], tasks_done[i + 1]);
    }
    (*num_tasks_done)--;
}





int main(void){
  noecho();
  initscr();
  curs_set(0);  
  start_color();
  init_pair(1, COLOR_GREEN, COLOR_MAGENTA);
  init_pair(2, COLOR_GREEN, COLOR_BLACK);
  init_pair(3, COLOR_WHITE, COLOR_BLACK);
  cbreak();
  keypad(stdscr, TRUE);
  refresh();
  char tasks[MAX_TASKS][TASK_SIZE];
  char tasks_done[MAX_TASKS][TASK_SIZE];
  int num_tasks = 0;
  int num_tasks_done = 0;

  const char *menu[] = {
    "TASKS",
    "COMPLETED",
    "ADD TASK"
  };

  int highlight = 0;
  int n_choices = 3;
  int key;
  int choice = -1;
  
  int highlight2 = 0;
  int choice2 = -1;
  
  int highlight3 = 0;

  WINDOW *menu_win = newwin(3, 41, 1, 1);
  WINDOW *tasks_win = newwin(20,27,4,1);


  box(menu_win, 0, 0);
  print_tasks_undone(tasks_win,num_tasks,tasks, highlight2);
  while(1){
    print_menu(menu_win, highlight ,menu, n_choices);
    wrefresh(menu_win);
    noecho();
    key = getch();
    switch (key) {
      case KEY_LEFT:
          if(highlight > 0)
            highlight--;
          if(highlight == 0)
            print_tasks_undone(tasks_win, num_tasks, tasks, highlight2);
          if(highlight==1)
            print_tasks_done(tasks_win, num_tasks_done,tasks_done, highlight3);
          break;
      case KEY_DOWN:
        if(highlight==0){
          if(highlight2 < num_tasks-1){
            highlight2++;
            print_tasks_undone(tasks_win, num_tasks, tasks, highlight2);
          }  
        }
        if(highlight == 1){
          if(highlight3 < num_tasks_done-1){
            highlight3++;
            print_tasks_done(tasks_win, num_tasks_done, tasks_done, highlight3);
          } 
        } break;

      case KEY_UP:
        if(highlight==0){
          if(highlight2 > 0){
            highlight2--;
            print_tasks_undone(tasks_win, num_tasks, tasks, highlight2);
          } 
        }
        if(highlight==1){
          if(highlight3 > 0){
              highlight3--;
              print_tasks_done(tasks_win, num_tasks_done, tasks_done, highlight3);
          } 

        } break;
      case  KEY_RIGHT:
          if(highlight < n_choices -1){
            highlight++;
          if(highlight == 1)
              print_tasks_done(tasks_win, num_tasks_done, tasks_done, highlight3);
          if(highlight == 2)
              print_tasks_undone(tasks_win, num_tasks, tasks, highlight2);
          } break;
      case 10:
        choice = highlight;
        if(choice == 2){
          print_add_menu(tasks, &num_tasks);
          print_tasks_undone(tasks_win, num_tasks, tasks, highlight2);
          print_menu(menu_win, highlight,menu, n_choices);
        } if(choice == 0 && num_tasks > 0){
            mark_task(tasks,tasks_done,highlight2,&num_tasks_done, &num_tasks);
            highlight2 = 0;
            highlight3 = 0;
            print_tasks_undone(tasks_win, num_tasks, tasks, highlight2);
        } if(choice == 1 && num_tasks_done > 0){
            dismark_task(tasks_done,tasks,highlight3,&num_tasks,&num_tasks_done);
            highlight3 = 0;
            highlight2 = 0;
            print_tasks_done(tasks_win,num_tasks_done,tasks_done, highlight3);
        }
        break;
      case 'q':
        endwin();
        return 0;
    
    }
  }

  return 0;
}


