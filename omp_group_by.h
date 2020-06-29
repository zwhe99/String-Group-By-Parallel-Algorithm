//
// Created by hezhiwei on 6/26/20.
//
#ifndef PARALLELPROJECT_OMP_GROUP_BY_H
#define PARALLELPROJECT_OMP_GROUP_BY_H

#include <string.h>
#include "serial_funciton.h"
#include <stdio.h>
#include <omp.h>
#include "stdlib.h"
#include "stdint.h"

//todo:decrease malloc
//todo:memory size not enough
//todo:dynammic allocate
//todo:delete \n\r

/**
 * @name omp_first_char_count_string_sort
 * @brief use counting sort to sort the array of strings with the first letter as their keys
 * and return the begin position and end position of each group(parallelize by openmp)
 * @param str_arr_p :[IN|OUT]pointer to the array of strings
 * @param num :[IN]size of the the array of strings
 * @param begin_pos_p :[OUT]pointer to the array that save the begin position of each group
 * @param end_pos_p :[OUT]pointer to the array that save the end position of each group
 */
void omp_first_char_count_string_sort(char*** str_arr_p, size_t num, size_t** begin_pos_p, size_t** end_pos_p);


/**
 * @name omp_prefix_sum
 * @brief calculate the prefix sum of arr and save in arr(parallelize by openmp)
 * @param arr :[IN|OUT]the array that use to be calculated prefix sum
 * @param num :[IN] num of elements in arr
 */
void omp_prefix_sum(size_t arr [], int num);


/**
 * @name omp_quick_sort_partial
 * @brief sort specified parts of the array of strings by quick-sort(parallelize by openmp)
 * @param arr :[IN|OUT]array of strings to be sort
 * @param starts :[IN]array of begin pos of each part
 * @param ends :[IN]array of end pos of each part
 * @param level :[IN] all the strings has a common prefix of "level" length
 */
void omp_quick_sort_partial(char** arr, size_t *starts, size_t *ends, int level);

/**
 * @name omp_radix_sort_partial
 * @brief sort specified parts of the array of strings by radix-sort(parallelize by openmp)
 * @param arr :[IN|OUT]array of strings to be sort
 * @param starts :[IN]array of begin pos of each part
 * @param ends :[IN]array of end pos of each part
 * @param level :[IN] all the strings has a common prefix of "level" length
 */
void omp_radix_sort_partial(char** arr, size_t *starts, size_t *ends, int level);


/**
 * @name omp_assign_group
 * @brief assign group id for each elements of sorted arr
 * and return keys and lengths of each group (parallelize by openmp)
 * @param arr :[IN]array that to be assigned group
 * @param group_keys :[OUT]keys of each group
 * @param group_lens :[OUT]size of each group
 * @param num_groups_p :[OUT] pointer to # of groups
 * @param num_strings :[IN] size of arr
 * @param prefix_starts :[IN] start positions of each group that distinguished by the first element the each string
 * @param prefix_ends  :[IN] end positions of each group that distinguished by the first element the each string
 */
void omp_assign_group(char** arr, char** group_keys, size_t* group_lens, size_t* num_groups_p, size_t num_strings, size_t* prefix_starts, size_t* prefix_ends);


void omp_group_by(const char* file_path, size_t num_rows);
#endif //PARALLELPROJECT_OMP_GROUP_BY_H
