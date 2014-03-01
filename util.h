#ifndef UTIL_H
#define UTIL_H

#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

int con_init();
int kbhit();
int msleep(int ms);
int rand_int(int min_n, int max_n);
double rand_float(double min_n, double max_n);

#endif
