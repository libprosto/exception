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
  throw_prosto_exception_with_stdstring();
  throw_prosto_exception_without_code();
  throw_prosto_exception_into_stringstream();
  
  std::cin.ignore();
  return 0;
}
