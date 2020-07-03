//
// Created by hezhiwei on 7/1/20.
//

#include "mpi_group_by.h"

void mpi_group_by(const char* filepath, size_t num_rows){
    int comm_sz, my_rank;
    int local_ok = 1;
    size_t my_row, it;
    char ** g_str_arr;
    char ** l_str_arr;
    char *  g_letters;
    char *  l_letters;
    size_t cnt[SIZE_OF_CNT8];
    MPI_File file;


    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    my_row = num_rows / comm_sz;
    if(my_rank < num_rows % comm_sz)
        ++my_row;

    //allocate
    l_letters = calloc(my_row, sizeof(char ));
    l_str_arr = calloc(my_row, sizeof(char *));
    if(!(l_letters && l_str_arr))
        local_ok = 0;
    for(it = 0; it < my_row; ++it){
        l_str_arr[it] = calloc(MAX_STRING_LEN, sizeof(char ));
        if(!l_str_arr[it])
            local_ok = 0;
    }

    if(my_rank == 0){
        g_letters = calloc(num_rows, sizeof(char ));
        g_str_arr = calloc(num_rows, sizeof(char *));
        if(!(g_letters && g_str_arr))
            local_ok = 0;
        for(it = 0;it < num_rows;++it){
            g_str_arr[it] = calloc(MAX_STRING_LEN, sizeof(char ));
            if(!g_str_arr[it])
                local_ok = 0;
        }
    }

    Check_for_error(local_ok, "failed to calloc!");

    MPI_File_open(MPI_COMM_WORLD, filepath, MPI_MODE_RDONLY, MPI_INFO_NULL, &file);


    mpi_read_file(&file, my_rank, comm_sz, l_str_arr);



    // Print off a hello world message
    printf("Hello world from processor %s, rank %d out of %d processors\n",
           "ubuntu", my_rank, comm_sz);

    MPI_File_close(&file);


    //free
    for(it = 0; it < my_row; ++it){
        free(l_str_arr[it]);
    }
    free(l_str_arr);
    free(l_letters);

//    if(my_rank == 0){
//        for(it = 0;it < num_rows;++it){
//           free(g_str_arr[it]);
//        }
//        free(g_str_arr);
//        free(g_letters);
//    }


    // Finalize the MPI environment.
    MPI_Finalize();
}

//todo:not by myself
void mpi_read_file(MPI_File *in, int my_rank, int comm_sz, char** l_str_arr) {
    MPI_Offset global_start, i;
    size_t my_size, cur_row = 0, cur_col = 0, start_offset = 0;
    char c;
    char *chunk;

    /* read in relevant chunk of file into "chunk",
     * which starts at location in the file global_start
     * and has size my_size
     */
    {
        MPI_Offset global_end;
        MPI_Offset file_size;

        /* figure out who reads what */
        MPI_File_get_size(*in, &file_size);
        file_size--;  /* get rid of text file eof */
        my_size = file_size / comm_sz;
        global_start = my_rank * my_size;
        global_end   = global_start + my_size - 1;
        if (my_size == comm_sz-1)
            global_end = file_size - 1;

        /* add overlap to the end of everyone's chunk except last proc... */
        if (my_rank != comm_sz-1)
            global_end += OVERLAP;

        my_size = global_end - global_start + 1;

        /* allocate memory */
        chunk = malloc( (my_size + 1)*sizeof(char));

        /* everyone reads in their part */
        MPI_File_read_at_all(*in, global_start, chunk, my_size, MPI_CHAR, MPI_STATUS_IGNORE);
        chunk[my_size] = '\0';
    }


    /*
     * everyone calculate what their start and end *really* are by going
     * from the first newline after start to the first newline after the
     * overlap region starts (eg, after end - overlap + 1)
     */

    size_t loc_start=0, loc_end= my_size - 1;
    if (my_rank != 0) {
        while(chunk[loc_start] != '\n') loc_start++;
        loc_start++;
    }
    if (my_rank != comm_sz-1) {
        loc_end-=OVERLAP;
        while(chunk[loc_end] != '\n') loc_end++;
    }
    my_size = loc_end - loc_start + 1;

    start_offset = global_start + loc_start;
    for(i = start_offset; i <= my_size + start_offset; ++i ){
        c = chunk[i];
        l_str_arr[cur_row][cur_col++] = c;
        if(c == '\n'){
            l_str_arr[cur_row++][cur_col] = '\0';
            cur_col = 0;
        }
    }
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