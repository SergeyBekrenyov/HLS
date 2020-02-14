#include "find_leading_ones.h"

bool find_leading_ones(ac_int<NUM_BITS,false> din,
                       ac_int<LOG2_CEIL<NUM_BITS>::val,0> &dout){
  int tmp;
  bool flag = false;
  for(int i=NUM_BITS-1;i>=0;i--){
    if(din[i]){
    flag = true;
    tmp = i;
    break;
    }
  }
  dout = tmp;
  return flag;
}

int main(void) {

  

  return 0;
}
