#include <stdio.h>
#include <stdlib.h>
#include "HLS/hls.h"
#include "HLS/ac_int.h"

//#define original
//#define local_mem
#define dependence

#define INPUT_SIZE 128
#define VALUE_MAX  128
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

//typedef ac_int <clogb2(VALUE_MAX), false> ival;


// tb function remains the same as straight implementation
void test_histogram(int in[INPUT_SIZE], int hist[VALUE_MAX]) {
  int val;
  for(int i = 0; i < INPUT_SIZE; i++) {
    //#pragma HLS PIPELINE
    val = in[i];
    hist[val] = hist[val] + 1;
  }
}

#ifdef original
// book example Figure 8.6
component void histogram(int in[INPUT_SIZE], int hist[VALUE_MAX]) {
  int val;
  for(int i = 0; i < INPUT_SIZE; i++) {
    //#pragma HLS PIPELINE
    val = in[i];
    hist[val] = hist[val] + 1;
  }
}
#endif // original
#ifdef local_mem
// use local mem to reduce II, using labels for loops
component void histogram(int in[INPUT_SIZE], int hist[VALUE_MAX]) {
  int val, i;
  static int hist_lcl[VALUE_MAX];
  for(i = 0; i < INPUT_SIZE; i++) {
    //#pragma HLS PIPELINE
    val = in[i];
    hist_lcl[val] = hist_lcl[val] + 1;
  }

  //copy results
  cp_rslt:
  for (i = 0; i < INPUT_SIZE; i++){
    hist[i] = hist_lcl[i];
  }
}
#endif //local_mem

#ifdef dependence
component void histogram(int in[INPUT_SIZE], int hist[VALUE_MAX]) {
  int acc = 0;
  int i, val;
  int old = in[0];
  //#pragma HLS DEPENDENCE variable=hist intra RAW false
  for(i = 0; i < INPUT_SIZE; i++) {
  //#pragma HLS PIPELINE II=1
    val = in[i];
    if(old == val) {
      acc = acc + 1;
    } else {
      hist[old] = acc;
      acc = hist[val] + 1;
    }
    old = val;
  }
  hist[old] = acc;
}
#endif //dependence

int main(void) {

  int test_vector[INPUT_SIZE];
  int result[VALUE_MAX]={0};
  int golden[VALUE_MAX]{0};
  bool passed = true;
  float var;
  // Prepare the input data
  srand(SEED);
  for (int i = 0; i < INPUT_SIZE; ++i) {
    // generate random number from 0 to 1.0
    var = (float)abs(rand())/RAND_MAX;
    // make it integer with given range
    test_vector[i] = (var*VALUE_MAX);
    //printf("i=%d, %f, %d\n", i, var, test_vector[i]);
  }

  test_histogram(test_vector, golden);
  histogram(test_vector, result);

  for (int i = 0; i < VALUE_MAX; ++i) {
    //printf("i=%d, %d, %d\n", i, result[i], golden[i]);
    if(result[i] != golden[i]){
      printf("Error i=%d, %d, %d\n", i, result[i], golden[i]);
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
