#include <stdio.h>
#include <stdlib.h>
#include "HLS/hls.h"
#include "HLS/ac_int.h"

// count number of pixels in two bins
// black from 0 to lw_lim
// white from hw_lim to 2^12-1
// ?Master is in charge to reset bins at the start of the frame
// both lw_lim, hg_lim - AVMM regs
// result is AVMM also but 2x32 bits???
// bins to implement as BRAM (hls_avalon_slave_memory_argument or local 2-port BRAM??? )
// bins to be static? "hls_init_on_reset static int" or "hls_init_on_powerup static int "
const 	ac_int<4, false> N_pxls = 1; // use all pixels
//const 	ac_int<4, false>  N_pxls = 1; // use only 1 channel
typedef ac_int<12, false> pxl_b12;


// using N pixels, 12-bit each
struct pxl_N {
  // Passing N elements at a time to the component for processing
  pxl_b12 data[N_pxls];
};

component void debayer(ihc::stream_in<pxl_N>&  i_str, 				// input image stream
                       hls_avalon_slave_register_argument int width,		// width
                       hls_avalon_slave_register_argument int height,		// heigth
                       ihc::stream_in<pxl_N>&  o_str, 				// input image stream
 )
{
  // line buffers - BRAM!
  pxl_b12 line_buf[MaxWdth][5];// 
  //window buffer - registers. 5x5 filters
  pxl_b12 window_buf[5*N_pxls][5];
  
  //update line buffers
  for(int i = 0; i < width; i+=N_pxls){
    for(int j = 0; j < N_pxls; j++){
      line_buf[i+j]
    }
  }
}

#define TEST_SIZE 1024
#define SEED 4

#define c_lw_lim 160
#define c_hg_lim 4032

void hist_cal_ver(int i, int lw_lim, int hg_lim, int*result){
  /*if      (i > hg_lim)
    result[1]++;
  else if (i < lw_lim)
    result[0]++;*/
    printf("hist_cal_ver %d\n", i);
    result[1] += ((i >> 4) > (hg_lim >> 4)) ? 1 : 0;
    result[0] += ((i >> 4) < (lw_lim >> 4)) ? 1 : 0;
    printf("%d, %d\n", result[1], result[0]);
}

int main(void) {

  pxl_b12 A[TEST_SIZE*8];
  int result[2] = {0, 0};
  int golden[2] = {0, 0};
  //int presult = *result;
  // Create the streams
  ihc::stream_in<pxl_v8>  str_A;
  
  // Prepare the input data
  srand(SEED);
  for (int i = 0; i < TEST_SIZE*8; ++i) {
    A[i] = abs(rand());
    //printf("Test SEED= %d\n", (int) A[i]);
  }

  for (int i = 0; i < TEST_SIZE; i++) {
    pxl_v8 a;
    for(int j = 0; j < 8; ++j) {
      a.data[j] = A[i+j];
      printf("Test %d,%d = %d\n", i, j, (int) a.data[j]);
      if(j < N_pxls)
        hist_cal_ver((int) a.data[j], c_lw_lim, c_hg_lim, golden);
    }
    str_A.write(a);
  }

  // Run the component
  hist_calc(str_A, TEST_SIZE, c_lw_lim, c_hg_lim, result);

  printf("Results: Black = %d, White = %d\n", result[0], result[1]);
  printf("Results: Black = %d, White = %d\n", golden[0], golden[1]);

  // Check the output
  /*bool passed = true;
  for (int i = 0; i < TEST_SIZE; i+=8) {
    pxl_v8 c = str_C.read();
    for (int j = 0; j < 8; ++j) {
      bool data_okay = (c.data[j] == (A[i+j] + B[i+j]));

      passed &= data_okay;
      if (!data_okay) {
        printf("ERROR: C[%d] = %d != %d\n", i+j, c.data[j], (A[i+j] + B[i+j]));
      }
    }
  }

  if (passed) {
    printf("PASSED\n");
  } else {
    printf("FAILED\n");
  }*/

  return 0;
}
