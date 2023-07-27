#include "closest_brute.h"

#include <math.h>
#include <stdio.h>

double brute_force(struct Point P[], size_t n) 
{ 
    int i, j;
    struct Point a, b;
    double low = INFINITY;
    //fprintf(stderr, "%f\n", low);
    double d;
    for (i = 0; i < n; i++) {
        for (j = i + 1; j < n; j++) {
            a = P[i];
            b = P[j];
            //fprintf(stderr, "i: %d, j: %d\n", i, j);
            d = dist(a, b);
            //fprintf(stderr, "Dist: %f\n", d);
            if (d < low) {
                low = d;
                //fprintf(stderr, "low: %f\n", low);
            }  
        }
    }
    return low; 
} 
