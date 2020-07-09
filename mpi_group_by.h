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
#include <limits.h>
#include "hashmap.h"
#define SIZE_OF_CNT 64
#define MAX_STRING_LEN 15

void mpi_group_by(const char *filepath, size_t num_rows, int ifOpenmp);

void mpi_read_file(const char* filepath, MPI_File* fp, char** buffer_p, MPI_Offset* filesize_p);

void mpi_get_let_range(int comm_sz, int my_rank, uint16_t l, uint16_t r, uint16_t* ml, uint16_t* mr, uint16_t* count);

void mpi_distribute_task(MPI_Offset size, const char* buffer, char** str_arr, uint16_t my_l_let, uint16_t my_r_let, size_t* my_rows);


void Check_for_error(int local_ok, char message[]);

#endif //PARALLELPROJECT_MPI_GROUP_BY_H
