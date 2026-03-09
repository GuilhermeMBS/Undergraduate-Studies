#include <stdio.h>
#include <time.h>

int fibo(int n) {

	int a, b, c;
	a = 0; b = 1;

	if (n == 1) return 0;
	else if (n == 2) return 1;

	for (int i = 2; i < n; i++)
	{
		c = a + b;
		a = b; b = c;
	}

	return c;

}


int fiborec(int n) {

	if (n == 2) return 1;
	else if (n == 1) return 0;

	return fiborec(n-1) + fiborec(n-2);

}


int main(void) {
	
	int n = 40;
	int fi, fr;

	printf("\t\tFIBONACCI DE %d\n", n);

	clock_t fi_start = clock();
	fi = fibo(n);
	clock_t fi_end = clock();
	double fi_time = ((double)(fi_end - fi_start)) / CLOCKS_PER_SEC;

	printf("Fibonacci Iterativo: %d\tTempo: %f\n", fi, fi_time);

	clock_t fr_start = clock();
	fr = fiborec(n);
	clock_t fr_end = clock();
	double fr_time = ((double)(fr_end - fr_start)) / CLOCKS_PER_SEC;

	printf("Fibonacci Recursivo: %d\tTempo: %f\n", fr, fr_time);

	return 0;

}