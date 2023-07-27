#include "closest_serial.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

double combine_lr(struct Point P[], size_t n, struct Point mid_point, double d)
{
    //struct Point arr[n];
    struct Point* arr = NULL;
    arr = malloc(sizeof(struct Point)*n);

    int i;
    size_t n1 = 0;

    for (i = 0; i < n; i++) {
        if (abs(P[i].x - mid_point.x) < d) {
            arr[n1++] = P[i];
            //fprintf(stderr, "Inside d: {%d, %d}\n", P[i].x, P[i].y);
        }
    }

    qsort(arr, n1, sizeof(struct Point), compare_y);

    int j, k;

    double low = INFINITY;
    double dret;

    for (j = 0; j < n1; j++) {
        for (k = i + 1; k < n1; k++) {
            if ((arr[k].y - arr[j].y) < d) {
                dret = dist(arr[j], arr[k]);
                if (dret < low) {
                    low = dret;
                }
            }
        }
    }
    free(arr);

    return low;
}

double _closest_serial(struct Point P[], size_t n)
{   
    if (n <= 3) {
        return brute_force(P, n);
    }

    int mi = floor(n/2);
    struct Point p_mid = P[mi];
    //fprintf(stderr, "midpoint: {%d, %d}\n", p_mid.x, p_mid.y);
    struct Point* arrl = malloc(sizeof(struct Point)*mi);
    struct Point* arrr = malloc(sizeof(struct Point)*(n-mi));
    int i;
    int cl = 0;
    int cr = 0;
    for (i = 0; i < n; i++) {
        if (i < mi) {
            arrl[cl++] = P[i];
            //fprintf(stderr, "Point: {%d, %d}\n", arrl[cl].x, arrl[cl].y);
        }
        else if (i >= mi) {
            arrr[cr++] = P[i];
            //fprintf(stderr, "Point: {%d, %d}\n", P[i].x, P[i].y);
        }
    }
    
    
    double dl, dr;
    //size_t nl = sizeof(*arrl) / sizeof((*arrl)[0]);
    size_t nl = mi;
    //fprintf(stderr, "nl: %lu\n", nl);
    //size_t nr = sizeof(*arrr) / sizeof((*arrr)[0]);
    size_t nr = n-mi;
    //fprintf(stderr, "nr: %lu\n", nr);

    
    if (cl < 4 && cr > 3) {
        dl = brute_force(arrl, nl);
        //fprintf(stderr, "dl: %f\n", dl);
        dr = _closest_serial(arrr, nr);
        //fprintf(stderr, "dr: %f\n", dr);
    }
    else if (cl > 3 && cr < 4) {
        dl = _closest_serial(arrl, nl);
        //fprintf(stderr, "dl: %f\n", dl);
        dr = brute_force(arrr, nr);
        //fprintf(stderr, "dr: %f\n", dr);
    }
    else if (cl > 3 && cr > 3) {
        dl = _closest_serial(arrl, nl);
        //fprintf(stderr, "dl: %f\n", dl);
        dr = _closest_serial(arrr, nr);
        //fprintf(stderr, "dr: %f\n", dr);
    }
    else {
        dl = brute_force(arrl, nl);
        //fprintf(stderr, "dl: %f\n", dl);
        dr = brute_force(arrr, nr);
        //fprintf(stderr, "dr: %f\n", dr);
    }
    
    free(arrl);
    free(arrr);

    double d;
    if (dl >= dr) {
        d = dr;
    } 
    else {
        d = dl;
    }

    double db;
    db = combine_lr(P, n, p_mid, d);
    //db = 1.11;


    double dret;
    if (db >= d) {
        dret = d;
    } 
    else {
        dret = db;
    }


    return dret;
    
    //return 0.0;
}

double closest_serial(struct Point P[], size_t n)
{
    qsort(P, n, sizeof(struct Point), compare_x);
    return _closest_serial(P, n);
}
