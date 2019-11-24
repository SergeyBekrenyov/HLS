#include <stdio.h>
#include <stdlib.h>
#include "HLS/hls.h"
#include "HLS/ac_int.h"

//#define original
#define local_mem

#define TEST_SIZE 128
#define TEST_SIZE_bits 8
#define SEED 4

constexpr int clogb2(int value){
  int val = 0;
  int clogb2 = 0;
  val = value - 1;
  for (; val > 0; ++clogb2) {
    val = val >> 1;
  }
  return clogb2+1;
}

// tb function remains the same as straight implementation
void test_prefixsum(int in[TEST_SIZE], int out[TEST_SIZE])
{
  out[0] = in[0];
  for (int i = 1; i < TEST_SIZE; i++){
    out[i] = out[i-1] + in[i];
  }
}

#ifdef original
// book example Figure 8.1
component void prefixsum(int arr_in[TEST_SIZE], int arr_out[TEST_SIZE])
{
  //int i; original index - 32 bit integer
  ac_int<clogb2(TEST_SIZE), false> i;
  arr_out[0] = arr_in[0];
  // ??? Intel's HLS compiler tries to pipeline loop
  // 18.1 says it's unknown pragma!
  // #pragma disable_loop_pipelining
  // #pragma unroll 1 // actually the same results as without it
  for (i = 1; i < TEST_SIZE; i++){
    //#pragma HLS_PIPELINE Xilinx's version - to replace with Altera's one
    arr_out[i] = arr_out[i-1] + arr_in[i];
  }
}
#endif // original
#ifdef local_mem
// use local mem to reduce II, using labels for loops
component void prefixsum(int arr_in[TEST_SIZE], int arr_out[TEST_SIZE])
{
  ac_int<clogb2(TEST_SIZE), false> i;
  static int arr_lcl[TEST_SIZE];
  arr_lcl[0] = arr_in[0];
  // local mem
  lcl_mem:
  // Intel's HLS compiler tries to pipeline loop
  //#pragma disable_loop_pipelining
  //#pragma unroll 1 // looks like acts the same as disable_loop_pipelining
  for (i = 1; i < TEST_SIZE; i++){
    //#pragma HLS_PIPELINE Xilinx's version - to replace with Altera's one
    arr_lcl[i] = arr_lcl[i-1] + arr_in[i];
  }
  //copy results
  cp_rslt:
  for (i = 0; i < TEST_SIZE; i++){
    arr_out[i] = arr_lcl[i];
  }
}
#endif //local_mem

int main(void) {

  int test_vector[TEST_SIZE];
  int result[TEST_SIZE];
  int golden[TEST_SIZE];
  bool passed = true;
  // Prepare the input data
  srand(SEED);
  for (int i = 0; i < TEST_SIZE; ++i) {
    test_vector[i] = abs(rand());
  }

  test_prefixsum(test_vector, golden);
  prefixsum(test_vector, result);

  for (int i = 0; i < TEST_SIZE; ++i) {
    //printf("%d, %d\n", result[i], golden[i]);
    if(result[i] != golden[i]){
      printf("i=%d, %d, %d\n", i, result[i], golden[i]);
      passed = false;
    }
  }
    
  if (passed) {
    printf("PASSED\n");
  } else {
    printf("FAILED\n");
  }

  return 0;
}
