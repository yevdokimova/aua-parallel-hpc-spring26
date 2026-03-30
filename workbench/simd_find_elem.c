#include <stdio.h>
#include <stdlib.h>
#include <immintrin.h>

#define N 100000
#define NUM 7
#define MIN 0
#define MAX 10


int find_elem(float* arr, float num) {

	__m256 sval = _mm256_set1_ps (num);

	int i;
	for (i = 0; i <= N - 8; i +=8) {
		__m256 va = _mm256_loadu_ps(&arr[i]);
		__m256 res = _mm256_cmp_ps (va, sval, _CMP_EQ_OQ);
		int mask = _mm256_movemask_ps(res);
		if (mask != 0)
           		return i + __builtin_ctz(mask);
	}

	for (; i < N; i++) {
		if (arr[i] == num) {
			return i;			
		}
	}

	return -1;
}


int main() {

	float* arr = (float*) malloc(N * sizeof(float));
	if (arr == NULL) {
		perror("malloc");
		return 1;
	}

	for (int i = 0; i < N; i++) {
		arr[i] = rand() % (MAX - MIN + 1);
	}

	int res = find_elem(arr, NUM);
	printf("%d\n", res);

	return 0;
}