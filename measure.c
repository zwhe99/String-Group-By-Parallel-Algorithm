//
// Created by hezhiwei on 6/28/20.
//
#include "omp.h"
#include <stdio.h>
#include "measure.h"
#include "stdlib.h"
#include "omp_group_by.h"

void mes_omp_group_by(const char *src_file_path, size_t num_rows, const char *dest_file_path) {
    int n_thread, k=0, j=0;
    FILE * fp_src, *fp_dest;
    size_t n, num_groups;
    char ** str_arr = calloc(num_rows, sizeof(char *));
    char ** group_keys = calloc(num_rows, sizeof(char *));
    size_t * group_lens = calloc(num_rows, sizeof(size_t));
    for(n_thread = 1; n_thread<=16;n_thread++){
        omp_set_num_threads(n_thread);

        double total = 0;
        for(k =0; k < 10; ++k){
            double start, end;

            fp_src = fopen(src_file_path, "r");
            if (fp_src == NULL){
                fprintf(stderr, "omp_group_by: can't open %s", src_file_path);
                exit(EXIT_FAILURE);
            }

            while ((getline(&str_arr[j], &n, fp_src)) != -1)
            {
                j++;
            }
            j = 0;
            fclose(fp_src);

            start = omp_get_wtime();
            omp_group_by(str_arr, num_rows, group_keys, group_lens, &num_groups);
            end = omp_get_wtime();
            total += end-start;
        }
        printf("%d\t%f\n", n_thread, total);
    }

    //write tp file
    fp_dest = fopen(dest_file_path, "w");
    if (fp_dest == NULL){
        fprintf(stderr, "mes_omp_group_by: can't open %s", dest_file_path);
        exit(EXIT_FAILURE);
    }
    size_t i;
    for(i =0; i < num_groups; ++i){
        fprintf(fp_dest,"%zd  %s", group_lens[i],group_keys[i]);
    }
    fprintf(fp_dest,"# of groups: %zu \n", num_groups);
    fclose(fp_dest);

    for (k = 0; k < num_rows; ++k) {
        free(str_arr[k]);
    }
    free(str_arr);
    free(group_keys);
    free(group_lens);
}