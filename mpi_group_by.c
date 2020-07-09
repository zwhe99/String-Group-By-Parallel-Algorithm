//
// Created by hezhiwei on 7/1/20.
//

#include "mpi_group_by.h"
#include "serial_funciton.h"
#include "omp_group_by.h"

void mpi_group_by(const char *filepath, size_t num_rows, int ifOpenmp) {
    int comm_sz, my_rank, ierr, i, local_ok = 1;
    const uint16_t l_let = ('A'<<8) | 'A';
    const uint16_t r_let = ('H'<<8) | 'H';
    uint16_t my_l_let, my_r_let, my_let_count;
    size_t it, max_my_row, my_row, my_num_groups = 0, global_num_groups=0;
    size_t* count_p = NULL;
    size_t* num_groups_arr = NULL;
    size_t* l_group_len = NULL;
    size_t* g_group_len = NULL;
    size_t* begin_pos = NULL;
    size_t* end_pos = NULL;
    const char* key =NULL;
    char* buffer = NULL;
    char ** l_str_arr = NULL;
    char ** l_group_key_discrete = NULL;
    char ** l_group_key_contiguous = NULL;
    char ** g_str_arr = NULL;
    char ** g_group_key = NULL;
    MPI_File file;
    MPI_Offset file_size;
    MPI_Status status;

    //INIT MPI
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);


    //init num_groups_arr
    if(my_rank == 0){
        num_groups_arr = calloc(comm_sz, sizeof(size_t));
        if(!num_groups_arr){
            local_ok = 0;
        }
    }
    Check_for_error(local_ok, "cant allocate num_groups_arr");

    //calculate max # of lines that will be distributed to current process
    max_my_row = (size_t)((num_rows / comm_sz) * 1.5);

    //get the range of char distributed to current process
    mpi_get_let_range(comm_sz, my_rank, l_let, r_let, &my_l_let, &my_r_let, &my_let_count);

    //allocate memory for l_str_arr
    l_str_arr = calloc(max_my_row , sizeof(char *));
    if(!l_str_arr)
        local_ok = 0;
    Check_for_error(local_ok, "failed to calloc l_str_arr!");

    for(it = 0; it < max_my_row; ++it){
        l_str_arr[it] = calloc(MAX_STRING_LEN, sizeof(char ));
        if(!l_str_arr[it])
            local_ok = 0;
    }
    Check_for_error(local_ok, "failed to calloc l_str_arr!");

    //open file
    ierr = MPI_File_open(MPI_COMM_WORLD, filepath, MPI_MODE_RDONLY, MPI_INFO_NULL, &file);
    if (ierr != MPI_SUCCESS) {
        local_ok = 0;
    }
    Check_for_error(local_ok, "cant open file!");

    //read file to buffer
    mpi_read_file(filepath, &file, &buffer, &file_size);
    MPI_File_close(&file);

    //distribute task
    mpi_distribute_task(file_size, buffer, l_str_arr, my_l_let, my_r_let, &my_row);
    free(buffer);

    char** l_str_arr_tmp = l_str_arr;
    if(ifOpenmp){
        //sort the l_str_arr according to the first super char
        omp_first_char_count_string_sort(&l_str_arr, my_row, &begin_pos, &end_pos);

        //sort the l_str_arr
        omp_radix_sort_partial(l_str_arr, begin_pos, end_pos, 1);

        //assign group
        l_group_key_discrete = calloc(my_row, sizeof(char *));
        l_group_len = calloc(my_row, sizeof(size_t));
        omp_assign_group(l_str_arr, l_group_key_discrete, l_group_len, &my_num_groups, my_row, begin_pos, end_pos);

        //copy the string to a contiguous memory
        l_group_key_contiguous = alloc_2d_char(my_num_groups, MAX_STRING_LEN);
        for(it = 0;it<my_num_groups;++it){
            strcpy(l_group_key_contiguous[it], l_group_key_discrete[it]);
        }
        free(l_group_key_discrete);

    }
    else{
        l_group_key_contiguous = alloc_2d_char(my_row, MAX_STRING_LEN);
        l_group_len = calloc(my_row, sizeof(size_t));
        radix_sort_main(l_str_arr, my_row, 0);

        char* cur_key = l_str_arr[0];
        size_t cur_group_num = 0;
        for(it = 0; it < my_row; ++it){
            if(strcmp(cur_key, l_str_arr[it]) == 0){
                //same group
                ++l_group_len[cur_group_num];
            }
            else
            {
                //other group
                ++cur_group_num;
                l_group_len[cur_group_num] = 1;
                cur_key = l_str_arr[it];
                strcpy(l_group_key_contiguous[cur_group_num], cur_key);
            }
        }
        my_num_groups = cur_group_num +1;
    }
    for (it = 0; it < max_my_row; ++it) {
        free(l_str_arr_tmp[it]);
    }
    free(l_str_arr_tmp);


    //get num_groups from worker processes
    MPI_Gather(&my_num_groups, 1, MPI_UNSIGNED_LONG, num_groups_arr, 1, MPI_UNSIGNED_LONG, 0, MPI_COMM_WORLD);

    //calculate total # of groups
    if(my_rank == 0){
        for(i = 0; i < comm_sz; ++i){
            global_num_groups += num_groups_arr[i];
        }
    }

    //allocate memory for global key and len
    if(my_rank == 0){
        g_group_key = alloc_2d_char(global_num_groups, MAX_STRING_LEN);
        g_group_len = calloc(global_num_groups, sizeof(size_t));
    }
    MPI_Barrier(MPI_COMM_WORLD);

    //gather all keys and lens from worker processes
    if(my_rank == 0){
        int * recvcounts_key = calloc(comm_sz, sizeof(int));
        int * recvcounts_len = calloc(comm_sz, sizeof(int));
        int * displs_key = malloc(comm_sz * sizeof(int));
        int * displs_len = malloc(comm_sz * sizeof(int));
        for(i=0;i<comm_sz;++i){
            recvcounts_key[i] = MAX_STRING_LEN * num_groups_arr[i];
            recvcounts_len[i] = num_groups_arr[i];
        }

        memcpy(displs_key, recvcounts_key, comm_sz * sizeof(int ));
        memcpy(displs_len, recvcounts_len, comm_sz * sizeof(int ));

        size_t prefix_sum = 0;
        for(i=0; i<comm_sz; ++i){
            size_t tmp = displs_key[i];
            displs_key[i] = prefix_sum;
            prefix_sum += tmp;
        }
        prefix_sum = 0;
        for(i=0; i<comm_sz; ++i){
            size_t tmp = displs_len[i];
            displs_len[i] = prefix_sum;
            prefix_sum += tmp;
        }
        MPI_Gatherv(&(l_group_key_contiguous[0][0]), MAX_STRING_LEN*my_num_groups, MPI_CHAR, &(g_group_key[0][0]), recvcounts_key, displs_key, MPI_CHAR, 0, MPI_COMM_WORLD);
        MPI_Gatherv(l_group_len, my_num_groups, MPI_UNSIGNED_LONG, g_group_len, recvcounts_len, displs_len, MPI_UNSIGNED_LONG, 0, MPI_COMM_WORLD);
        free(recvcounts_key);
        free(displs_key);
        free(recvcounts_len);
        free(displs_len);
    }
    else{
        MPI_Gatherv(&(l_group_key_contiguous[0][0]), MAX_STRING_LEN*my_num_groups, MPI_CHAR, NULL, NULL, NULL, MPI_CHAR, 0, MPI_COMM_WORLD);
        MPI_Gatherv(l_group_len, my_num_groups, MPI_UNSIGNED_LONG, NULL, NULL, NULL, MPI_UNSIGNED_LONG, 0, MPI_COMM_WORLD);

    }
    free(l_group_key_contiguous[0]);
    free(l_group_key_contiguous);
    free(l_group_len);


    //print
    if(my_rank == 0){
        for(it = 0; it <global_num_groups;++it){
            printf("%zu\t%s\n", g_group_len[it], g_group_key[it]);
        }
        printf("# of groups:%zu", global_num_groups);
    }



    //free
    if(my_rank == 0){
        free(num_groups_arr);
        free(g_group_len);
        free(g_group_key[0]);
        free(g_group_key);
    }

    // Finalize the MPI environment.
    MPI_Finalize();
}
void mpi_read_file(const char* filepath, MPI_File* fp, char** buffer_p, MPI_Offset* filesize_p){
    int local_ok = 1;
    MPI_Offset file_size;
    int ierr = MPI_File_open(MPI_COMM_WORLD, filepath, MPI_MODE_RDONLY, MPI_INFO_NULL, fp);

    if (ierr != MPI_SUCCESS) {
        local_ok = 0;
    }
    Check_for_error(local_ok, "cant open file!");

    MPI_File_get_size(*fp, &file_size);
    *filesize_p = file_size;

    *buffer_p = malloc(file_size * sizeof(char));
    if (file_size <= INT_MAX) {
        MPI_File_read_at_all(*fp, 0, *buffer_p, file_size, MPI_CHAR, MPI_STATUS_IGNORE);
    }
    else
    {
        MPI_Offset offset = 0;
        int part_size = 0;
        size_t remain_size = file_size;

        while (remain_size != 0)
        {
            part_size = INT_MAX < remain_size? INT_MAX: remain_size;
            MPI_File_read_at_all(*fp, offset, (*buffer_p)+offset, part_size, MPI_CHAR, MPI_STATUS_IGNORE);
            remain_size -= part_size;
            offset += part_size;
        }
    }
}
void mpi_get_let_range(int comm_sz, int my_rank, uint16_t l, uint16_t r, uint16_t* ml, uint16_t* mr, uint16_t* count_p){
    uint16_t remain = (r - l + 1) % comm_sz;
    *count_p = (r - l + 1) / comm_sz;
    if (my_rank < remain) {
        ++(*count_p);
        *ml = my_rank * (*count_p) + l;
    }
    else
    {
        *ml = my_rank * (*count_p) + remain + l;
    }
    *mr = (*ml) + (*count_p) - 1;
}
void mpi_distribute_task(MPI_Offset size, const char* buffer, char** str_arr, uint16_t my_l_let, uint16_t my_r_let, size_t* my_rows_p){
    int new_line_flag = 1;
    size_t i = 0, cur_row = 0, cur_col = 0;
    uint16_t cat;
    for (i = 0; i < size; ++i) {
        char c = buffer[i];
        if (new_line_flag) {
            cat = (c << 8) | buffer[i + 1];
            if (cat < my_l_let || cat > my_r_let) {
                while (buffer[i] != '\n') {
                    ++i;
                }
                continue;
            }
            else
            {
                new_line_flag = 0;
                str_arr[cur_row][cur_col++] = c;
            }
        }
        else
        {
            str_arr[cur_row][cur_col++] = c;
            if (c == '\n' || c == '\r') {
                str_arr[cur_row][cur_col-1] = '\0';
                ++cur_row;
                cur_col = 0;
                new_line_flag = 1;
            }
        }
    }
    *my_rows_p = cur_row;
}
void Check_for_error(int local_ok, char message[]) {
    int ok;
    MPI_Allreduce(&local_ok, &ok, 1, MPI_INT, MPI_MIN, MPI_COMM_WORLD);
    if (ok == 0) {
        int my_rank;
        MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
        if (my_rank == 0) {
            fprintf(stderr, "Proc %d > In %s, %s\n", my_rank, __FUNCTION__ , message);
            fflush(stderr);
        }
        MPI_Finalize();
        exit(-1);
    }
}