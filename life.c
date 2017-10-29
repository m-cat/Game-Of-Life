#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <unistd.h>
#include "util.h"

int main(int argc, char **argv) {
  char **field, **field2;
  MEVENT event;
  int i, j, a, b, a2, b2, n, k, w=0;
  int con_width, con_height, top_border=1, FPS=5, iter=0;
  float d = 0.2, g = -1.0;
  int B[9] = {0, 0, 0, 1, 0, 0, 0, 0, 0}; /* default rules for being born */
  int S[9] = {0, 0, 1, 1, 0, 0, 0, 0, 0}; /* default rules for staying alive */
  char c = '#';
  char *msg_edit = "GAME OF LIFE ~~ Editing... (edit with mouse, run with space)";
  char *msg_run =  "GAME OF LIFE ~~ Running... (pause with space, quit with 'q')";
  char msg[80];
  char buf[40];

  /* Process command line options */
  opterr = 0;
  while ((k = getopt(argc, argv, "i:d:c:g:wb:s:")) != -1) {
    switch (k)
      {
      case 'i': /* Iterations per second */
        FPS = atoi(optarg);
        if (!(FPS > 0 && FPS <= 100)) {
          fprintf(stderr, "Error: argument to -i must be >0 and <=100.\n");
          return 1;
        }
        break;
      case 'd': /* Initial density */
        d = atof(optarg);
        if (d < 0 || d > 1) {
          fprintf(stderr, "Error: argument to -f must be between 0 and 1.\n");
          return 1;
        }
        break;
      case 'c': /* Display character for cells */
        c = optarg[0];
        break;
      case 'g': /* Spontaneous generation parameter */
        g = atof(optarg);
        if (g < 0 || g > 1) {
          fprintf(stderr, "Error: argument to -f must be between 0 and 1.\n");
          return 1;
        }
        break;
      case 'w': /* Wrapping */
        w = 1;
        break;
      case 'b': /* B rules */
        B[3] = 0;
        i = atoi(optarg);
        if (i < 0)
          break;
        if (strlen(optarg) > 9) {
          fprintf(stderr, "Error: invalid arguments to b.\n");
          return 1;
        }
        while (i > 0)
          B[i % 10] = 1, i /= 10;
        if (optarg[0] == '0')
          B[0] = 1;
        break;
      case 's': /* S rules */
        S[2] = 0; S[3] = 0;
        i = atoi(optarg);
        if (i < 0)
          break;
        if (strlen(optarg) > 9) {
          fprintf(stderr, "Error: invalid arguments to s.\n");
          return 1;
        }
        while (i > 0)
          S[i % 10] = 1, i /= 10;
        if (optarg[0] == '0')
          S[0] = 1;
        break;
      case '?':
        if (optopt == 'i' || optopt == 'd' || optopt == 'c' || optopt == 'g' ||
            optopt == 'b' || optopt == 's')
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
  getmaxyx(stdscr, con_height, con_width);
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
  strcpy(msg, msg_edit);
  if (con_width < strlen(msg))
    msg[con_width] = 0;
  mvprintw(0, 0, "%s", msg);
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
  while ((k = kbhit()) != ' ' && k != 'q') {
    iter ++;
    strcpy(msg, msg_run);
    strcat(msg, " ~~ Iteration: ");
    itoa(buf, 10, iter);
    strcat(msg, buf);
    if (con_width < strlen(msg))
      msg[con_width] = 0;
    mvprintw(0, 0, "%s", msg);

    for (i = top_border; i < con_height; i++) {
      for (j = 0; j < con_width; j++) {
        n = 0;

        /* Count neighbors */
        for (a = (w ? i-1 : max(top_border, i-1));
             a <= (w ? i+1 : min(con_height-1, i+1));
             a++) {
          for (b = (w ? j-1 : max(0,j-1));
               b <= (w ? j+1 : min(con_width-1, j+1));
               b++) {
            a2 = (a < top_border) ? con_height-1 :
              ((a >= con_height) ? top_border : a);
            b2 = (b < 0) ? con_width-1 : ((b >= con_width) ? 0 : b);
            if (!(a2 == i && b2 == j))
              n += field[a2][b2];
          }
        }

        /* Apply GoL rules */
        field[i][j] ? (field2[i][j] = S[n]) : (field2[i][j] = B[n]);

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
