//Lab7 Ben Morley

//#include<pthreads.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

//double gPi = 0;

long double calc_x(long double n){
	long double x = (1.0 /( (4.0*(n*n)) - 1.0));
	return(x);
}


double pi_serial(long double* y, long double e){
	long double pi, actual_error;
	long double n = 0.0;
	long double x;
	long double denominator = 0;
	long double numerator = 1;

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
		printf("N  =  %.30LF\n", n);
		printf("x  =  %.30Lf\n", x);
		printf("pi =  %.30Lf\n", pi);
		printf("y  =  %.30Lf\n", *y);
		printf("e  =  %.30Lf\n", e);
		printf("er =  %.30Lf\n\n", actual_error);

		*y = pi;


	}while(actual_error > e);

	return(*y);
}

int main(int argc, char* argv[]){
	long double error = 1E-14;
	long double pi = 0;
	pi_serial(&pi, error);

	printf("The final value is: %Lf\n", pi);



}


