#include <algorithm>
#include <scc/autoparallel.h>
#include "common.h"
#include <time.h>

uint64_t y;

__attribute__((noinline))
__attribute__((assertswarmified))
static void foo(){
    for(uint64_t i = 0; i < 1000; i++){
        y++;
    }
}


int main(int argc , const char** argv){
    clock_t start, stop;
    double ct;
    y = 0;
    start = clock();
    SCC_PARALLEL( foo() );
    stop = clock();
    ct = (stop - start) / (double)CLOCKS_PER_SEC;
    printf("Time: %7.3f sec.; Bits: %ld\n", ct, y);
    tests::assert_eq(y, 1000)

    return 0;
}
