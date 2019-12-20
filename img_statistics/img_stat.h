// header file for HLS component
#ifndef IMGSTAT_H
#define IMGSTAT_H

#include "HLS/hls.h"
#include "HLS/ac_int.h"
//
#define MIN_VAL 0
#define MAX_VAL 4095
//#define MAX_VAL 255
//
#define BIN_NMB 16
// 
#define BIN_WDTH MAX_VAL/BIN_NMB
//
#define BIN_ACT 2//BIN_NMB // or 1,2,3 .. BIN_NMB
//
#define WHT_SAT (MAX_VAL - BIN_WDTH)
#define BLK_SAT (0       + BIN_WDTH)

constexpr int clogb2(int value){
  int val = 0;
  int clogb2 = 0;
  val = value - 1;
  for (; val > 0; ++clogb2) {
    val = val >> 1;
  }
  return clogb2+1;
}

// 12 bit pixel
typedef ac_int<clogb2(MAX_VAL), false> pxl_b12;
typedef ac_int <4, false>              pxl_ind;
typedef ac_int <16, false>             counter;
//typedef ac_int<8, false> pxl_b12;
// using 8 pixels, 12-bit each
struct pxl_v8 {
  // Passing 8 elements at a time to the component for processing
  pxl_b12 data[8];
};

typedef ihc::stream_in<pxl_v8> input_image_stream;
//typedef ihc::stream_out<pxl_v8> output_image_stream;



component void img_stat( input_image_stream& a,//ihc::stream_in<pxl_v8>&  a, 				// input image stream 
                          hls_avalon_slave_register_argument int height,	// frame size
                          hls_avalon_slave_register_argument int width,	// frame size
                          //hls_avalon_slave_register_argument int npxls,	// low band from 0 to lw_lim  - saturated black
                          //hls_avalon_slave_register_argument pxl_b12 lw_lim,	// low band from 0 to lw_lim  - saturated black
			                    //hls_avalon_slave_register_argument pxl_b12 hg_lim,	// high band from 0 to lw_lim - saturated white
			                    hls_avalon_slave_register_argument int* result );


#endif // IMGSTAT_H