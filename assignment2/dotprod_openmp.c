#include<stdio.h>
#include "solution_openmp.h"

#define imin(a,b) (a<b?a:b)

const int numberofThreads = 3;
const int threadsPerBlock = 3;

int main(int argc, char *argv[]) {

	//dynamic variables
	int rows, cols, nprocs, CUDA_DEVICE;
	rows = atoi(argv[1]);
	cols = atoi(argv[2]);
	nprocs = atoi(argv[5]);

	// printf("%d %d", rows, cols);
	FILE *fp, *fv;
	size_t size;
	size_t vec_Size;
	size = (size_t)((size_t) rows * (size_t) cols);
	vec_Size = (size_t)((size_t) 1 * (size_t) cols);
	float c;
	float *partial_c = (float*) malloc(rows * sizeof(float));
	float *w_vect = (float*) malloc((vec_Size) * sizeof(float));
	float *b = (float*) malloc((size) * sizeof(float));
	float *dev_a, *dev_b, *dev_partial_c;
	char *line = NULL;
	size_t len = 0;
	char *token, *saveptr;
	float file_data;
	float mat[rows][cols];

	fp = fopen(argv[3], "r");
	if (fp == NULL) {
		printf("Cannot Open the File");
		return 0;
	}

	int i = 0;
	int j = 0;

	for (i = 0; i < rows; i++) {
		for (j = 0; j < cols; j++) {
			fscanf(fp, "%f", &file_data);
			mat[i][j] = file_data;
			//    printf("date=%f",mat[i][j]);
		}
		//   printf("\n");

	}
	fclose(fp);

	for (i = 0; i < cols; i++) {
		for (j = 0; j < rows; j++) {
			b[rows * i + j] = mat[j][i];

		}
		//   printf("%f ", b[5]);
	}

	fv = fopen(argv[4], "r");

	for (j = 0; j < cols; j++) {
		fscanf(fv, "%f", &w_vect[j]);
		//  printf("v=%f",w_vect[j]);
	}

	fclose(fv);

//	int jobs = cols;
//	int BLOCKS = (jobs + threads - 1)/threads;
	int jobs = (unsigned int) ((cols + nprocs - 1) / nprocs);

	//kernel<<<BLOCKS, threads>>>(dev_a, dev_b, dev_partial_c, rows, cols);
#pragma omp parallel num_threads(nprocs)
	kernel(w_vect, b, partial_c, rows, cols, jobs);

	for (i = 0; i < rows; i++) {
		printf("%f\n", partial_c[i]);
	}

	free(partial_c);
}
