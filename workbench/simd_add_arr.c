#include <immintrin.h>
#include <stdlib.h>
#include <stdio.h>


#define N 10000
#define MIN 0
#define MAX 100


void simd_add(float* a, float* b, float* res) {
	
	int i;
	for(i = 0; i <= N - 8; i += 8) {
		__m256 va = _mm256_loadu_ps (&a[i]);
		__m256 vb = _mm256_loadu_ps (&b[i]);
		__m256 vres = _mm256_add_ps(va, vb);
		_mm256_storeu_ps(&res[i], vres); 
	}

	for (; i < N; i++) {
		res[i] = a[i] + b[i];
	}

}


int main() {

	float* a = (float *)malloc(N * sizeof(float));
	float* b = (float *)malloc(N * sizeof(float));
	float* rest = (float *)calloc(N, sizeof(float));
	
	if (a == NULL || b == NULL || rest == NULL) {
		perror("malloc");
		return 1;
	}

	for (int i = 0; i < N; i++) {
		a[i] = rand() % (MAX - MIN + 1);
		b[i] = rand() % (MAX - MIN + 1);
	}

	simd_add(a, b, rest);
	
	free(a);
	free(b);
	free(rest);

	return 0;
}
