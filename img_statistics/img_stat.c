#include "img_stat.h"

/* Result map
0 - BLK_SAT
1 - WHT_SAT
2 - Min
3 - Max
4 - Sum
*/

#define use_single_pixel
// less resource utilization without init locals!
#define no_init
component void img_stat( input_image_stream& a,//ihc::stream_in<pxl_v8>&  a, 				// input image stream 
                          hls_avalon_slave_register_argument int height,	// frame size
                          hls_avalon_slave_register_argument int width,	// frame size
                          //hls_avalon_slave_register_argument int npxls,	// low band from 0 to lw_lim  - saturated black
                          //hls_avalon_slave_register_argument pxl_b12 lw_lim,	// low band from 0 to lw_lim  - saturated black
			              //hls_avalon_slave_register_argument pxl_b12 hg_lim,	// high band from 0 to lw_lim - saturated white
			              hls_avalon_slave_register_argument int* result ){
    //static int loc_result[2] hls_init_on_reset;// local var to decrease II and external memory access;
 #ifdef no_init
    static int wht_sat_nm;// = 0; // white saturated
    static int blk_sat_nm;// = 0; // black saturated
    static pxl_b12 frm_min[8];// = {MAX_VAL, MAX_VAL, MAX_VAL, MAX_VAL, MAX_VAL, MAX_VAL, MAX_VAL, MAX_VAL};
    static pxl_b12 frm_max[8];// = {0};
    static pxl_b12 frm_min_global;
    static pxl_b12 frm_max_global;
    static int sum;
 #else
    static int wht_sat_nm = 0; // white saturated
    static int blk_sat_nm = 0; // black saturated
    static pxl_b12 frm_min[8] = {MAX_VAL, MAX_VAL, MAX_VAL, MAX_VAL, MAX_VAL, MAX_VAL, MAX_VAL, MAX_VAL};
    static pxl_b12 frm_max[8] = {0};
    static pxl_b12 frm_min_global = MAX_VAL;
    static pxl_b12 frm_max_global = MIN_VAL;
    static int sum = 0;
 #endif
    // use local to each scope variables (for body) - about 2000 FFs less
    // histogram
    cmpt :
    for(counter j= 0; j < height;++j){
        for(counter i= 0; i < width;i=i+8){
            pxl_v8 a_val; // pixel data
            a_val = a.read();
            #ifndef use_single_pixel
            for (pxl_ind p = 0; p < 8; ++p){
                //wht_sat_nm += (a_val.data[p]  > WHT_SAT) ? 1 : 0;
                //blk_sat_nm += (a_val.data[p]  < BLK_SAT) ? 1 : 0;
                if(a_val.data[p]  > WHT_SAT)
                    wht_sat_nm ++;
                else if(a_val.data[p]  < BLK_SAT)
                    blk_sat_nm ++;

                if(frm_min[p] > a_val.data[p]) // 354
                    frm_min[p] = a_val.data[p];
                if(frm_max[p] < a_val.data[p]) // 256
                    frm_max[p] = a_val.data[p];
                sum = (sum + a_val.data[p]);///2;
            }
            #else // only first pixel 
                pxl_ind p = 0;
                if(a_val.data[p]  > WHT_SAT)
                    wht_sat_nm ++;
                else if(a_val.data[p]  < BLK_SAT)
                    blk_sat_nm ++;

                if(frm_min[p] > a_val.data[p]) // 354
                    frm_min[p] = a_val.data[p];
                if(frm_max[p] < a_val.data[p]) // 256
                    frm_max[p] = a_val.data[p];
                sum = sum + a_val.data[p];
            #endif
        }
    }
  
   
    // find global min and max
    frm_min_global = MAX_VAL;
    frm_max_global = MIN_VAL;
    #ifndef use_single_pixel
        #pragma unroll 1 // ~12000 FFs - we do not need parallelism here
        for(pxl_ind p = 0; p < 8; ++p){
            if(frm_min_global > frm_min[p])
                frm_min_global = frm_min[p];
            if(frm_max_global < frm_max[p])
                frm_max_global = frm_max[p];    
            /*result[2+2*p]     = frm_min[p];
            result[2+2*p+1]   = frm_max[p];*/
        }
    #else
        frm_min_global = frm_min[0];
        frm_max_global = frm_max[0];    
    #endif
    //copy results and reset all locals 
    cp_results:
    result[0] = blk_sat_nm;
    result[1] = wht_sat_nm;
    result[2] = frm_min_global;
    result[3] = frm_max_global;
    result[4] = sum;
#ifdef no_init // reset local vars
    reinit_locals:
    for(pxl_ind p = 0; p < 8; ++p){
        frm_min[p]        = MAX_VAL;
        frm_max[p]        = MIN_VAL;
    }
    blk_sat_nm = 0;
    wht_sat_nm = 0;
    sum   = 0;
#endif
}