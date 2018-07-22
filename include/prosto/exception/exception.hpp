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
 * \file   exception.hpp
 * \author michail peterlis
 * \brief  extended exception class which gives more details what/where it happend.
 * ************************************************************************* */


#ifndef PROSTO_EXCEPTION_HPP
#define PROSTO_EXCEPTION_HPP

#include <exception>
#include <cstdint>
#include <ostream>
#include <utility>

#include <boost/exception/all.hpp>

#include <prosto/environment/debug.hpp>
//#include <prosto/environment/compiler_specialization.hpp>


#ifdef PROSTO_PSEUDO_DEBUG
#  define prosto_error(CODE, ...) \
            prosto::exception(CODE, __VA_ARGS__, \
              prosto::exception::linenumber(__LINE__), \
              prosto::exception::filename(__FILE__), \
              prosto::exception::function(BOOST_CURRENT_FUNCTION))
#else
#  define prosto_error(CODE, ...) \
              prosto::exception(CODE, __VA_ARGS__)
#endif


namespace prosto {

//! \example exception.cpp

/*! \brief extended exception class.
 *
 * from a purly formal point of view, this class is just a container to hold
 * exceptions while they will be thrown. it is derived from std::exception and
 * boost::exception. this shall guarantee conversion and a wide ranged use of
 * the exceptions. this class contains a error code and a errormessage from the
 * scretch. also it is possible to add some other information in boost::exception
 * style. some tags are already defined (as typedefs) and can directly be used.
 *
 * there is a preprocessor define which should be used on the throw side. the
 * preprocessor define has the advantage of the debug-additionals. while you
 * compile in debug mode, some additional information is added to the exception.
 * this information are linenumber, filename and function where the exception
 * was thrown.
 *
 * <h2>short usage</h2>
 * \code
 * // create a class public inheriting from prosto::exception, providing a constructor
 * // with prosto::exception as parameter.
 * class my_exception : public prosto::exception {
 * public:
 *   my_exception(prosto::exception const& e)
 *     : prosto::exception(e) {}
 *  };
 * // ...
 *
 * try {
 *   // throw a error always by PROSTO_error() macro (or the selfmade version(see note below)).
 *   throw(my_exception(PROSTO_error(0x100000, "this is the message")));
 * }
 * catch(std::exception &e) { // note the catched exception is from c++ std libs
 *   // all reading-operations should be done inside an if.
 *   // the pointer will be nulled if the reading was unsuccessful.
 *   if(auto ec = prosto::exception::info<prosto::exception::code>(e))
 *     std::cerr << "error" << *ec << std::endl;
 * }
 * \endcode
 * for a more detailed example see \ref exception.cpp. especially \b case \b 6.
 *
 * \pre
 * this class is written in c++11 standard.
 *
 * \pre
 * please read first how std::exceptions and boost::exceptions work, to understand
 * how this exception is working. it is possible to append iformation the boost
 * way. however, using prosto::exception it is more common to use the PROSTO_error
 * which still give the possibility to use it the boost way or the prosto way, which
 * is by parameter.
 *
 * \note
 * why is prosto::exception a define and not a constexpr? because __LINE__,
 * and __FILE__ isn't working in a contexpr.
 *
 * \note
 * avoid double catches. double catches will result in hardly to find errors
 * in exception handling, while the code is still compiling.
 * since prosto::exception is derived from std::exception, prosto::exception is
 * down-castable, but std::excption is not up-castable. also, since the exception
 * handler is up to the compiler and handling is usually done by "first come first
 * served" the first handler will be executed which could handle the exception.
 * this means:
 * \code
 * throw(prosto::exception)
 * catch(std::exception)   // will catch both std and prosto
 * catch(prosto::exception)  // will never be reached
 * \endcode
 * or:
 * \code
 * throw(prosto::exception)
 * catch(prosto::exception)  // will catch prosto
 * catch(std::exception)   // will catch std
 * \endcode
 * this behaviuor is best avoided by simply catch just std::exceptions. catching
 * only prosto::exceptions will never catch std::exceptions, so this also should be avoided!
 *
 * \note if you plan to define a different prosto, the macro PROSTO_error also can be replaced.
 * defining a new macro like:
 * \code
 * #define my_error(CODE, ...) PROSTO_error(CODE, __VA_ARGS__)
 * \endcode
 * will make it conform to the own api.
 *
 * \exception noexcept no exception will be thrown.
 * \exception all the functionpointer tags (handle) can throw a exception depending on the given function.
 */
class exception
  : public std::exception
  , public boost::exception {

public:

  typedef void(printf_type)(std::exception const& e, std::ostream& os, unsigned int rec);


  //! template like typedef. this is used for all typedefs.
  template<typename tagT, typename typeT>
  using info_type  = boost::error_info<tagT, typeT>;

#if defined( _MSC_VER )/* && (_MSC_VER < insert here version which will support)*/
  //! workaround for msvc leakin...
  //! nested exception.
  using nested = info_type<struct exception_nested_tag, std::exception_ptr>;
#endif

  //! contains an error code.
  using code       = info_type<struct tag_exception_code, unsigned int>;

  //! contains the message.
  using message    = info_type<struct tag_exception_message, char const*>;

#ifdef PROSTO_PSEUDO_DEBUG
  //! contains the filename of the thrown exception.
  using filename   = info_type<struct tag_exception_filename, char const*>;

  //! contains the linenumber in the file of the thrown exception.
  using linenumber = info_type<struct tag_exception_linenumber, int>;

  //! contains the function-name of the thrown exception.
  using function   = info_type<struct tag_exception_function, char const*>;
#endif

  //! can store a usual handler for this kind of exceptions.
  template<typename FN>
  using handle     = info_type<struct tag_exception_handle, std::function<FN> >;


  /*! \brief creates an exception with a code, message and some additional information.
   *
   * \warning dont use the constructor directly, use the the \b prosto_error macro.
   *
   * the constructor is creating an instance of prosto::exception. it requiers a
   * code and a message.
   */
  template<typename... T>
  explicit exception(unsigned int c, char const* m, T&&... rest) {
    *this << code(c);
    *this << message(m);
    add(std::forward<T>(rest)...);
  }
  

  /*! \brief non-trivial destructor.
   *
   * the destructor is non-trivial since it is exception specified (like the
   * exception it inherits from) so it could be used in a constexpr.
   */
  virtual ~exception() noexcept {}


  /*! \brief for std::exception catches. returns the message.
   *
   * this is for the standard exception what call.
   *
   * \note if the exception is not casted to prosto::exception, this function returns
   * the name of the exception (e.g. \b std::exception).
   */
  virtual char const* what() const noexcept {
    return *info<message>(*this);
  }

  /*! \brief returns the info of the exception.
   *
   * this fonction is used to get the information of the exception. it returns
   * a const variant of the information.
   */
  template<typename tag_T>
  static inline typename tag_T::value_type const*
  info(std::exception const& e) {
    return boost::get_error_info<tag_T>(e);
  }


protected:

  /*! \brief adds all given parameter to the exception.
   *
   * using this recursive template function, it is possible to enable variadic
   * template parameter on the constructor. the constructor is calling this
   * method, which is adding all parameter to the exception.
   */
  template<typename T, typename... R>
  void add(T t, R... r) {
    *this << t;
    if(sizeof...(r))
      add(r...);
  }

  /*! \brief the end of the recursiv template.
   *
   * since the variadic template add is recursive, there have to be an end.
   * because of if(sizeof..(r)) the end will never be called but is neccessary
   * to compile.
   */
  void add() {}
};


}  // prosto

#endif // PROSTO_EXCEPTION_HPP
