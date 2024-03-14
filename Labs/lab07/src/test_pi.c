//Lab7 Ben Morley

#include<stdio.h>
#include<assert.h>
#include"pi_calc.c"
#include<math.h>

// M_PI was not working on my linux system, not sure why.
#ifndef M_PI
#define M_PI acos(-1)
#endif



int main(int argc, char* argv[]){

	int nprocs = get_nprocs();
	long double error1 = 1E-10;
    long double error2 = 1E-11;
	long double serial_pi = 0;
    long double serial_pi2 = 0;
    long double mutex_pi = 0;
    long double mutex_pi2 = 0;
    long double sema_pi = 0;
    long double sema_pi2 = 0;
	
	printf("There are %d processes\n", nprocs);
	printf("Computing pi serially...\n");
	pi_serial(&serial_pi, error2);
    pi_serial(&serial_pi2, error1);
    assert(serial_pi - serial_pi2 <= error1);
	printf("The final value is: %.30Lf\n", serial_pi);
	printf("Computing pi via a mutex...\n");
	pi_mutex(&mutex_pi, error2, nprocs);
    pi_mutex(&mutex_pi2, error2, nprocs);
    assert(mutex_pi2 - mutex_pi <= error1);
	printf("The final value is: %.30Lf\n", mutex_pi);
	printf("Computing pi via a semaphore...\n");
	pi_semaphore(&sema_pi, error2, nprocs);
    pi_semaphore(&sema_pi2, error2, nprocs);
    assert(sema_pi - sema_pi2 <= error1);
	printf("The final value is: %.30Lf\n", sema_pi);
    
    assert(2+2 == 4);

}