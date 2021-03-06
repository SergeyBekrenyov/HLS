#include <stdio.h>
#include <stdlib.h>
#include "bmp_tools.h"
#include "HLS/hls.h"
#include "HLS/ac_int.h"
#include "bayer.h"

// test array
/*pxl_N test_img[MAX_HGHT][MAX_WDTH] = {
                            0x000, 0x001, 0x002, 0x003, 0x004, 0x005, 0x006, 0x007,
                            0x010, 0x011, 0x012, 0x013, 0x014, 0x015, 0x016, 0x017,
                            0x020, 0x021, 0x022, 0x023, 0x024, 0x025, 0x026, 0x027,
                            0x100, 0x101, 0x102, 0x103, 0x104,
                            0x200, 0x201, 0x202, 0x203, 0x204,
                            0x300, 0x301, 0x302, 0x303, 0x304,
                            0x400, 0x401, 0x402, 0x403, 0x404};*/

void gen_test_img(pxl_N test_img[MAX_HGHT][MAX_WDTH]){
  for (int j = 0; j < MAX_HGHT; ++j){
    for (int i = 0; i < MAX_WDTH; ++i){
      test_img[j][i].data[0] = ((j & 0xF) << 8) + (i & 0xFF);
    }
  }
}

int main(void) {
  bool passed = false;

  int width   = MAX_WDTH;
  int height  = MAX_HGHT;
  pxl_N test_img[MAX_HGHT][MAX_WDTH] = {0};
  pxl_N line_buf[5][MAX_WDTH] = {0};
  pxl_N *p_line_buf = NULL;
  
  //
  pxl_b12 window[5][5];
  
  // image files
  std::string input_bmp_filename    = "test_gray.bmp";
  std::string output_bmp_filename   = "result.bmp";
  // load image
  unsigned int* in_img = 0;
  int rows, cols;
  read_bmp(input_bmp_filename.c_str(), &in_img, rows, cols);
  printf("InImage %dx%d \n", cols, rows);
  unsigned int* out_img = (unsigned int*) malloc(rows * cols * sizeof(unsigned int));

  // Create the streams
  ihc::stream_in<pxl_N>  str_A;
  
  /*for (int j = 0; j < 5; ++j){
    for (int i = 0; i < width; ++i){
      line_buf[j][i].data[0] = 0x0;
    }
  }*/

  for (int j = 0; j < rows; ++j){
    //printf("\n Line #%d : ", j); 
    for (int i = 0; i < cols; ++i){
      //printf(" 0x%3X", (int) test_img[j][i].data[0]);
      bf_window_5x5_and_line_buffer(in_img[j*cols+i], width, window);
	  out_img[j*cols+i] = (int) window[2][2];
	  //printf("InImage 0x%3X \n", in_img[j*cols+i]);
	  // printf("OutImage 0x%3X \n", out_img[j*cols+i]);
	  //printf("WindowCenter 0x%3X \n", (int) window[2][2]);
    }
  }
  
  write_bmp(output_bmp_filename.c_str(), out_img, rows, cols);

  if (passed) {
    printf("PASSED\n");
  } else {
    printf("FAILED\n");
  }

  return 0;
}