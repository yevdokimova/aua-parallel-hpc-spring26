#include <stdio.h>
#include <stdlib.h>
#include <immintrin.h>

// gcc -mavx2 simd_calc_uppercase.c -o simd_calc_uppercase

#define ARR_SIZE 1000000

int count_uppercase(char *arr) {

    int res = 0;
    int i;

    __m256i lo = _mm256_set1_epi8('A' - 1);
    __m256i hi = _mm256_set1_epi8('Z' + 1);

    for (i = 0; i + 32 <= ARR_SIZE; i += 32) {
        __m256i va = _mm256_loadu_si256((__m256i*)&arr[i]);
        __m256i cmp1 = _mm256_cmpgt_epi8(va, lo);    // val > '@'  → val >= 'A'
        __m256i cmp2 = _mm256_cmpgt_epi8(hi, va);    // '[' > val  → val <= 'Z'
        __m256i result = _mm256_and_si256(cmp1, cmp2);
        int mask = _mm256_movemask_epi8(result);
        res += __builtin_popcount(mask);
    }

    for (; i < ARR_SIZE; i++) {
        if (arr[i] >= 'A' && arr[i] <= 'Z') {
            res++;
        }
    }

    return res;
}


int main() {

	char* arr = (char *) malloc(ARR_SIZE * sizeof(char));
	if (arr == NULL) {
		return 1;
	}

	for (int i = 0; i < ARR_SIZE; i++) {
		arr[i] = '!' + (rand() % 89);
	}

    int result_v = 0;

    for (int i = 0; i < ARR_SIZE; i++) {
        if (arr[i] >= 'A' && arr[i] <= 'Z') {
            result_v++;
        }
    }

	int result = count_uppercase(arr);
    if(result != result_v) {
        printf("Invalid results from SIMD");
        return 1;
    }
    else {
        printf("Number of uppercase occurence is %d\n", result);
    }
	
	free(arr);
	return 0;
}