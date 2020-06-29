//
// Created by hezhiwei on 6/28/20.
//
#include "omp.h"
#include <stdio.h>
#include "measure.h"
#include "stdlib.h"
#include "omp_group_by.h"

void mes_omp_group_by(){
    for(int n_thread = 1; n_thread<=16;n_thread++){
        omp_set_num_threads(n_thread);
        double total = 0;
        for(int k =0;k<100;++k){
            double start, end;
            start = omp_get_wtime();
            omp_group_by("/home/hezhiwei/CLionProjects/ParallelProject/1M.txt", 2000);
            end = omp_get_wtime();
            total += end-start;
        }
        printf("%d\t%f\n", n_thread, total);
    }
}