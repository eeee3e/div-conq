#include "closest_helpers.h"

#include <math.h>

int verbose = 0;

int compare_x(const void* a, const void* b) 
{   
    int x = (*(struct Point*)a).x;
    int y = (*(struct Point*)b).x;
    
    return x < y ? -1 : x > y ? 1 : 0;

} 

int compare_y(const void* a, const void* b) 
{ 
    int x = (*(struct Point*)a).y;
    int y = (*(struct Point*)b).y;
    
    return x < y ? -1 : x > y ? 1 : 0;
} 

double dist(struct Point p1, struct Point p2) 
{
    double d, xsq, ysq;

    xsq = pow((p2.x - p1.x), 2);
    ysq = pow((p2.y - p1.y), 2);
     
    d = sqrt(xsq + ysq);

    return d;
} 
