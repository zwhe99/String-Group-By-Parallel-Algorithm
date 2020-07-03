//
// Created by hezhiwei on 6/27/20.
//

#ifndef PARALLELPROJECT_SERIAL_FUNCITON_H
#define PARALLELPROJECT_SERIAL_FUNCITON_H

#include <string.h>
#include "stdlib.h"
#include <stdint.h>
#include <stdio.h>
#define SIZE_OF_CNT8 1<<8
#define SIZE_OF_CNT16 1<<16
#define RADIX_MIN_NUM 30

/**
 * @name swapStringPointer
 * @brief swap s1 and s2
 * @param s1
 * @param s2
 */
void swapStringPointer(char** s1_p, char** s2_p);

/**
 * @name swapInt
 * @brief swap two size_t value using their pointer
 * @param pInt
 * @param pInt1
 */
void swapInt(size_t* pInt, size_t* pInt1);

/**
 * @name first_char_count_string_sort
 * @brief use counting sort to sort the array of strings with the first letter as their keys
 * and return the begin position and end position of each group
 * @param str_arr_p :[IN|OUT]pointer to the array of strings
 * @param num :[IN]size of the the array of strings
 * @param begin_pos_p :[OUT]pointer to the array that save the begin position of each group
 * @param end_pos_p :[OUT]pointer to the array that save the end position of each group
 */
void first_char_count_string_sort(char*** str_arr_p , size_t num, size_t** begin_pos_p, size_t** end_pos_p);

/**
 * @name quick_sort
 * @brief sort the array of string by quick-sort. The strings has been sorted according their "level" length prefix,
 * @param arr :[IN|OUT]array of strings to be sort
 * @param num :[IN] # of elements in arr
 * @param level :[IN] The strings has been sorted according their "level" length prefix.
 */
void quick_sort(char** arr, size_t num, int level);


/**
 * @name quick_sort_partial
 * @brief sort specified parts of the array of strings by quick-sort
 * @param arr :[IN|OUT]array of strings to be sort
 * @param starts :[IN]array of begin pos of each part
 * @param ends :[IN]array of end pos of each part
 * @param level :[IN] all the strings has a common prefix of "level" length
 */
void quick_sort_partial(char** arr, size_t* starts, size_t *ends, int level);

/**
 * @name radix_sort_partial
 * @brief sort specified parts of the array of strings by radix-sort
 * @param arr :[IN|OUT]array of strings to be sort
 * @param starts :[IN]array of begin pos of each part
 * @param ends :[IN]array of end pos of each part
 * @param level :[IN] all the strings has a common prefix of "level" length
 */
void radix_sort_partial(char** arr, size_t* starts, size_t *ends, int level);

/**
 * @name radix_sort_main
 * @brief sort the array of strings
 * @param arr :[IN|OUT]array to be sorted
 * @param num :[IN]num of elements in array
 * @param level :[IN]all the strings has a common prefix of "level" length
 */
void radix_sort_main(char** arr, size_t num, int level);

/**
 * @name radix_sort_inner
 * @brief Recursively called by radix_sort8 and radix_sort16.Sort the [beg, end) of arr.
 * Use different method to sort based on size. If size <= RADIX_MIN_NUM then use quick sort,
 * and if RADIX_MIN_NUM < size <= 1<<16 user radix_sort8, otherwise use radix_sort16.
 * @param arr :[IN|OUT]array to be sorted
 * @param src :[IN|OUT]read data from src
 * @param dest :[IN|OUT]the result of "level" step will write in dest
 * @param leb8 :[IN]Start pos of the array of chars in each strings;
 * @param leb16 :[IN]Start pos of the array of sup-chars in each strings;
 * @param beg :[IN]left point of interval that should br sorted(include)
 * @param end :[IN]right point of interval that should br sorted(except)
 * @param level :[IN]all the strings has a common prefix of "level" length
 */
void radix_sort_inner(char** arr, char** src, char** dest, uint8_t* leb8, uint16_t* leb16, size_t beg, size_t end, int level);

/**
 * @name radix_sort8
 * @brief Sort the array of strings that with "level" chars prefix based on radix sort, considering
 * a string is combination of uint_8.
 * @param arr :[IN|OUT]array to be sorted
 * @param src :[IN|OUT]read data from src
 * @param dest :[IN|OUT]the result of "level" step will write in dest
 * @param leb8 :[IN]Start pos of the array of chars in each strings;
 * @param leb16 :[IN]Start pos of the array of sup-chars in each strings;
 * @param beg :[IN]left point of interval that should br sorted(include)
 * @param end :[IN]right point of interval that should br sorted(except)
 * @param level :[IN]all the strings has a common prefix of "level" length
 */
void radix_sort8(char** arr, char** src, char** dest, uint8_t* leb8, uint16_t* leb16, size_t beg, size_t end, int level);

/**
 * @name radix_sort16
 * @brief Sort the array of strings that with "level" chars prefix based on radix sort, considering
 * a string is combination of uint_16.
 * @param arr :[IN|OUT]array to be sorted
 * @param src :[IN|OUT]read data from src
 * @param dest :[IN|OUT]the result of "level" step will write in dest
 * @param leb8 :[IN]Start pos of the array of chars in each strings;
 * @param leb16 :[IN]Start pos of the array of sup-chars in each strings;
 * @param beg :[IN]left point of interval that should br sorted(include)
 * @param end :[IN]right point of interval that should br sorted(except)
 * @param level :[IN]all the strings has a common prefix of "level" length
 */
void radix_sort16(char** arr, char** src, char** dest, uint8_t* leb8, uint16_t* leb16, size_t beg, size_t end, int level);


#endif //PARALLELPROJECT_SERIAL_FUNCITON_H
