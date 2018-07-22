#include <iostream>

#include "all.hpp"


int main() {
  throw_std_catch_std_first();
  throw_std_catch_prosto_first();
  throw_prosto_catch_std_first();
  throw_prosto_catch_prosto_first();
    
  throw_prosto_exception();
  throw_prosto_error();
  throw_custom_prosto_exception();
  throw_custom_prosto_error();
  throw_nested_test();
  
  std::cin.ignore();
  return 0;
}
