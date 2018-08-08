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
 * \brief  Extended exception class which gives more details about what happend where.
 * ************************************************************************* */


#ifndef PROSTO_EXCEPTION_HPP
#define PROSTO_EXCEPTION_HPP

#include <exception>
#include <cstdint>
#include <ostream>
#include <utility>
#include <functional>
#include <string>

#include <boost/exception/all.hpp>

#if __cplusplus >= 201703L && __has_include("../environment/debug.hpp")
#  include "../environment/debug.hpp"
#elif __has_include(<prosto/environment/debug.hpp>)
#  include <prosto/environment/debug.hpp>
#endif

#ifdef PROSTO_PSEUDO_DEBUG
#  define prosto_error(...) \
            prosto::exception(__VA_ARGS__, \
              prosto::exception::linenumber(__LINE__), \
              prosto::exception::filename(__FILE__), \
              prosto::exception::function(BOOST_CURRENT_FUNCTION))
#else
#  define prosto_error(...) \
            prosto::exception(__VA_ARGS__)
#endif


namespace prosto {

//! \example exception.cpp

/*! \brief Extended exception class.
 *
 * From a purly formal point of view, this class is just a container to hold
 * exceptions while they are thrown. It derives from std::exception and
 * boost::exception. This shall guarantee conversion and a wide ranged use of
 * the exceptions. This class contains an error code and a message from
 * scratch. Also it is possible to add some other information in boost::exception
 * style. Some tags are already defined (as typedefs) and can be used directly.
 *
 * There is a preprocessor define which should be used on the throw side. The
 * preprocessor define has the advantage of the debug-additionals. While
 * compiling in debug mode, some additional information is added to the exception.
 * This information are linenumber, filename and function where the exception
 * was thrown.
 *
 * <h2>Short usage</h2>
 * \code
 * // Create a class public inheriting from prosto::exception, providing a constructor
 * // with prosto::exception as parameter.
 * class my_exception : public prosto::exception {
 * public:
 *   my_exception(prosto::exception const& e)
 *     : prosto::exception(e) {}
 *  };
 * // ...
 *
 * try {
 *   // Throw an error always by prosto_error() macro (or the selfmade version(see note below)).
 *   throw(my_exception(prosto_error(0x100000, "my error")));
 * }
 * catch(std::exception &e) { // note the catched exception is std::exception
 *   // All reading-operations should be done inside an if.
 *   // The pointer will be nulled if the reading was unsuccessful.
 *   if(auto ec = prosto::exception::info<prosto::exception::code>(e))
 *     std::cerr << "error" << *ec << std::endl;
 * }
 * \endcode
 * for a more detailed example see \ref different_throws.cpp especially
 * \b throw_custom_prosto_exception \b 6.
 *
 * \pre
 * For addition information linenumber, file and function either define
 * \def PROSTO_PSEUDO_DEBUG or include prosto/environment/debug.hpp before including
 * exception.hpp. In case of C++17 it is auto include if provided.
 *
 * \note
 * for more details on how exceptions work, please consider reading how
 * std::exceptions and boost::exceptions work. It is possible to append iformation the boost
 * way. However, with prosto::exception it is more common to use the prosto_error
 * which still give the possibility to use it the boost way or the prosto way, which
 * is by parameter.
 *
 * \note
 * Why is prosto_error a define and not a constexpr? because __LINE__,
 * and __FILE__ isn't working in a contexpr.
 *
 * \note
 * Avoid double catches. Double catches will result in hardly to find unexpected behavior
 * in exception handling.
 * Since prosto::exception is derived from std::exception, prosto::exception is
 * down-castable, but std::excption is not up-castable. Also, since the exception
 * handler is up to the compiler and handling is usually done by "first come first
 * served" the first handler will be executed which could handle the exception.
 * This means:
 * \code
 * throw(prosto::exception)
 * catch(std::exception)     // will catch both std and prosto
 * catch(prosto::exception)  // should never be reached
 * \endcode
 * or:
 * \code
 * throw(prosto::exception)
 * catch(prosto::exception)  // will catch prosto
 * catch(std::exception)     // will catch std
 * \endcode
 * This behaviuor is best avoided by simply catch just std::exceptions. Catching
 * only prosto::exceptions will never catch std::exceptions, so this also should be avoided.
 *
 * \exception noexcept No exception will be thrown.
 * \exception all The functionpointer tags (handle) can throw a exception depending on the given function.
 */
class exception
  : public std::exception
  , public boost::exception {

public:

  typedef void(printf_type)(std::exception const& e, std::ostream& os, unsigned int rec);


  //! Template like typedef. This is used for all typedefs.
  template<typename tagT, typename typeT>
  using info_type  = boost::error_info<tagT, typeT>;

  //! Contains an error code.
  using code       = info_type<struct tag_exception_code, unsigned int>;

  //! Contains the message.
  using message    = info_type<struct tag_exception_message, std::string>;

#ifdef PROSTO_PSEUDO_DEBUG
  //! Contains the filename of the thrown exception.
  using filename   = info_type<struct tag_exception_filename, char const*>;

  //! Contains the linenumber in the file of the thrown exception.
  using linenumber = info_type<struct tag_exception_linenumber, int>;

  //! Contains the function-name of the thrown exception.
  using function   = info_type<struct tag_exception_function, char const*>;
#endif

  //! Can store an user defined handler for this kind of exceptions.
  template<typename FN>
  using handle     = info_type<struct tag_exception_handle, std::function<FN> >;


  /*! \brief Creates an exception with a code, message and some additional information.
   *
   * \warning Avoid using the constructor directly, use the \b prosto_error macro instead.
   *
   * The constructor is creating an instance of prosto::exception with a
   * code and a message.
   */
  template<typename... T>
  explicit exception(unsigned int c, std::string const& m, T&&... rest) {
    *this << code(c);
    *this << message(m);
    add(std::forward<T>(rest)...);
  }

  //! \brief Overload with no code.
  template<typename... T>
  explicit exception(std::string const& m, T&&... rest) {
    *this << message(m);
    add(std::forward<T>(rest)...);
  }

  /*! \brief Non-trivial destructor.
   *
   * The destructor is non-trivial since it is exception specified (like the
   * exception it inherits from) so it could be used in a constexpr.
   */
  virtual ~exception() noexcept {}


  /*! \brief For std::exception catches; Returns the message.
   *
   * This is for the standard exception function \i what.
   */
  virtual char const* what() const noexcept {
    return info<message>(*this)->c_str();
  }

  /*! \brief Returns the info of the exception.
   *
   * This function is used to get the information of the exception. It returns
   * a const variant of the information.
   */
  template<typename tag_T>
  static inline typename tag_T::value_type const*
  info(std::exception const& e) {
    return boost::get_error_info<tag_T>(e);
  }


protected:

  /*! \brief Adds all given parameter to the exception.
   *
   * Using this recursive template function, it is possible to enable variadic
   * template parameter on the constructor. The constructor is calling this
   * method, which is adding all parameter to the exception.
   */
  template<typename T, typename... R>
  void add(T t, R... r) {
    *this << t;
    add(r...);
  }

  /*! \brief End of the recursiv template.
   *
   * Since the variadic template add is recursive, there have to be an end.
   */
  void add() {}
};


}  // namespace prosto

#endif // PROSTO_EXCEPTION_HPP
