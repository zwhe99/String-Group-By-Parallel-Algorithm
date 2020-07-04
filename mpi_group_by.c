//
// Created by hezhiwei on 7/1/20.
//

#include "mpi_group_by.h"
#include "serial_funciton.h"

void mpi_group_by(const char* filepath, size_t num_rows){
    int comm_sz, my_rank, ierr, local_ok = 1;
    const uint16_t l_let = ('A'<<8) | 'A';
    const uint16_t r_let = ('H'<<8) | 'H';
    uint16_t my_l_let, my_r_let, my_let_count;
    size_t it, max_my_row, my_row, my_num_groups;
    size_t* count_p = NULL;
    const char* key =NULL;
    char* buffer = NULL;
    char ** l_str_arr = NULL;
    MPI_File file;
    MPI_Offset file_size;
    HASHMAP(char, size_t) map;

    //INIT MPI
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    //calculate max # of lines that respondding to current process
    max_my_row = (size_t)((num_rows / comm_sz) * 1.5);

    //init hashmap
    hashmap_init(&map, hashmap_hash_string, strcmp);

    //get the range of char distributed to current process
    mpi_get_let_range(comm_sz, my_rank, l_let, r_let, &my_l_let, &my_r_let, &my_let_count);

    if (my_rank == 0) {
        int a;
        scanf("%d", &a);
    }
    MPI_Barrier(MPI_COMM_WORLD);


    //allocate memory for l_str_arr
    l_str_arr = calloc(max_my_row , sizeof(char *));
    if(!l_str_arr)
        local_ok = 0;
    Check_for_error(local_ok, "failed to calloc!");

    for(it = 0; it < max_my_row; ++it){
        l_str_arr[it] = calloc(MAX_STRING_LEN, sizeof(char ));
        if(!l_str_arr[it])
            local_ok = 0;
    }
    Check_for_error(local_ok, "failed to calloc!");

    //open file
    ierr = MPI_File_open(MPI_COMM_WORLD, filepath, MPI_MODE_RDONLY, MPI_INFO_NULL, &file);
    if (ierr != MPI_SUCCESS) {
        local_ok = 0;
    }
    Check_for_error(local_ok, "cant open file!");

    //read file to buffer
    mpi_read_file(filepath, &file, &buffer, &file_size);

    //distribute task
    mpi_distribute_task(file_size, buffer, l_str_arr, my_l_let, my_r_let, &my_row);

    for(it = 0; it<my_row; ++it){
        count_p = hashmap_get(&map, l_str_arr[it]);
        if (count_p) {
            ++(*count_p);
        }
        else{
            count_p = calloc(1, sizeof(size_t));
            *count_p = 1;
            int r = hashmap_put(&map, l_str_arr[it], count_p);
            if (r != 0) {
                local_ok = 0;
            }
        }
    }
    Check_for_error(local_ok, "failed to put in hashmap!");

    my_num_groups = hashmap_size(&map);
    printf("myid:%d\tnum_groups:%zu\n", my_rank, my_num_groups);
    hashmap_foreach(key, count_p, &map) {
        printf("%ld\t%s", *count_p, key);
    }

    MPI_File_close(&file);

    //free
    hashmap_foreach_data(count_p, &map) {
        free(count_p);
    }
    hashmap_cleanup(&map);

    for (it = 0; it < max_my_row; ++it) {
        free(l_str_arr[it]);
    }
    free(l_str_arr);
    free(buffer);

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
            if (c == '\n') {
                str_arr[cur_row][cur_col] = '\0';
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