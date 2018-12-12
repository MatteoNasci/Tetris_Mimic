#include "utils.h"
#include <string.h>
int precise_rand_lim(const int limit)
{
    int divisor = RAND_MAX / (limit + 1);
    int retval;

    do
    {
        retval = rand() / divisor;
    } while (retval > limit);

    return retval;
}
int quick_rand_range(const int min_n, const int max_n)
{
    return rand() % (max_n - min_n + 1) + min_n;
}
int min_value(int len, ...)
{
    va_list valist;
    int min = INT_MAX;
    register int i;

    va_start(valist, len);

    for (i = 0; i < len; i++)
    {
        int v = va_arg(valist, int);
        if (v < min)
        {
            min = v;
        }
    }

    va_end(valist);

    return min;
}
int max_value(int len, ...)
{
    va_list valist;
    int max = INT_MIN;
    register int i;

    va_start(valist, len);

    for (i = 0; i < len; i++)
    {
        int v = va_arg(valist, int);
        if (v > max)
        {
            max = v;
        }
    }

    va_end(valist);

    return max;
}