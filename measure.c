//
// Created by hezhiwei on 6/28/20.
//
#include "omp.h"
#include <stdio.h>
#include "measure.h"
#include "stdlib.h"
#include "omp_group_by.h"

void mes_omp_group_by(const char * file_path, size_t num_rows){
    int n_thread, k=0, j=0;
    FILE * fp;
    size_t n;
    char ** str_arr = calloc(num_rows, sizeof(char *));

    for(n_thread = 1; n_thread<=16;n_thread++){
        omp_set_num_threads(n_thread);

        double total = 0;
        for(k =0; k < 10; ++k){
            double start, end;

            fp = fopen(file_path, "r");
            if (fp == NULL){
                fprintf(stderr, "omp_group_by: can't open %s", file_path);
                exit(EXIT_FAILURE);
            }
            while ((getline(&str_arr[j], &n, fp)) != -1)
            {
                j++;
            }
            j = 0;
            fclose(fp);

            start = omp_get_wtime();
            omp_group_by(str_arr, num_rows);
            end = omp_get_wtime();
            total += end-start;
        }
        printf("%d\t%f\n", n_thread, total);
    }

    for (k = 0; k < num_rows; ++k) {
        free(str_arr[k]);
    }
    free(str_arr[k]);
}