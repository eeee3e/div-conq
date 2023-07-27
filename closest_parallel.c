#include "closest_parallel.h"

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <sys/wait.h>

int curr_depth = 0;

double _closest_parallel(struct Point P[], size_t n, int pdmax, int *pcount)
{
    static int num_forks = 0;
    
    qsort(P, n, sizeof(struct Point), compare_x); // it says to in the instructions ¯\_(ツ)_/¯

    if (n <= 3 || pdmax == 0) {
        return _closest_serial(P, n);
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
            //fprintf(stderr, "Point: {%d, %d}\n", P[i].x, P[i].y);
        }
        else if (i >= mi) {
            arrr[cr++] = P[i];
            //fprintf(stderr, "Point: {%d, %d}\n", P[i].x, P[i].y);
        }
    }

    int child1[2], child2[2];

    if (pipe(child1) == -1) { perror("pipe"); exit(1); }
    if (pipe(child2) == -1) { perror("pipe"); exit(1); }
    int wstatus;
    num_forks++;
    pid_t p = fork();
    curr_depth++;
    
    if (p < 0) { perror("fork"); exit(1); }
    else if (p == 0) {
        // Child to solve left side.
        if (close(child1[0]) == -1) { perror("close"); exit(1); }
        if (close(child2[0]) == -1) { perror("close"); exit(1); }
        if (close(child2[1]) == -1) { perror("close"); exit(1); }

        //*pcount += 1;
        double dl = _closest_parallel(arrl, cl, pdmax-1, pcount);
        if (write(child1[1], &dl, sizeof(double)) == -1) { 
            perror("write"); 
            exit(1);
        }
        if (close(child1[1]) == -1) { perror("close"); exit(1); }
        free(arrl);
        free(arrr);
        //*pcount++;
        //fprintf(stderr, "num_forks: %d\n", num_forks);
        exit(*pcount + 1);
    }
    else { // Parent.
        //num_forks++;
        pid_t q = fork();
        
        if (q < 0) { perror("fork"); exit(1); }
        else if (q == 0) {
            // Child to solve right side.
            if (close(child1[0]) == -1) { perror("close"); exit(1); }
            if (close(child2[0]) == -1) { perror("close"); exit(1); }
            if (close(child1[1]) == -1) { perror("close"); exit(1); }
            
            //*pcount += 1;
            double dr = _closest_parallel(arrr, cr, pdmax-1, pcount);
            if (write(child2[1], &dr, sizeof(double)) == -1) { 
                perror("write"); 
            }
            if (close(child2[1]) == -1) { perror("close"); exit(1); }
            free(arrl);
            free(arrr);
            //*pcount++;
            //fprintf(stderr, "num_forks: %d\n", num_forks);
            exit(*pcount + 1);
        }
        else { // Parent.
            if (close(child1[1]) == -1) { perror("close"); exit(1); }
            if (close(child2[1]) == -1) { perror("close"); exit(1); }
            double dl, dr, db;
            //int x = 0;
            if (read(child1[0], &dl, sizeof(double)) == 0) {
                perror("read"); 
                exit(1);
            }
            if (close(child1[0]) == -1) { perror("close"); exit(1); }
            free(arrl);
            
            if (read(child2[0], &dr, sizeof(double)) == 0) {
                perror("read"); 
                exit(1);
            }
            if (close(child2[0]) == -1) { perror("close"); exit(1); }
            free(arrr);

            if (waitpid(p, &wstatus, 0) == -1) { 
                perror("waitpid");
                exit(1);
            }
            if (WIFEXITED(wstatus)) { *pcount += WEXITSTATUS(wstatus); }
            if (waitpid(q, &wstatus, 0) == -1) {
                perror("waitpid");
                exit(1);                
            }
            if (WIFEXITED(wstatus)) { *pcount += WEXITSTATUS(wstatus); }
            
            //fprintf(stderr, "pcount: %d\n", *pcount);
            

            if (dr >= dl) { 
                db = combine_lr(P, n, p_mid, dl); 
                if (db >= dl) { return dl; }
                else { return db; }
            }
            else { 
                db = combine_lr(P, n, p_mid, dr); 
                if (db >= dr) { return dr; }
                else { return db; }
            }

        }
    }
}

double closest_parallel(struct Point P[], size_t n, int pdmax, int *pcount)
{   
    
    qsort(P, n, sizeof(struct Point), compare_x);

    return _closest_parallel(P, n, pdmax, pcount);
    
    //return 0.0;
}
