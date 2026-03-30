#include <stdlib.h>
#include <stdio.h>
#include <immintrin.h>

#define N 100000
#define C 'A'


int find_occurence(char* arr, char c) {
	
	__m256i target = _mm256_set1_epi8('A');

	int i;
	int res = 0;

	for (i = 0; i <= N - 32; i += 32) {
		__m256i va = _mm256_loadu_si256((__m256i*)&arr[i]);
		__m256i cmp = _mm256_cmpeq_epi8(va, target);
		int mask = _mm256_movemask_epi8(cmp);
		res += __builtin_popcount(mask);
	
	}

	for (; i < N; i++) {
		if (arr[i] == c) {
			res++;
		}
	}

	return res;
}


int main() {

	char* arr = (char* )malloc(N * sizeof(char));
	if (arr == NULL){
		perror("malloc");
		return 1;
	}

	for (int i = 0; i < N; i++) {
   		arr[i] = 'A' + (rand() % 26);
	}

	int res = find_occurence(arr, C);
	printf("%d\n", res);
	free(arr);

	return 0;
}