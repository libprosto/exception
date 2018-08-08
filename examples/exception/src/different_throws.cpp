#ifndef TEST_DIFFERENT_THROWS_HPP
#define TEST_DIFFERENT_THROWS_HPP


#include <iostream>
#include <sstream>

#include "my_exception.hpp"


bool throw_prosto_exception() {
  try {
    throw(prosto::exception(0x1, "throw prosto exception"));
  }
  catch(std::exception const& e) {
    using namespace prosto;
    std::cerr << e << std::endl;
    return true;
  }

  return false;
}


bool throw_prosto_error() {
  try {
    throw(prosto_error(0x2, "throw prosto error"));
  }
  catch(std::exception const& e) {
    using namespace prosto;
    std::cerr << e << std::endl;
    return true;
  }
  
  return false;
}


bool throw_custom_prosto_exception() {
  try {
    throw(my_exception(prosto::exception(0x3
                                        ,"throw custom prosto exception"
                                        ,my_exception::my_type(12.345))));
  }
  catch(std::exception const& e) {
    using namespace prosto;
    std::cerr << e << std::endl;
    return true;
  }
  
  return false;
}


bool throw_custom_prosto_error() {
  try {
    throw(my_exception(prosto_error(0x4
                                   ,"throw custom prosto error"
                                   ,my_exception::my_type(9999999))));
  }
  catch(std::exception const& e) {
    using namespace prosto;
    std::cerr << e << std::endl;
    return true;
  }

  return false;
}


bool throw_nested_test() {
  using namespace prosto;
  
  try {
    try {
      try {
        try {
          throw(std::runtime_error("std::exception 1"));
        }
        catch(std::exception const& e) {
          std::throw_with_nested(prosto_error(0x05, "my_execption   2"));
        }
      }
      catch(std::exception const& e) {
        std::throw_with_nested(std::runtime_error("std::exception 3"));
      }  
    }
    catch(std::exception const& e) {
      std::throw_with_nested(prosto_error(0x06
                                         ,"my_execption   4"
                                         ,my_exception::my_type(4)));
    }
  }
  catch(std::exception const& e) {
    std::cerr << "caugth a bunch of exceptions\n" << e << std::endl;
    return true;
  }
  
  return false;
}


bool throw_prosto_exception_with_stdstring() {
  try {
    throw(prosto_error(0x7, std::string("throw prosto exception with std::string")));
  }
  catch(std::exception const& e) {
    using namespace prosto;
    std::cerr << e << std::endl;
    return true;
  }

  return false;
}


bool throw_prosto_exception_without_code() {
  try {
    throw(prosto_error("throw prosto exception without code"));
  }
  catch(std::exception const& e) {
    using namespace prosto;
    std::cerr << e << std::endl;
    return true;
  }

  return false;
}


bool throw_prosto_exception_into_stringstream() {
  try {
    throw(prosto_error(0x08, "throw prosto exception into stringstream"));
  }
  catch(std::exception const& e) {
    using namespace prosto;
    std::stringstream ss;
    ss << e;
    std::cerr << ss.str();
    return true;
  }

  return false;
}

#endif // TEST_DIFFERENT_THROWS_HPP
