//
// Created by hezhiwei on 7/1/20.
//

#ifndef PARALLELPROJECT_MPI_GROUP_BY_H
#define PARALLELPROJECT_MPI_GROUP_BY_H
#include "mpi.h"
#include <string.h>
#include "mpi.h"
#include "stdio.h"
#include "stdlib.h"

#define SIZE_OF_CNT8 1<<8
#define MAX_STRING_LEN 25
#define OVERLAP 25

void mpi_first_char_count_string_sort(char*** str_arr_p, size_t num, size_t** begin_pos_p, size_t** end_pos_p);

void mpi_group_by(const char* filepath, size_t num_rows);

void mpi_read_file(MPI_File *in, int my_rank, int comm_sz, char** l_str_arr);

void Check_for_error(int local_ok, char message[]);

#endif //PARALLELPROJECT_MPI_GROUP_BY_H
