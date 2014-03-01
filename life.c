#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <unistd.h>
#include "util.h"

int main(int argc, char **argv) {
  char **field, **field2;
  MEVENT event;
  int i, j, a, b, n, k, con_width, con_height, top_border=1, FPS=5;
  float d = 0.2, g = -1.0;
  char c = '#';
  char msg1[80];
  char msg2[80];
  strcpy(msg1, "GAME OF LIFE ~~ Editing... (edit with mouse, run with space)");
  strcpy(msg2, "GAME OF LIFE ~~ Running... (pause with space, quit with 'q')");
  
  /* Process command line options */
  opterr = 0;
  while ((k = getopt(argc, argv, "i:d:c:g:")) != -1) {
    switch (k)
      {
      case 'i':
	FPS = atoi(optarg);
	if (!(FPS > 0 && FPS <= 100)) {
	  fprintf(stderr, "Error: argument to -i must be >0 and <=100.\n");
	  return 1;
	}
	break;
      case 'd':
	d = atof(optarg);
	if (d < 0 || d > 1) {
	  fprintf(stderr, "Error: argument to -f must be between 0 and 1.\n");
	  return 1;
	}
	break;
      case 'c':
	c = optarg[0];
	break;
      case 'g':
	g = atof(optarg);
	if (g < 0 || g > 1) {
	  fprintf(stderr, "Error: argument to -f must be between 0 and 1.\n");
	  return 1;
	}
	break;
      case '?':
	if (optopt == 'i' || optopt == 'd' || optopt == 'c' || optopt == 'g')
	  fprintf(stderr, "Error: Option -%c requires an argument.\n", optopt);
	else if (isprint(optopt))
	  fprintf(stderr, "Error: Unknown option '-%c'.\n", optopt);
	else
	  fprintf(stderr, "Error: Unknown option character '\\x%x'.\n", optopt);
	return 1;
      default:
	abort();
      }
  }

  /* Initialize ncurses */
  con_init();
  getmaxyx(stdscr,con_height,con_width);
  /* Truncate messages if they can't fit in console */
  //if (con_width < strlen(msg1))
  //  msg1[con_width-1] = 0;
  if (con_width < strlen(msg2))
    msg2[con_width-1] = 0;
  /* Allocate game field */
  field = malloc(con_height * sizeof(char *));
  field2 = malloc(con_height * sizeof(char *));
  for (i = 0; i < con_height; i++) {
    field[i] = calloc(con_width, 1); /* initialize all elements to 0 */
    field2[i] = malloc(con_width);
  }
  if (d != -1.0) {
    for (i = top_border; i < con_height; i++)
      for (j = 0; j < con_width; j++) {
	field[i][j] = (rand_float(0,1) < d);
	mvprintw(i, j, "%c", field[i][j] ? c : ' ');
      }
  }

 input:
  /* Editing mode */
  mvprintw(0, 0, "%s", msg1);
  refresh();
  while ((k = kbhit()) != ' ') {
    if (k == KEY_MOUSE) {
      if (getmouse(&event) == OK && event.y > top_border-1) {
	field[event.y][event.x] = !field[event.y][event.x];
	mvprintw(event.y, event.x, "%c", field[event.y][event.x] ? c : ' ');
	refresh();
      }
    }
  }

  /* Run mode */
  mvprintw(0, 0, "%s", msg2);
  refresh();
  while ((k = kbhit()) != ' ' && k != 'q') {
    for (i = top_border; i < con_height; i++) {
      for (j = 0; j < con_width; j++) {
	n = 0;
	/* Count neighbors */
	for (a = max(top_border, i-1); a <= min(con_height-1, i+1); a++)
	  for (b = max(0, j-1); b <= min(con_width-1, j+1); b++)
	    if (!(a==i && b==j))
	      n += field[a][b];
	/* Apply GoL rules */
	field[i][j] ? (field2[i][j] = (n==2 || n==3)) : (field2[i][j] = (n==3));
	/* Spontaneous generation (if option is set) */
	if (!field2[i][j] && g != -1.0 && rand_float(0,1) < g)
	  field2[i][j] = 1;
      }
    }
    /* Print current generation */
    for (i = 1; i < con_height; i++)
      for (j = 0; j < con_width; j++) {
	field[i][j] = field2[i][j];
	mvprintw(i, j, "%c", field[i][j] ? c : ' ');
      }
 
    refresh();
    msleep(1000/FPS); /* naive fps implementation */
  }
  if (k == ' ')
    goto input;

  /* Cleanup and exit */
  for (i = top_border; i < con_height; i++) {
    free(field[i]);
    free(field2[i]);
  }
  free(field);
  free(field2);
  endwin();
  return 0;
}
