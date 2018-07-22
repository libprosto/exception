#ifndef TEST_CUSTOM_EXCEPTION_HPP
#define TEST_CUSTOM_EXCEPTION_HPP


#include <prosto/exception_all.hpp>


bool throw_std_catch_std_first() {
  try {
    throw(std::runtime_error("std::runtime_error"));
  }
  catch(std::exception const& e) {
    using namespace prosto;
    std::cerr << e << std::endl;
    return true;
  }
  catch(prosto::exception const& e) {
    // should never happen
    // should be caught by catch(std::exception) above
    using namespace prosto;
    std::cerr << e << std::endl;
    return false;
  }
  
  return false;
}


bool throw_std_catch_prosto_first() {
  try {
    throw(std::runtime_error("std::runtime_error"));
  }
  catch(prosto::exception const& e) {
    // should nether happen
    // should be caught by catch(std::exception) below
    using namespace prosto;
    std::cerr << e << std::endl;
    return false;
  }
  catch(std::exception const& e) {
    using namespace prosto;
    std::cerr << e << std::endl;
    return true;
  }
  
  return false;
}


bool throw_prosto_catch_std_first() {
  try {
    throw(prosto_error(0x1, "prosto::exception"));
  }
  catch(std::exception const& e) {
    using namespace prosto;
    std::cerr << e << std::endl;
    return true;
  }
  catch(prosto::exception const& e) {
    // should never happen
    // should be caught by catch(std::exception) above
    using namespace prosto;
    std::cerr << e << std::endl;
    return false;
  }
  
  return false;
}


bool throw_prosto_catch_prosto_first() {
  try {
    throw(prosto_error(0x1, "prosto::exception"));
  }
  catch(prosto::exception const& e) {
    using namespace prosto;
    std::cerr << e << std::endl;
    return true;
  }
  catch(std::exception const& e) {
    // should never happen
    // std::exception should not be thrown
    using namespace prosto;
    std::cerr << e << std::endl;
    return false;
  }
  
  return false;
}

#endif // TEST_CUSTOM_EXCEPTION_HPP
