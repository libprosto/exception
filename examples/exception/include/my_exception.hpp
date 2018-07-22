#ifndef MY_EXCEPTION_HPP
#define MY_EXCEPTION_HPP


#include <prosto/exception_all.hpp>


class my_exception : public prosto::exception {
public:
  using my_type = prosto::exception::info_type<struct my_exception_tag, float>;

  my_exception(prosto::exception const& e)
    : prosto::exception(e) { *this << handle<printf_type>(printf); }

  static void printf(std::exception const& e, std::ostream& os, unsigned int rec) {
    auto pt = [&rec, &os]() { for (unsigned int i = 0; i<rec; i++) os << "\t"; };

    if(auto eh=prosto::exception::info<my_type>(e)) {
      pt();
      os << "additional\t:\t" << *eh << std::endl;
    }
  }
};

#endif // MY_EXCEPTION_HPP
