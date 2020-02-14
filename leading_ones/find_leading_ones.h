#include <stdio.h>
#include <stdlib.h>
#include "HLS/hls.h"
#include "HLS/ac_int.h"

#define NUM_BITS 3

#ifndef __NEXTPOW2__
#define __NEXTPOW2__
template<int N_VAL>
struct NEXT_POW2{
enum {
    val = N_VAL <= 1 ? 1: N_VAL <= 2 ? 2 : N_VAL <= 4 ? 4 :
    N_VAL <= 8 ? 8 : N_VAL <= 16 ? 16 : N_VAL <= 32 ? 32 :
    N_VAL <= 64 ? 64 : N_VAL <= 128 ? 128 : N_VAL <= 256 ? 256 :
    N_VAL <= 512 ? 512 : N_VAL <= 1024 ? 1024 :N_VAL <= 2048 ? 2048 :
    N_VAL <= 4096 ? 4096 : N_VAL <= 8192 ? 8192 : N_VAL <= 16384 ? 16384 :
    N_VAL <= 32768 ? 32768 : N_VAL <= 65536 ? 65536 : 1048576
    };
};
#endif //__NEXTPOW2__

#ifndef __LOG2CEIL__
#define __LOG2CEIL__
template<int N_VAL>
struct LOG2_CEIL{
enum {
    val = N_VAL <= 1 ? 1: N_VAL <= 2 ? 1 : N_VAL <= 4 ? 2 :
    N_VAL <= 8 ? 3 : N_VAL <= 16 ? 4 : N_VAL <= 32 ? 5 :
    N_VAL <= 64 ? 6 : N_VAL <= 128 ? 7 : N_VAL <= 256 ? 8 :
    N_VAL <= 512 ? 9 : N_VAL <= 1024 ? 10 : N_VAL <= 2048 ? 11 :
    N_VAL <= 4096 ? 12 : N_VAL <= 8192 ? 13 : N_VAL <= 16384 ? 14 :
    N_VAL <= 32768 ? 15 : N_VAL <= 65536 ? 16 : 32
    };
};
#endif //__LOG2CEIL__