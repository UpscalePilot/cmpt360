//Lab7 Ben Morley

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<pthread.h>
#include<sys/sysinfo.h>
#include<unistd.h>
#include<semaphore.h>


#ifndef CALCS_PER_PROC
#define CALCS_PER_PROC 1000.0L
#endif



long double gpi = 0;
long double gerr = 100;
long double gnumer = 1.0;
long double gdenom = 0.0;
long double endingN = 0;

typedef struct segment{
	long double ns;
	long double ne;
	long double e;
}segment;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t semaphore;

long double calc_x(long double);
void* mutex_calc(void* arg);
void pi_mutex(long double*, long double, int);
void* sem_calc(void* arg);
void pi_semaphore(long double*, long double, int);
void pi_serial(long double*, long double);


// int main(int argc, char* argv[]){
// 	int nprocs = get_nprocs();
// 	long double error = 1E-14;
// 	long double pi = 0;
	
// 	printf("There are %d processes\n", nprocs);
// 	printf("Computing pi serially...\n");
// 	pi_serial(&pi, error);
// 	printf("The final value is: %.30Lf\n", pi);
// 	printf("Computing pi via a mutex...\n");
// 	pi_mutex(&pi, error, nprocs);
// 	printf("The final value is: %.30Lf\n", pi);
// 	printf("Computing pi via a semaphore...\n");
// 	pi_semaphore(&pi, error, nprocs);
// 	printf("The final value is: %.30Lf\n", pi);


// }

long double calc_x(long double n){
	long double x = (1.0L /( (4.0L*(n*n)) - 1.0L));
	return(x);
}

void calc_pi(){
	long double new_pi = gnumer/gdenom;
	gerr = gpi - new_pi;
	if(gerr < 0){
		gerr = gerr * (0-1);
	}	
	gpi = new_pi;
}


void* mutex_calc(void* arg){
	segment* d = (segment*) arg;

	long double x;
	//long double pi;

	pthread_mutex_lock(&mutex);

	for(long double i = d->ns; i <= d->ne && gerr > d->e; i++){
		x = calc_x(i);
		gnumer *= (1 + x);
		gdenom += x;

	}

	pthread_mutex_unlock(&mutex);

	return(NULL);
}

void pi_mutex(long double* y, long double e, int nprc){
	pthread_t tid[nprc];
	segment dat[nprc];

	gpi = 0.0L;
	gdenom = 0.0L;
	gnumer = 1.0L;
	gerr = 100.0L;

	long double start = 1.0L;
	long double end = CALCS_PER_PROC;

	do{

		for(int i = 0; i < nprc; i++){
			dat[i].ns = start;
			dat[i].ne = end;
			dat[i].e = e;
			
			pthread_create(&tid[i], NULL, mutex_calc, (void*) &dat[i]);

			start += CALCS_PER_PROC;
			end += CALCS_PER_PROC;
		}

		for(int i = 0; i < nprc; i++){
			pthread_join(tid[i], NULL);
		}

		calc_pi();

	}while(gerr > e);

	*y = gpi;

}



void* sem_calc(void* arg){
	segment* d = (segment*) arg;

	long double x;

	sem_wait(&semaphore);

	for(long double i = d->ns; i < d->ne && gerr > d->e; i++){
		x = calc_x(i);
		gnumer *= (1 + x);
		gdenom += x;
		
	}

	sem_post(&semaphore);

	return(NULL);
}

void pi_semaphore(long double* y, long double e, int nprc){
	pthread_t tid[nprc];
	segment dat[nprc];
	sem_init(&semaphore,0,1);

	gpi = 0.0L;
	gdenom = 0.0L;
	gnumer = 1.0L;
	gerr = 100.0L;

	long double start = 1.0L;
	long double end = CALCS_PER_PROC;

	do{

		for(int i = 0; i < nprc; i++){
			dat[i].ns = start;
			dat[i].ne = end;
			dat[i].e = e;
			
			pthread_create(&tid[i], NULL, mutex_calc, (void*) &dat[i]);

			start += CALCS_PER_PROC;
			end += CALCS_PER_PROC;
		}

		for(int i = 0; i < nprc; i++){
			pthread_join(tid[i], NULL);
		}

		calc_pi();

	}while(gerr > e);

	*y = gpi;
}

void pi_serial(long double* y, long double e){
	long double pi, actual_error;
	long double n = 0.0L;
	long double x;
	long double denominator = 0.0L;
	long double numerator = 1.0L;

	do{
		n += 1;
		x = calc_x(n);
		numerator *= (1 + x);
		denominator += x;
		pi = numerator / denominator;
		
		actual_error = *y - pi;
		if(actual_error < 0){
			actual_error *= (-1);
		}

		*y = pi;


	}while(actual_error > e);
}

