#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include<stdio.h>
#include "solution_openmp.h"



void kernel(float* a, float* b, float* c, int rows, int cols, int jobs) {
	//int tid  = threadIdx.x + blockIdx.x * blockDim.x;
	int tid = omp_get_thread_num();

	int i=0;
	float temp = 0;
	for(i =0; i<cols; i++){
		temp += a[i]*b[i*rows+tid];
	}
	c[tid] = temp;
	
}
