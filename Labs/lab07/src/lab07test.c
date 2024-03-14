#include <stdio.h>
#include <assert.h>
#include "lab07.c"

// Test cases, testing differences in e value, specifically when e=10^-3 to 10^-7
void testSerial(double* x, double e){
    double u=e;
    printf("\nTesting pi_serial with different e values from e=10^-3 to 10^-7\n");
    for (int i=0; i<4; i++){
        pi_serial(x, u);
        printf("Pi vlaue: %f error by e: %.7f\n", *x, u);
        u = u*10;
    }
    return;
}

// Test cases, testing different number of threads from 1-12
// testing e=10^-4 to 10^-7 as well for all thread levels
void testMutex(double* x, double e){
    double u=e;
    for (int t=0; t<3; t++){
        printf("\nTesting pi_mutex function\n");
        for(int i=1; i<=12; i++){
            pi_mutex(x, i, u);
            printf("Number of threads: %d, pi: %f, error of e: %.7f\n", i, *x, u);
        }
        u = u*10;
    }
    return;
}

// Test cases, testing different number of threads from 1-12
// testing e=10^-4 to 10^-7 as well for all thread levels
void testSemaphore(double* x, double e){
    double u=e;
    for (int t=0; t<3; t++){
    printf("\nTesting pi_semaphore function\n");
        for(int i=1; i<=12; i++){
            pi_semaphore(x, i, u);
            printf("Number of threads: %d, pi: %f, error of e: %.7f\n", i, *x, u);
        }
        u = u*10;
    }
    return;
}

int main(){
    double e=0.0000001;
    double pi=0;
    double *y=&pi;

    testSerial(y, e);
    testMutex(y, e);
    testSemaphore(y, e);

    exit(EXIT_SUCCESS);

    return 0;
}