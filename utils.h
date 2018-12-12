#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <limits.h>
#define MIN(x, y) x > y ? y : x;
#define MAX(x, y) x > y ? x : y;
#define IS_INCLUDED_INCLUSIVE(v, min, max) v >= min &&v <= max
#define IS_INCLUDED(v, min, max) v > min &&v < max
#define IS_EXCLUDED_INCLUSIVE(v, min, max) v <= min || v >= max
#define IS_EXCLUDED(v, min, max) v<min || v> max
int precise_rand_lim(const int limit_inclusive);
int quick_rand_range(const int min_inclusive, const int max_inclusive);
int min_value(int len, ...);
int max_value(int len, ...);