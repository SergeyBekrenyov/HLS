#include <stdio.h>
#include <stdlib.h>
#include "HLS/hls.h"
#include "HLS/ac_int.h"
#include "bayer.h"

int main(void) {
  bool passed = false;

  int width   = MaxWdth;
  int height  = MaxHght;

  pxl_N line_buf[5][MaxWdth];// = {0x0 0x0};//
  pxl_N *p_line_buf = NULL;
  // Create the streams
  ihc::stream_in<pxl_N>  str_A;
  
  for (int j = 0; j < height; ++j){
    for (int i = 0; i < width; ++i){
      update_line_buffers(width, height, test_img[j][i], p_line_buf);
    }
  }
  
  if (passed) {
    printf("PASSED\n");
  } else {
    printf("FAILED\n");
  }

  return 0;
}