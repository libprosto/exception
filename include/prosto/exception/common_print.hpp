/* ************************************************************************* *\
 * This file is part of prosto-lib.                                          *
 *                                                                           *
 * This library is free software; you can redistribute it and/or modify it   *
 * under the terms of the GNU Lesser General Public License as published by  *
 * the Free Software Foundation; either version 2.1 of the License.          *
 *                                                                           *
 * This library is distributed in the hope that it will be useful, but       *
 * WITHOUT ANY WARRANTY; without even the implied warranty of                *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser   *
 * General Public License for more details.                                  *
 *                                                                           *
 * You should have received a copy of the GNU Lesser General Public License  *
 * along with this library (see the file LICENCE); If not, see               *
 * <http://www.gnu.org/licenses/old-licenses/lgpl-2.1.en.html>.              *
\* ************************************************************************* */

/*!
 * \file   common_print.hpp
 * \author michail peterlis
 * \brief  streaming overload for prosto::exception printing.
 * ************************************************************************* */


#ifndef PROSTO_EXCEPTION_COMMON_PRINT_HPP
#define PROSTO_EXCEPTION_COMMON_PRINT_HPP

#include <ostream>

#include "exception.hpp"


namespace prosto  {
namespace detail_ {


/// \todo describe this function.
template<typename ostream_T>
void error_printer(std::exception const& e, ostream_T& os, unsigned int rec) {
  std::string pt(rec, '\t');

#ifdef PROSTO_PSEUDO_DEBUG
  os << pt << "type\t\t:\t" << typeid(e).name() << "\n";
#endif
  
  if(auto eh = exception::info<prosto::exception::code>(e))
    os << pt << "code\t\t:\t0x" << std::hex << std::uppercase << *eh << std::dec << std::nouppercase << "\n";

  if(auto eh = exception::info<exception::message>(e))
    os << pt << "message\t\t:\t" << *eh << "\n";
  else
    os << pt << "what\t\t:\t" << e.what() << "\n";

  // since programming relevant information is only added in with (at least pseudo)
  // debugmode, there is no need print it out.
#ifdef PROSTO_PSEUDO_DEBUG
  if(auto eh = exception::info<exception::filename>(e))
    os << pt << "filename\t:\t" << *eh << "\n";

  if(auto eh = exception::info<exception::linenumber>(e))
    os << pt << "linenumber\t:\t" << *eh << "\n";

  if(auto eh = exception::info<exception::function>(e))
    os << pt << "fuction\t\t:\t" << *eh << "\n";
#endif

  if(auto eh = exception::info<exception::handle<exception::printf_type>>(e))
    (*eh)(e, os, rec);

  try{ std::rethrow_if_nested(e); }
  catch(std::exception const& n) {
    os << pt << "with nested error\t:\n";
    error_printer(n, os, ++rec);
  }
}

} // namespace detail


/// \todo Make the template streamable for all kinds of stream and \b test it.
/// \todo Make streamable with const exception.
template<typename ostream_T = std::ostream>
ostream_T& operator<<(ostream_T& os, std::exception const& e) {
  detail_::error_printer(e, os, 0);
  return os;
}

} // namespace prosto

#endif // PROSTO_EXCEPTION_COMMON_PRINT_HPP
