/**
 * @file common.hpp
 * @ingroup rcppsw common
 *
 * @copyright 2017 John Harwell, All rights reserved.
 *
 * This file is part of RCPPSW.
 *
 * RCPPSW is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * RCPPSW is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * RCPPSW.  If not, see <http://www.gnu.org/licenses/
 */

#ifndef INCLUDE_RCPPSW_COMMON_COMMON_HPP_
#define INCLUDE_RCPPSW_COMMON_COMMON_HPP_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <memory>
#include <string>
#include <tuple>
#include <utility>

#include "rcsw/common/common.h"

/*******************************************************************************
 * Macros
 ******************************************************************************/
/**
 * @internal
 *
 * @def NS_START(ns)
 *
 * Callback macro for recursive macro expansion for nested namespace start
 * pasting. \c ns is the namespace to paste.
 */
#define NS_START_(ns) namespace ns {
/**
 * @def NS_START(ns)
 *
 * Callback macro for recursive macro expansion for nested namespace end
 * pasting. \c ns is the namespace to paste. Note that it is not strictly
 * necessary to pass the namespace name to this macro, as it only pastes a
 * single bracket; it is more self documenting to do so, however.
 *
 * @endinternal
 */
#define NS_END_(ns) }

/**
 * @def NS_START(...)
 *
 * Declare a nested namespace, with each successive token in the comma-separated
 * argument list being declared inside the namespace of the previous token.
 */
#define NS_START(...) XFOR_EACH1(NS_START_, __VA_ARGS__)

/**
 * @def NS_END(...)
 *
 * Close a previously declared nested namespace. Tokens should be passed to this
 * macro in the reverse order they were declared in (not actually necessary for
 * valid code, but it is more self documentating to do so).
 */
#define NS_END(...) XFOR_EACH1(NS_END_, __VA_ARGS__)

/**
 * @def RCPPSW_DECLDEF_WRAP(Func, member,...)
 *
 * Wrap a public function from a member variable (or even another member
 * function that returns an object that contains the function you want to
 * wrap). The variable argument list is to allow specification of 'const' as
 * part of the function definition. Wrapped functions can take any number of
 * arguments of any type.
 *
 * Cannot be used to wrap overriden functions.
 */
#define RCPPSW_DECLDEF_WRAP(Func, Member, ...)                                \
  template <typename... Args>                                                 \
  auto Func(Args&&... args)                                                   \
      __VA_ARGS__->decltype(std::declval<decltype(Member)>().Func(args...)) { \
    return Member.Func(std::forward<Args>(args)...);                          \
  }

/**
 * @def RCPPSW_DECLDEF_OVERRIDE(Func, member,...)
 *
 * Wrap a public function from a member variable (or even another member
 * function that returns an object that contains the function you want to
 * wrap). The variable argument list is to allow specification of 'const' as
 * part of the function definition. Wrapped functions must take 0 arguments
 * (can't do anything other than that for a general macro that works with
 * virtual functions).
 *
 * Should be used to wrap overriden functions in the current class (e.g the
 * enclosing class implements the same interface as a member variable).
 */

#define RCPPSW_DECLDEF_OVERRIDE_WRAP(Func, Member, ...)                         \
  auto Func(void)                                                               \
      __VA_ARGS__->decltype(std::declval<decltype(Member)>().Func()) override { \
    return Member.Func();                                                       \
  }

#define RCPPSW_WRAP_DECL(Ret, Func, ...) Ret Func(void) __VA_ARGS__ h

#define RCPPSW_WRAP_DEF(Class, Func, Handle, ...)                      \
  auto Class::Func(void)                                               \
      __VA_ARGS__->decltype(std::declval<decltype(Handle)>().Func()) { \
    return (Handle).Func();                                            \
  }

#define RCPPSW_WRAP_DEFP(Class, Func, Handle, NullRet, ...)             \
  auto Class::Func(void)                                                \
      __VA_ARGS__->decltype(std::declval<decltype(*Handle)>().Func()) { \
    if (nullptr == Handle) {                                            \
      return NullRet;                                                   \
    }                                                                   \
    return (Handle)->Func();                                            \
  }

/**
 * @def RCPPSW_WRAP_OVERRIDE_DECL(Ret, Func, ...)
 *
 * Declare a "simple" overrnide of an inherited function with the __pure_
 * attribute. Should be *NOT* be used if the override is complex to implement
 * (i.e. for every instance of this macro in a header file there should be an
 * instance of \ref RCPPSW_WRAP_OVERRIDE_DEF() in the corresponding source file
 * for a class).
 */
#define RCPPSW_WRAP_OVERRIDE_DECL(Ret, Func, ...) \
  Ret Func(void) __VA_ARGS__ override __rcsw_pure

/**
 * @def RCPPSW_WRAP_OVERRIDE_DEF(Ret, Func, ...)
 *
 * Define a "simple" override of an inherited function with the __pure__
 * attribute in which the corresponding function on the handle is called and the
 * result returned.
 */
#define RCPPSW_WRAP_OVERRIDE_DEF(Class, Func, Handle, ...) \
  __rcsw_pure RCPPSW_WRAP_DEF(Class, Func, Handle, __VA_ARGS__)

#define RCPPSW_WRAP_OVERRIDE_DEFP(Class, Func, Handle, NullRet, ...) \
  __rcsw_pure RCPPSW_WRAP_DEFP(Class, Func, Handle, NullRet, __VA_ARGS__)

/**
 * @def RCPPSW_SFINAE_FUNC(...)
 *
 * Specify the condition to enable a function for SFINAE.
 *
 * It turns out that in C++14, a default template type parameter (e.g. typename
 * = std::enable_if<...>::value)is NOT considered part of a function's signature
 * for the purposes of SFINAE, so functions with 100% identical signatures
 * except for such a parameter will cause a compilation error.
 *
 * However, a non-template type parameter (e.g. a template parameter that is an
 * integer) that is defaulted (i.e. have a default value specified) ARE
 * considered part of a function's signature for the purposes of SFINAE, so two
 * functions that differ only in the value of the defaulted non-type parameter
 * in their template argument lists will be considered distinct and trigger
 * SFINAE as expected.
 */
#define RCPPSW_SFINAE_FUNC(...)                         \
  typename std::enable_if<__VA_ARGS__, int>::type = 0

#define RCPPSW_SFINAE_TYPE(...)                         \
  typename std::enable_if<__VA_ARGS__, bool>::type

/*******************************************************************************
 * Namespaces/Decls
 ******************************************************************************/
NS_START(rcppsw);

/*******************************************************************************
 * Debugging Templates
 ******************************************************************************/
#if __cplusplus < 201703L
template <class...>
using void_t = void;
#else
template <typename... Args>
using void_t = std::void_t<Args...>;
#endif

NS_START(detail);
template <class unused, class = void_t<>>
struct has_to_str : std::false_type {};

template <class T>
struct has_to_str<T, void_t<decltype(std::declval<T>().to_str())>>
    : std::true_type {};
NS_END(detail);

template <typename T, RCPPSW_SFINAE_FUNC(detail::has_to_str<T>::value)>
std::string to_string(const T& obj) {
  return obj.to_str();
}

/*******************************************************************************
 * Misc. Templates
 ******************************************************************************/
/**
 * @brief Implementation of std::make_unique which exists in C++14, but not
 * C++11, so if compiling for that standard I have to roll my own.
 */
template <typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
  return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

NS_START(detail);

template <typename T, typename Functor, std::size_t... Is>
void tuple_for_each(T&& t, const Functor& f, std::index_sequence<Is...>) {
  __attribute__((unused)) auto dummy = {(f(std::get<Is>(t)), 0)...};
}
NS_END(detail);

template <typename... Ts, typename Functor>
void tuple_apply(const std::tuple<Ts...>& t, const Functor& functor) {
  detail::tuple_for_each(t, functor, std::index_sequence_for<Ts...>{});
}

template <typename T>
T& maybe_deref(T& x) {
  return x;
}

template <typename T>
T& maybe_deref(T* x) {
  return *x;
}

template <typename T>
const T& maybe_deref(const T* x) {
  return *x;
}

NS_END(rcppsw);

#endif /* INCLUDE_RCPPSW_COMMON_COMMON_HPP_ */
