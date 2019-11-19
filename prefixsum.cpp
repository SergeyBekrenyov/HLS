#include <stdio.h>
#include <stdlib.h>
#include "HLS/hls.h"
#include "HLS/ac_int.h"

#define TEST_SIZE 128
#define TEST_SIZE_bits 8
#define SEED 4

// tb function remains the same as straight implementation
void test_prefixsum(int in[TEST_SIZE], int out[TEST_SIZE])
{
  out[0] = in[0];
  for (int i = 1; i < TEST_SIZE; i++){
    out[i] = out[i-1] + in[i];
  }
}

// book example Figure 8.1
component void prefixsum(int in[TEST_SIZE], int out[TEST_SIZE] )
{
  ac_int<TEST_SIZE_bits, false> i;
  out[0] = in[0];
  for (i = 1; i < TEST_SIZE; i++){
    //#pragma HLS_PIPELINE Xilinx's version - to replace with Altera's one
    out[i] = out[i-1] + in[i];
  }
}

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
    if(result[i] != golden[i])
      passed = false;
  }
    
  if (passed) {
    printf("PASSED\n");
  } else {
    printf("FAILED\n");
  }

  return 0;
}
