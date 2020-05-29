#ifndef BAYER_H
#define BAYER_H

#define AVL_ST_W 1
#define MAX_WDTH  AVL_ST_W*5*2
#define MAX_HGHT  5*1

// tpe definitions
const 	ac_int<4, false> N_pxls = AVL_ST_W; // AVL-ST width
typedef ac_int<12, false> pxl_b12;

// using N pixels, 12-bit each
struct pxl_N {
  // Passing N elements at a time to the component for processing
  pxl_b12 data[AVL_ST_W];
};

component void debayer(ihc::stream_in<pxl_N>&  i_str, 				// input image stream
                       hls_avalon_slave_register_argument int width,		// width
                       hls_avalon_slave_register_argument int height,		// heigth
                       ihc::stream_in<pxl_N>&  o_str 				// input image stream
 );

void bf_window_5x5_and_line_buffer(pxl_b12 input_pixel, int width, pxl_b12 window[5][5]);

#endif