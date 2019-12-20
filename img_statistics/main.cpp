// main.cpp
// functional verification tb
#include "HLS/hls.h"
#include "img_stat.h"
#include "bmp_tools.h"

#include <string>
#include <stdio.h> //printf
#include <stdlib.h> // malloc, free

#include "HLS/hls.h"
#include "bmp_tools.h"
#include "img_stat.h"

/*
0 - BLK_SAT
1 - WHT_SAT
2 - Min
3 - Max
4 - Sum
*/


void hist_calc_golden(pxl_v8 a_val, int* result){

  for (int p = 0; p < 8; ++p){
      // histogram
      result[1] += (a_val.data[p]  > WHT_SAT) ? 1 : 0;
      result[0] += (a_val.data[p]  < BLK_SAT) ? 1 : 0;
      //min
      if(result[2] > a_val.data[p])
          result[2] = a_val.data[p];
      /*if(result[2+2*p] > a_val.data[p])
          result[2+2*p] = a_val.data[p];*/
      //max
      if(result[3] < a_val.data[p])
          result[3] = a_val.data[p];
      /*if(result[2+2*p+1] < a_val.data[p])
          result[2+2*p+1] = a_val.data[p];*/
      result[4] = (result[4] + a_val.data[p]);///2;
  }
}

/*void hist_cal_ver(int i, int* result){
  result[1] += (i  > WHT_SAT) ? 1 : 0;
  result[0] += (i  < BLK_SAT) ? 1 : 0;
  //min
  if(result[2] > i)
     result[2] = i;
  //max
  if(result[3] < i)
     result[3] = i;
}*/

int main(void) {
  bool passed = true;
  int result[2+2+1] = {0};// all zeroes???
  int golden[2+2+1] = {0, 0, MAX_VAL, MIN_VAL, 0};
  // input AVL-ST
  input_image_stream in_img_s;
  //
  pxl_v8 a;
  // image files
  std::string input_bmp_filename    = "test.bmp";
  // load image
  unsigned int* in_img = 0;
  int rows, cols;
  read_bmp(input_bmp_filename.c_str(), &in_img, rows, cols);

  printf("WhiteSat=%d, BlackSat=%d \n", WHT_SAT, BLK_SAT);
  // fill image stream
  for(int j= 0; j < rows;++j){
      for(int i= 0; i < cols;i=i+8){
        for(int p = 0; p < 8; ++p){
          a.data[p]= (pxl_b12) (in_img[j*cols+i+p] & 0xFFF) ;
        }
        // write to the stream - push FIFO
        in_img_s.write(a);
        //
        hist_calc_golden(a, golden);
      }
  }
  
  img_stat( in_img_s,// input image stream
            rows, cols,	// frame size
            result);

  printf("Golden White = %d, Black = %d \n", golden[1], golden[0]);
  golden[1] = 0; golden[0] = 0;
  printf("Golden Min   = %d, Max   = %d \n", golden[2], golden[3]);
  golden[2] = MAX_VAL;
  golden[3] = MIN_VAL;
  printf("Golden SUM = %d \n", golden[4]);
  golden[4] = 0;

  printf("UUT    White = %d, Black = %d \n", result[1], result[0]);
  result[1] = 0; result[0] = 0;
  printf("UUT Min   = %d, Max   = %d \n", result[2], result[3]);
  result[2] = MAX_VAL; result[3] = MIN_VAL;
  printf("UUT Average = %d \n", result[4]);
  golden[4] = 0;
  /*// second frame
  // fill image stream
  for(int j= 0; j < rows;++j){
      for(int i= 0; i < cols;i=i+8){
        for(int p = 0; p < 8; ++p){
          a.data[p]= (pxl_b12) (in_img[j*cols+i+p] & 0xFFF) ;
        }
        // write to the stream - push FIFO
        in_img_s.write(a);
        //
        hist_calc_golden(a, golden);
      }
  }
  // second invocation
  img_stat( in_img_s,// input image stream
            rows, cols,	// frame size
            result);

  printf("UUT    White = %d, Black = %d \n", result[1], result[0]);
  for(int p = 0; p < 2; ++p){
    printf("%d UUT Min   = %d, Max   = %d \n", p, result[2+2*p], golden[2+p*2+1]);
    if(result[2+2*p] != golden[2+2*p]){
      passed = false;
      printf("Min failed \n");
    }
    if(result[2+2*p+1] != golden[2+2*p+1]){
      passed = false;
      printf("Max failed \n");
    }
  }
  printf("UUT Average = %d\n", result[4]);
  if(result[4] != golden[4]){
      passed = false;
      printf("Sum failed %d != %d\n", result[4], golden[4]);
  }*/
  if (passed) {
    printf("PASSED\n");
  } else {
    printf("FAILED\n");
  }

  return 0;
}


/*int main (void) {
  // image files
  std::string input_bmp_filename    = "test.bmp";
  std::string output_bmp_filename   = "downsampled.bmp";
  std::string expected_bmp_filename = "expected.bmp";

  // downsampling parameters
  unsigned int ratio = 128;

  // load image
  unsigned int* in_img = 0;;
  int rows, cols;
  read_bmp(input_bmp_filename.c_str(), &in_img, rows, cols);

  // run downsampling component
  input_image_stream in_img_s;
  output_image_stream out_img_s;
  for (int i = 0; i < (rows*cols); ++i) {
    in_img_s.write(in_img[i]);
  }
  resize(ratio, rows, cols, in_img_s, out_img_s);

  // save output image
  int out_rows = rows / (SCALE_GRAN / ratio);
  int out_cols = cols / (SCALE_GRAN / ratio);
  unsigned int* out_img = (unsigned int*) malloc(out_rows * out_cols * sizeof(unsigned int));
  for (int i = 0; i < (out_rows*out_cols); ++i) {
    out_img[i] = out_img_s.read();
  }
  write_bmp(output_bmp_filename.c_str(), out_img, out_rows, out_cols);

  // load expected output and compare
  unsigned int* exp_img = 0;;
  int exp_rows, exp_cols;
  read_bmp(expected_bmp_filename.c_str(), &exp_img, exp_rows, exp_cols);

  // check dimensions
  bool passed = (out_rows == exp_rows) && (out_cols == exp_cols);
  if (!passed) {
    printf("ERROR: dimensions (%d, %d) != (%d, %d)\n", out_cols, out_rows, exp_cols, exp_rows);
  }

  // compare image data
  if (passed) {
    for (int i = 0; i < (out_rows*out_cols); ++i) {
      passed &= (out_img[i] == exp_img[i]);
    }
  }

  if (passed) {
    printf("PASSED\n");
  } else {
    printf("FAILED\n");
  }

  // free memory
  free(in_img);
  free(out_img);
  free(exp_img);

  return 0;
}*/
