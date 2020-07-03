//
// Created by hezhiwei on 6/26/20.
//
#include "test.h"

void test_count_string_sort() {
    char** str_arr = (char**)malloc(10 * sizeof(char*));
    size_t* begin_pos, *end_pos;
    for (int i = 0; i < 10; ++i) {
        str_arr[i] = (char*)malloc(256*sizeof(char));
    }
    strcpy(str_arr[0], "aa");
    strcpy(str_arr[1], "bb");
    strcpy(str_arr[2], "cc");
    strcpy(str_arr[3], "a");
    strcpy(str_arr[4], "b");
    strcpy(str_arr[5], "c");
    strcpy(str_arr[6], "aa");
    strcpy(str_arr[7], "bb");
    strcpy(str_arr[8], "c");
    strcpy(str_arr[9], "abcd");

    first_char_count_string_sort(&str_arr, 10, &begin_pos, &end_pos);

    for (int i = 0; i < 10; ++i) {
        printf("%s", str_arr[i]);
        printf("\n");
    }

    printf("begin_pos and end_pos:\n");
    for (int i = 0; i < 1<<8; ++i) {
        printf("%d %zd  %zd\n", i, begin_pos[i], end_pos[i]);
    }

    for (int i = 0; i < 10; ++i) {
        free(str_arr[i]);
    }
    free(str_arr);
}
void test_omp_count_string_sort(){
    char** str_arr = (char**)malloc(10 * sizeof(char*));
    size_t* begin_pos, *group_lens;
    for (int i = 0; i < 10; ++i) {
        str_arr[i] = (char*)malloc(256*sizeof(char));
    }
    strcpy(str_arr[0], "aa");
    strcpy(str_arr[1], "bb");
    strcpy(str_arr[2], "cc");
    strcpy(str_arr[3], "a");
    strcpy(str_arr[4], "b");
    strcpy(str_arr[5], "c");
    strcpy(str_arr[6], "aa");
    strcpy(str_arr[7], "bb");
    strcpy(str_arr[8], "c");
    strcpy(str_arr[9], "abcd");

    omp_first_char_count_string_sort(&str_arr, 10, &begin_pos, &group_lens);

    for (int i = 0; i < 10; ++i) {
        printf("%s", str_arr[i]);
        printf("\n");
    }

    printf("begin_pos and end_pos:\n");
    for (int i = 0; i < 1<<8; ++i) {
        printf("%c %zd  %zd\n", i, begin_pos[i], group_lens[i]);
    }

    for (int i = 0; i < 10; ++i) {
        free(str_arr[i]);
    }
    free(str_arr);
}
void test_omp_count_string_sort2(){
    char** str_arr = (char**)malloc(10 * sizeof(char*));
    size_t* begin_pos, *group_lens;
    for (int i = 0; i < 10; ++i) {
        str_arr[i] = (char*)malloc(256*sizeof(char));
    }
    strcpy(str_arr[0], "aa");
    strcpy(str_arr[1], "bb");
    strcpy(str_arr[2], "cc");
    strcpy(str_arr[3], "a");
    strcpy(str_arr[4], "b");
    strcpy(str_arr[5], "c");
    strcpy(str_arr[6], "aa");
    strcpy(str_arr[7], "bb");
    strcpy(str_arr[8], "c");
    strcpy(str_arr[9], "abcd");


    omp_first_char_count_string_sort2(&str_arr, 10, &begin_pos, &group_lens);

    for (int i = 0; i < 10; ++i) {
        printf("%s", str_arr[i]);
        printf("\n");
    }

    printf("begin_pos and end_pos:\n");
    for (int i = 0; i < 1<<8; ++i) {
        printf("%c %zd  %zd\n", i, begin_pos[i], group_lens[i]);
    }

    for (int i = 0; i < 10; ++i) {
        free(str_arr[i]);
    }
    free(str_arr);
}
void test_quick_string_sort(){
    char** str_arr = (char**)malloc(10 * sizeof(char*));

    for (int i = 0; i < 10; ++i) {
        str_arr[i] = (char*)malloc(256*sizeof(char));
    }
    strcpy(str_arr[0], "aad");
    strcpy(str_arr[1], "abc");
    strcpy(str_arr[2], "bcb");
    strcpy(str_arr[3], "bda");
    strcpy(str_arr[4], "bad");
    strcpy(str_arr[5], "cbc");
    strcpy(str_arr[6], "dcb");
    strcpy(str_arr[7], "ddhh");
    strcpy(str_arr[8], "da");
    strcpy(str_arr[9], "dbcd");


    quick_sort(str_arr, 10, 0);

    for (int i = 0; i < 10; ++i) {
        printf("%s", str_arr[i]);
        printf("\n");
    }
}
void test_quick_sort_partial(){
    char** str_arr = (char**)malloc(10 * sizeof(char*));
    for (int i = 0; i < 10; ++i) {
        str_arr[i] = (char*)malloc(256*sizeof(char));
    }
    strcpy(str_arr[0], "aad");
    strcpy(str_arr[1], "abc");
    strcpy(str_arr[2], "bcb");
    strcpy(str_arr[3], "bda");
    strcpy(str_arr[4], "bad");
    strcpy(str_arr[5], "cbc");
    strcpy(str_arr[6], "dcb");
    strcpy(str_arr[7], "ddhh");
    strcpy(str_arr[8], "da");
    strcpy(str_arr[9], "dbcd");

    size_t* begin_pos, *end_pos;
    first_char_count_string_sort(&str_arr, 10, &begin_pos, &end_pos);
    quick_sort_partial(str_arr, begin_pos, end_pos, 1);

    for (int i = 0; i < 10; ++i) {
        printf("%s", str_arr[i]);
        printf("\n");
    }
}
void test_omp_quick_sort_partial(){
    char** str_arr = (char**)malloc(10 * sizeof(char*));
    for (int i = 0; i < 10; ++i) {
        str_arr[i] = (char*)malloc(256*sizeof(char));
    }
    strcpy(str_arr[0], "aad");
    strcpy(str_arr[1], "abc");
    strcpy(str_arr[2], "bcb");
    strcpy(str_arr[3], "bda");
    strcpy(str_arr[4], "bad");
    strcpy(str_arr[5], "cbc");
    strcpy(str_arr[6], "dcb");
    strcpy(str_arr[7], "ddhh");
    strcpy(str_arr[8], "da");
    strcpy(str_arr[9], "dbcd");

    size_t* begin_pos, *end_pos;
    first_char_count_string_sort(&str_arr, 10, &begin_pos, &end_pos);
    omp_quick_sort_partial(str_arr, begin_pos, end_pos, 1);

    for (int i = 0; i < 10; ++i) {
        printf("%s", str_arr[i]);
        printf("\n");
    }
}

void test_assign_group(){
    char** str_arr = (char**)malloc(10 * sizeof(char*));
    char* group_keys[10];
    size_t group_lens[10];
    size_t num_groups;

    for (int i = 0; i < 10; ++i) {
        str_arr[i] = (char*)malloc(256*sizeof(char));
    }
    strcpy(str_arr[0], "abdh");
    strcpy(str_arr[1], "bcflmn");
    strcpy(str_arr[2], "abdh");
    strcpy(str_arr[3], "abei");
    strcpy(str_arr[4], "abej");
    strcpy(str_arr[5], "bbej");
    strcpy(str_arr[6], "bcfk");
    strcpy(str_arr[7], "bcflmn");
    strcpy(str_arr[8], "abdh");
    strcpy(str_arr[9], "bcgo");

    size_t* begin_pos, *lens;

    omp_first_char_count_string_sort(&str_arr, 10, &begin_pos, &lens);
    omp_radix_sort_partial(str_arr, begin_pos, lens, 1);
    omp_assign_group(str_arr, group_keys, group_lens, &num_groups, 10, begin_pos, lens);

    printf("# of groups: %zu \n", num_groups);
    for(size_t i =0;i<num_groups;++i){
        printf("%s\t%zd\n", group_keys[i], group_lens[i]);
    }


}
void test_radix_sort_main(){
    char** str_arr = (char**)malloc(10 * sizeof(char*));

    for (int i = 0; i < 10; ++i) {
        str_arr[i] = (char*)malloc(256*sizeof(char));
    }
    strcpy(str_arr[0], "bad");
    strcpy(str_arr[1], "abc");
    strcpy(str_arr[2], "bdb");
    strcpy(str_arr[3], "bda");
    strcpy(str_arr[4], "bad");
    strcpy(str_arr[5], "abc");
    strcpy(str_arr[6], "dab");
    strcpy(str_arr[7], "ddhh");
    strcpy(str_arr[8], "da");
    strcpy(str_arr[9], "dbcd");


    radix_sort_main(str_arr, 10, 0);

    for (int i = 0; i < 10; ++i) {
        printf("%s", str_arr[i]);
        printf("\n");
    }
}

void test_parallel_read_file(const char* file_path, int num_rows){
    char** str_srr1 = calloc(num_rows, sizeof(char*));
    char** str_srr2 = calloc(num_rows, sizeof(char*));
    int i = 0;
    FILE *f;
    size_t n = 0;
    double total = 0;
    for(int k = 0;k<10000;++k){
        double start, end, el;
        start = omp_get_wtime();
        f = fopen(file_path, "r");
        for(i = 0;i<num_rows;++i){
            getline(&str_srr2[i], &n, f);
        }
        fclose(f);
        end = omp_get_wtime();
        total += end-start;
    }
    printf("%d %f \n", 0, total);

    for(int n_thread = 1;n_thread<=16;n_thread++){
        for(int k = 0;k<10000;++k){
            double start, end, el;
            start = omp_get_wtime();
            f = fopen(file_path, "r");
#pragma omp parallel for num_threads(n_thread) default(none), shared(num_rows, str_srr2, n, f)
            for(i = 0;i<num_rows;++i){
                getline(&str_srr2[i], &n, f);
            }
            fclose(f);
            end = omp_get_wtime();
            total += end-start;
        }

        printf("%d %f \n", n_thread, total);
        total = 0;
    }


    for(i = 0;i<num_rows;++i){
//        printf("%s\t%s", str_srr1[i], str_srr2[i]);
        if(strcmp(str_srr1[i], str_srr2[i]) != 0){
            printf("wrong");
        }
    }

}