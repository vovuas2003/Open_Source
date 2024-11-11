#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int main(int argc, char *argv[], char *env[]) {
	if(argc != 2) {
		printf("I need only one additional command line argument!\n");
		return 0;
	}
	char *check = NULL;
	double a = strtod(argv[1], &check); 
	if((check == argv[1]) || (*check != '\0')) {
		printf("Incorrect input!\n");
		return 0;
	}
	if(a < 0) {
		printf("Negative value!\n");
		return 0;
	}
	printf("Sqrt(%lg) = ", a);
	if(a == 0) {
		printf("0\n");
		return 0;
	}
	double prev = 1;
	double res = (prev + (a / prev)) / 2;
	double eps = 1e-6;
	while(fabs(res - prev) >= eps) {
		prev = res;
		res = (prev + (a / prev)) / 2;
	}
	printf("%lg\n", res);
	return 0;
}
