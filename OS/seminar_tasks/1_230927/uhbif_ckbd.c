#include <stdio.h>
#include <math.h>
#include <stdlib.h>
int main(int argc, char *argv[], char *env[]) {
	double eps = 1e-6;
	double a, prev, res;
	char* buf = getenv("MSQRT");
	if(buf == NULL) {
		printf("No environmental variable MSQRT!\n");
		return 0;
	}
	char* check = NULL;
	a = strtod(buf, &check); 
	if((check == buf) || (*check != '\0')) {
		printf("Incorrect input!\n");
		return 0;
	}
	if(a < 0) {
		printf("Negative value!\n");
		return 0;
	}
	if(a == 0) {
		printf("0\n");
		return 0;
	}
	prev = 1;
	res = (prev + (a / prev)) / 2;
	while(fabs(res - prev) >= eps) {
		prev = res;
		res = (prev + (a / prev)) / 2;
	}
	printf("%lg\n", res);
	return 0;
}
