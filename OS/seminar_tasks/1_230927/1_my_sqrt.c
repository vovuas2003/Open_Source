#include <stdio.h>
#include <math.h>
int main() {
	double eps = 1e-6;
	double a, prev, res;
	if(scanf("%lg", &a) != 1) {
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
