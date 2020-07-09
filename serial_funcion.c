//
// Created by hezhiwei on 6/27/20.
//

#include "serial_funciton.h"


void swapInt(size_t *pInt, size_t *pInt1) {
    size_t temp = *pInt;
    *pInt = *pInt1;
    *pInt1 = temp;
};

void swapStringPointer(char** s1_p, char** s2_p) {
    char* tmp = *s1_p;
    *s1_p = *s2_p;
    *s2_p = tmp;
}

void first_char_count_string_sort(char*** str_arr_p , size_t num, size_t** begin_pos_p, size_t** end_pos_p)
{
    char** str_arr = *str_arr_p;
    size_t* cnt = calloc(SIZE_OF_CNT, sizeof(size_t)); //use for counting sort
    size_t* beg_pos = calloc(SIZE_OF_CNT, sizeof(size_t));
    size_t it = 0, pre_sum = 0;
    int i=0;

    uint16_t * first_lets = calloc(num, sizeof(uint16_t));  //store the first elem of each string
    char** temp = calloc(num, sizeof(char*));  //store the sorted array
    if (!(beg_pos && cnt && first_lets && temp)) {
        fprintf(stderr, "fail to malloc in first_char_count_string_sort");
        exit(-1);
    }

    //save the first letter of each string
    for (it = 0; it < num; ++it) {
        first_lets[it] = uint16_map_to_idx((str_arr[it][0]<<8)|str_arr[it][1]);
    }

    //cal # of appearance of each letter
    for (it = 0; it < num; ++it) {
        ++cnt[first_lets[it]];
    }

    // cal begin pos of each category
    for (i = 0; i < SIZE_OF_CNT; ++i) {
        swapInt(&pre_sum, &cnt[i]);
        pre_sum += cnt[i];
    }
    memcpy(beg_pos, cnt, sizeof(size_t)*(SIZE_OF_CNT));


    // sort and save res to temp
    for (it = 0; it < num; ++it) {
        temp[cnt[first_lets[it]]++] = str_arr[it];
    }

    //return value
    *str_arr_p = temp;
    *begin_pos_p = beg_pos;
    *end_pos_p = cnt;

    free(first_lets);
}

//todo:not by myself
void quick_sort(char **arr, size_t num, int level) {
    const int MAX_LEVELS = 64;
    size_t beg[MAX_LEVELS], end[MAX_LEVELS], L, R;
    int i = 0;

    beg[0] = 0;
    end[0] = num;
    while (i >= 0) {
        L = beg[i];
        R = end[i];
        if (L + 1 < R--) {
            char* piv = arr[L];
            if (i == MAX_LEVELS - 1){
                fprintf(stderr, "quick_sort: Please increase MAX_LEVELS!");
                exit(-1);
            }
            while (L < R) {
                while (strcmp(piv + level, arr[R] + level)<=0 && L < R)
                    R--;
                if (L < R)
                    arr[L++] = arr[R];
                while (strcmp(arr[L] + level, piv + level)<=0 && L < R)
                    L++;
                if (L < R)
                    arr[R--] = arr[L];
            }
            arr[L] = piv;
            if (L - beg[i] > end[i] - R) {
                beg[i + 1] = L + 1;
                end[i + 1] = end[i];
                end[i++] = L;
            } else {
                beg[i + 1] = beg[i];
                end[i + 1] = L;
                beg[i++] = L + 1;
            }
        } else {
            i--;
        }
    }
}

void quick_sort_partial(char** arr, size_t* starts, size_t *ends, int level){
    size_t i;
    for(i=0; i < SIZE_OF_CNT; ++i){
        if(ends[i] != 0){
            quick_sort(arr+starts[i], ends[i] - starts[i], level);
        }
    }
}

void radix_sort_partial(char** arr, size_t* starts, size_t *ends, int level){
    size_t i;
    for(i=0; i < SIZE_OF_CNT; ++i){
        if(ends[i] != 0){
            radix_sort_main(arr+starts[i], ends[i] - starts[i], level);
        }
    }
}

void radix_sort_main(char** arr, size_t num, int level){
    char** temp_arr = calloc(num, sizeof(char *));
    uint8_t* letter_base8 = calloc(num, sizeof(uint8_t));
    uint16_t* letter_base16 = calloc(num, sizeof(uint16_t));
    uint16_t* letters = letter_base16;
    size_t cnt[SIZE_OF_CNT16] = {0};
    size_t i, j;

    if(!(temp_arr && letter_base8 && letter_base16)){
        fprintf(stderr, "cant allocate memory in radix sort main");
        exit(-1);
    }

    for (i = 0; i < num; ++i) {
        uint16_t sc = arr[i][level];
        letters[i] = sc == 0 ? 0 : ((sc << 8) | arr[i][level + 1]);
    }

    for(i = 0; i<num;++i){
        ++cnt[letters[i]];
    }

    size_t pre_sum = 0;
    for(i = 0; i<SIZE_OF_CNT16; ++i){
        swapInt(&pre_sum, &cnt[i]);
        pre_sum += cnt[i];
    }

    for (i = 0; i < num; ++i) {
        swapStringPointer(&temp_arr[cnt[letters[i]]++], &arr[i]);
    }

    //copy the terminated string back to arr
    for (i = 0; i < (1 << 8); ++i) {
        size_t b = i == 0 ? 0 : cnt[(i << 8) - 1];
        size_t e = cnt[i << 8];
        for (j = b; j < e; ++j) {
            arr[j] = temp_arr[j];
        }
    }

    for (i = 1; i < (1 << 16); ++i) {
        if ((i & 0xFF) != 0 && cnt[i] - cnt[i - 1] >= 1) {
            radix_sort_inner(arr, temp_arr, arr, letter_base8, letter_base16, cnt[i - 1], cnt[i], level+2);
        }
    }


    free(temp_arr);
    free(letter_base8);
    free(letter_base16);
}

inline void radix_sort_inner(char** arr, char** src, char** dest, uint8_t* leb8, uint16_t* leb16, size_t beg, size_t end, int level){
    size_t num = end - beg;
    size_t i;
    if(num <= RADIX_MIN_NUM){
        if (arr == dest) {
            for (i = beg; i < end; ++i) {
                swapStringPointer(&arr[i], &src[i]);
            }
        }
        if(num > 1){
            quick_sort(arr+beg, num, level);
        }
    }
    else if (num < (1 << 16)) {
        radix_sort8(arr, src, dest, leb8, leb16, beg, end, level);
    }
    else {
        radix_sort16(arr, src, dest, leb8, leb16, beg, end, level);
    }
}

void radix_sort8(char** arr, char** src, char** dest, uint8_t* leb8, uint16_t* leb16, size_t beg, size_t end, int level){
    size_t num = end - beg;
    size_t i;
    size_t cnt[SIZE_OF_CNT8] = {0};
    uint8_t* letters = leb8 + beg;

    for (i = 0; i < num; ++i) {
        letters[i] = src[i+beg][level];
    }

    for (i = 0; i < num; ++i) {
        ++cnt[letters[i]];
    }

    size_t pre_sum = 0;
    for (i = 0; i < (SIZE_OF_CNT8); ++i) {
        swapInt(&cnt[i], &pre_sum);
        pre_sum += cnt[i];
    }

    for (i = 0; i < num; ++i) {
        swapStringPointer(&dest[beg + cnt[letters[i]]++], &src[beg + i]);
    }

    if (arr != dest) {
        size_t j = 0, e = cnt[0];
        for (j = 0; j < e; ++j) {
            swapStringPointer(&src[j+beg], &dest[j+beg]);
        }
    }

    for (i = 1; i < (SIZE_OF_CNT8); ++i) {
        if (cnt[i] - cnt[i - 1] >= 1) {
            radix_sort_inner(arr, dest, src, leb8, leb16, beg + cnt[i-1], beg + cnt[i], level+1);
        }
    }
}

void radix_sort16(char** arr, char** src, char** dest, uint8_t* leb8, uint16_t* leb16, size_t beg, size_t end, int level){
    size_t num = end - beg;
    size_t i, j;
    size_t cnt[SIZE_OF_CNT16] = {0};
    uint16_t* letters = leb16 + beg;

    for (i = 0; i < num; ++i) {
        uint16_t sc = arr[i+beg][level];
        letters[i] = sc == 0 ? 0 : ((sc << 8) | arr[i+beg][level + 1]);
    }

    for(i = 0; i<num;++i){
        ++cnt[letters[i]];
    }

    size_t pre_sum = 0;
    for(i = 0; i<SIZE_OF_CNT16; ++i){
        swapInt(&pre_sum, &cnt[i]);
        pre_sum += cnt[i];
    }

    for (i = 0; i < num; ++i) {
        swapStringPointer(&dest[beg + cnt[letters[i]]++], &src[beg + i]);
    }

    if (arr != dest) {
        for (i = 0; i < 1 << 8; ++i) {
            size_t b = i == 0 ? 0 : cnt[(i << 8) - 1];
            size_t e = cnt[i << 8];
            for (j = b; j < e; ++j) {
                swapStringPointer(&src[beg + j], &dest[beg + j]);
            }
        }
    }

    for (i = 1; i < (SIZE_OF_CNT16); ++i) {
        if ((i & 0xFF) != 0 && cnt[i] - cnt[i - 1] >= 1) {
            radix_sort_inner(arr, dest, src, leb8, leb16, beg + cnt[i-1], beg + cnt[i], level+2);
        }
    }

}

unsigned int next_power_2(unsigned int n)
{
    unsigned count = 0;

    if (n && !(n & (n - 1)))
        return n;

    while( n != 0)
    {
        n >>= 1;
        count += 1;
    }

    return 1 << count;
}

char **alloc_2d_char(size_t rows, size_t cols) {
    char *data = (char *)malloc(rows*cols*sizeof(char ));
    char **array= (char **)malloc(rows*sizeof(char* ));
    size_t i;
    for (i=0; i<rows; i++)
        array[i] = &(data[cols*i]);
    return array;
}

int uint16_map_to_idx(uint16_t ch){
    uint16_t diff = ch - 0X4141;
    uint16_t idx = (diff/0X00FF)*7 + diff%0X00FF;
    if (!(idx>=0&&idx<=63)){
        fprintf(stderr, "value %d err in uint16_map_to_idx",ch);
        exit(-1);
    }
    return idx;
}