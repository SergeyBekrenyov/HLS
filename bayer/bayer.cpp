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
/*void sf_window_3x3_and_line_buffer(unsigned char input_pixel,
                                   unsigned char window[3][3]) {

    // shift registers
    static unsigned prev_row_index = 0;
    static unsigned char prev_row1[WIDTH] = {0};
    static unsigned char prev_row2[WIDTH] = {0};

    // window buffer:
    //      window[0][0], window[0][1], window[0][2]
    //      window[1][0], window[1][1], window[1][2]
    //      window[2][0], window[2][1], window[2][2]

    // shift existing window to the left by one
    window[0][0] = window[0][1];
    window[0][1] = window[0][2];
    window[1][0] = window[1][1];
    window[1][1] = window[1][2];
    window[2][0] = window[2][1];
    window[2][1] = window[2][2];

    int prev_row_elem1 = prev_row1[prev_row_index];
    int prev_row_elem2 = prev_row2[prev_row_index];

    // grab next column (the rightmost column of the sliding window)
    window[0][2] = prev_row_elem2;
    window[1][2] = prev_row_elem1;
    window[2][2] = input_pixel;

    prev_row1[prev_row_index] = input_pixel;
    prev_row2[prev_row_index] = prev_row_elem1;

    prev_row_index = (prev_row_index == WIDTH - 1) ? 0 : prev_row_index + 1;
}*/

void bf_window_5x5_and_line_buffer(pxl_b12 input_pixel, int width,
                                   pxl_b12 window[5][5]
								   ){
	// shift registers
    static unsigned prev_row_index = 0;
    static pxl_b12 prev_row1[MAX_WDTH] = {0};
    static pxl_b12 prev_row2[MAX_WDTH] = {0};
	static pxl_b12 prev_row3[MAX_WDTH] = {0};
	static pxl_b12 prev_row4[MAX_WDTH] = {0};
	
	pxl_b12 prev_row_elem1;
	pxl_b12 prev_row_elem2;
	pxl_b12 prev_row_elem3;
	pxl_b12 prev_row_elem4;
	

    // window buffer:
    //      window[0][0], window[0][1], window[0][2],  window[0][3],  window[0][4], 
    //      window[1][0], window[1][1], window[1][2],  window[1][3],  window[1][4]
    //      window[2][0], window[2][1], window[2][2],  window[2][3],  window[2][4]
	//      window[3][0], window[3][1], window[3][2],  window[3][3],  window[3][4]
	//      window[4][0], window[4][1], window[4][2],  window[4][3],  window[4][4]

    //shift existing window to the left by one
    // window[0][0] = window[0][1];
    // window[0][1] = window[0][2];
	
    // window[1][0] = window[1][1];
    // window[1][1] = window[1][2];
	
    // window[2][0] = window[2][1];
    // window[2][1] = window[2][2];

	outer_wnd_sr : for (int j = 0; j < 4; ++j){
		inner_wnd_sr : for (int k = 0; k < 4; ++k){
			window[j][k] = window[j][k+1];
		}
	}
		
// for
    prev_row_elem1 = prev_row1[prev_row_index];
    prev_row_elem2 = prev_row2[prev_row_index];
	prev_row_elem3 = prev_row3[prev_row_index];
	prev_row_elem4 = prev_row4[prev_row_index];
	
// for
    // grab next column (the rightmost column of the sliding window)
    window[0][4] = prev_row_elem4;
	window[1][4] = prev_row_elem3;
	window[2][4] = prev_row_elem2;
    window[3][4] = prev_row_elem1;
    window[4][4] = input_pixel;

    prev_row1[prev_row_index] = input_pixel;
    prev_row2[prev_row_index] = prev_row_elem1;
	prev_row3[prev_row_index] = prev_row_elem2;
	prev_row4[prev_row_index] = prev_row_elem3;

    prev_row_index = (prev_row_index == width - 1) ? 0 : prev_row_index + 1;
}

/*void update_line_buffers(int width, int height, pxl_b12 i_pxls, pxl_N line_buf[5][MAX_WDTH]){
    static int pxl_nmb; 
    //update line buffers
    /*for(int j = 0; j < height; j++){
      for(int i = 0; i < width; i+=N_pxls){/
        pxl_N i_str_val; // input stream pixel data
        i_str_val.data[0] = i_pxls;
        //printf("Bayer:  0x%3X", (int) i_pxls);
        //printf(" 0x%3X", (int) i_str_val.data[0]);
        line_buf[4][pxl_nmb] = line_buf[3][pxl_nmb];
        line_buf[3][pxl_nmb] = line_buf[2][pxl_nmb];
        line_buf[2][pxl_nmb] = line_buf[1][pxl_nmb];
        line_buf[1][pxl_nmb] = line_buf[0][pxl_nmb];
        line_buf[0][pxl_nmb] = i_str_val;//[j];
        if(pxl_nmb < (width - 1))
          pxl_nmb++;
        else 
          pxl_nmb = 0;
    //  }
    //}
    
    for (int j = 0; j < 5; ++j){
      printf("\nLine #%d : ", j);
      for(int i = 0; i < width; i++){
        printf("0x%3X ", (int) line_buf[j][i].data[0]);
      }
    }
    
}

void update_window_buffer(int width, int height, pxl_N line[5], pxl_N window_buf[5][5]){
    static int pxl_nmb; 
    //update window buffers
    for(int pxl_nmb = 0; pxl_nmb < 4; ++pxl_nmb){
      window_buf[4][pxl_nmb] = line_buf[3][pxl_nmb];
        line_buf[3][pxl_nmb] = line_buf[2][pxl_nmb];
        line_buf[2][pxl_nmb] = line_buf[1][pxl_nmb];
        line_buf[1][pxl_nmb] = line_buf[0][pxl_nmb];
        line_buf[0][pxl_nmb] = i_str_val;//[j];
        if(pxl_nmb < (5 - 1))
          pxl_nmb++;
        else 
          pxl_nmb = 0;
    //  }
    //}
    /*for(int j = 0; j < height; j++){
      for(int i = 0; i < width; i+=N_pxls){/
        pxl_N i_str_val; // input stream pixel data
        i_str_val.data[0] = i_pxls;
        //printf("Bayer:  0x%3X", (int) i_pxls);
        //printf(" 0x%3X", (int) i_str_val.data[0]);
        line_buf[4][pxl_nmb] = line_buf[3][pxl_nmb];
        line_buf[3][pxl_nmb] = line_buf[2][pxl_nmb];
        line_buf[2][pxl_nmb] = line_buf[1][pxl_nmb];
        line_buf[1][pxl_nmb] = line_buf[0][pxl_nmb];
        line_buf[0][pxl_nmb] = i_str_val;//[j];
        if(pxl_nmb < (5 - 1))
          pxl_nmb++;
        else 
          pxl_nmb = 0;
    //  }
    //}
    
    for (int j = 0; j < 5; ++j){
      printf("\nLine #%d : ", j);
      for(int i = 0; i < width; i++){
        printf("0x%3X ", (int) line_buf[j][i].data[0]);
      }
    }
    
}*/
