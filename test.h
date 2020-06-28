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
void test_quick_string_sort();
void test_quick_sort_partial();
void test_omp_prefix_sum();

void test_omp_quick_sort_partial();
void test_assign_group();
void test_radix_sort_main();

#endif //PARALLELPROJECT_TEST_H
