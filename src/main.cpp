#include "cansend.hpp"
#include <iostream>

int main()
{
  // initial CAN model 
  int can_initial_value;
  // can_init(can_initial_value);

  while(true) {
    // int arr[15] = {1};
    // cansend(can_initial_value, arr);
    canread(can_initial_value);
  }
  close(can_initial_value);

  return 0;
}

