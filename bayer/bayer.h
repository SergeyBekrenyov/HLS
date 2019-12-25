#define AVL_ST_W 1
#define MaxWdth  AVL_ST_W*5
#define MaxHght  5*1

// tpe definitions
const 	ac_int<4, false> N_pxls = AVL_ST_W; // AVL-ST width
typedef ac_int<12, false> pxl_b12;

// using N pixels, 12-bit each
struct pxl_N {
  // Passing N elements at a time to the component for processing
  pxl_b12 data[AVL_ST_W];
};




// test array
pxl_N test_img[MaxWdth][MaxHght] = {
                            0x000, 0x001, 0x002, 0x003, 0x004,/* 0x005, 0x006, 0x007,
                            0x010, 0x011, 0x012, 0x013, 0x014, 0x015, 0x016, 0x017,
                            0x020, 0x021, 0x022, 0x023, 0x024, 0x025, 0x026, 0x027,*/
                            0x100, 0x101, 0x102, 0x103, 0x104,
                            0x200, 0x201, 0x202, 0x203, 0x204,
                            0x300, 0x301, 0x302, 0x303, 0x304,
                            0x400, 0x401, 0x402, 0x403, 0x404};

component void debayer(ihc::stream_in<pxl_N>&  i_str, 				// input image stream
                       hls_avalon_slave_register_argument int width,		// width
                       hls_avalon_slave_register_argument int height,		// heigth
                       ihc::stream_in<pxl_N>&  o_str 				// input image stream
 );

void update_line_buffers(int width, int height, pxl_N i_pxls, pxl_N line_buf[]);
