//
// Created by hezhiwei on 6/26/20.
//

#ifndef PARALLELPROJECT_TEST_H
#define PARALLELPROJECT_TEST_H

#include "stdlib.h"
#include "string.h"
#include "omp_group_by.h"
#include "stdio.h"
#include "serial_funciton.h"

void test_count_string_sort();
void test_omp_count_string_sort();
void test_omp_count_string_sort2();
void test_quick_string_sort();
void test_quick_sort_partial();

void test_omp_quick_sort_partial();
void test_assign_group();
void test_radix_sort_main();
void test_parallel_read_file(const char* file_path, int num_rows);
#endif //PARALLELPROJECT_TEST_H
