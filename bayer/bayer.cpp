#include <stdio.h>
#include <stdlib.h>
#include "HLS/hls.h"
#include "HLS/ac_int.h"
#include "bayer.h"
#include "test.h"





/*component void debayer(ihc::stream_in<pxl_N>&  i_str, 				// input image stream
                       hls_avalon_slave_register_argument int width,		// width
                       hls_avalon_slave_register_argument int height,		// heigth
                       ihc::stream_in<pxl_N>&  o_str 				// input image stream
 )
{
  // line buffers - BRAM!
  //pxl_b12 line_buf[5][MaxWdth];// 
  // it might be better to use 5 independent arrays to provide random access
  pxl_N line0_buf[MaxWdth];
  pxl_N line1_buf[MaxWdth];
  pxl_N line2_buf[MaxWdth];
  pxl_N line3_buf[MaxWdth];
  pxl_N line4_buf[MaxWdth];
  //window buffer - registers. 5x5 filters
  pxl_b12 window_buf[5][5*N_pxls];
  
  //update line buffers
  for(int i = 0; i < width; i+=N_pxls){
    pxl_N i_str_val; // input stream pixel data
    i_str_val = i_str.read();b12j];
  }
}*/

void update_line_buffers(int width, int height, pxl_N i_pxls, pxl_N line_buf[]){
     
    //update line buffers
    for(int j = 0; j < height; j++){
      for(int i = 0; i < width; i+=N_pxls){
        pxl_N i_str_val; // input stream pixel data
        i_str_val = i_pxls;
        line_buf[4][i+j] = line_buf[3][i+j];
        line_buf[3][i+j] = line_buf[2][i+j];
        line_buf[2][i+j] = line_buf[1][i+j];
        line_buf[1][i+j] = line_buf[0][i+j];
        line_buf[0][i+j] = i_str_val[j];
      }
    }
    
    for (int j = 0; j < 5; ++j){
      printf("\nLine #%d", j);
      for(int i = 0; i < width; i++){
        printf("%3X ", line_buf[j,i]);
      }
    }
    
}


