#include "test.h"
#include "measure.h"
#include "mpi_group_by.h"
int main(){
//    test_count_string_sort();
//    mes_omp_group_by("/home/hezhiwei/CLionProjects/ParallelProject/1M.txt", 1000000, "res1M.txt");
mpi_group_by("/home/hezhiwei/CLionProjects/ParallelProject/100M.txt", 100000000, 1);
}
