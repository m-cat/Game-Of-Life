#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include <time.h>
#include "util.h"

int con_init() {
  initscr();

  keypad(stdscr, TRUE);  /* Allow reading arrow keys */
  curs_set(0);           /* Make the cursor invisible */
  noecho();              /* Suppress echoing of user input */
  raw();                 /* Disable line buffering */
  nodelay(stdscr, TRUE); /* Disable delay on getch() */
  mousemask(BUTTON1_CLICKED, NULL); /* Enable mouse events */
  srand(time(NULL));     /* Initialize seed to rand() */

  return 0;
}

int kbhit() {
  int ch = getch();

  if (ch != ERR) {
    return ch;
  }
  return 0;
}

int msleep(int ms) {
  struct timespec t1, t2;
  t1.tv_sec = 0;
  t1.tv_nsec = ms*1000000;
  nanosleep(&t1, &t2);

  return 0;
}

int rand_int(int min_n, int max_n)
{
    return rand() % (max_n - min_n + 1) + min_n;
}

double rand_float(double min_n, double max_n)
{
    return (double)rand()/RAND_MAX * (max_n - min_n) + min_n;
}
