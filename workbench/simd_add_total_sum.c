#include <immintrin.h>
#include <stdlib.h>
#include <stdio.h>


#define N 10000
#define MIN 0
#define MAX 100


float hsum(__m256 v) {
    __m128 lo = _mm256_castps256_ps128(v);
    __m128 hi = _mm256_extractf128_ps(v, 1);
    __m128 sum128 = _mm_add_ps(lo, hi);
    sum128 = _mm_hadd_ps(sum128, sum128);
    sum128 = _mm_hadd_ps(sum128, sum128);
    return _mm_cvtss_f32(sum128);
}

void simd_add(float* a, float* b, float* res) {
	
	int i;
	__m256 s = _mm256_setzero_ps();

	for(i = 0; i <= N - 8; i += 8) {
		__m256 va = _mm256_loadu_ps (&a[i]);
		__m256 vb = _mm256_loadu_ps (&b[i]);
		__m256 temp = _mm256_add_ps(va, vb);
		s = _mm256_add_ps(temp, s); 
	}

	*res = hsum(s);

	for (; i < N; i++) {
		float temp = a[i] + b[i];
		*res += temp;
	}


}


int main() {

	float* a = (float *)malloc(N * sizeof(float));
	float* b = (float *)malloc(N * sizeof(float));
	float res = 0;
	
	if (a == NULL || b == NULL) {
		perror("malloc");
		return 1;
	}

	for (int i = 0; i < N; i++) {
		a[i] = rand() % (MAX - MIN + 1);
		b[i] = rand() % (MAX - MIN + 1);
	}

	simd_add(a, b, &res);
	
	free(a);
	free(b);

	return 0;
}
