/**
 * @file releasing_factory.hpp
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

#ifndef INCLUDE_RCPPSW_PATTERNS_FACTORY_RELEASING_FACTORY_HPP_
#define INCLUDE_RCPPSW_PATTERNS_FACTORY_RELEASING_FACTORY_HPP_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include "rcppsw/patterns/factory/base_factory.hpp"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NS_START(rcppsw, patterns, factory);

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
/**
 * @class releasing_factory
 * @ingroup patterns factory
 *
 * @brief A factory that releases ownership of the created objects to the
 * class/context that requests object creation.
 *
 * @tparam T Type of objects capable of creation from this factory (restricted
 * to this type and its derived types).
 *
 * @tparam TypeArgs List of constructor arguments for the types to be
 * constructed. If this is non-empty then ALL derived types of the base type
 * that you want to use with the same unique_factory instance will have to have
 * the same construct signature available. If this is empty, then the only
 * limitation on types the factor will accept is what is specified above.
 */
template <typename T, typename... TypeArgs>
class releasing_factory : public base_factory {
 public:
  releasing_factory(void) : m_release_funcs() {}
  ~releasing_factory(void) override = default;

  /**
   * @brief Register a type with the factory, and associate it with the
   * specified name.
   *
   * The type to register must be derived from the factory base class.
   */
  template <typename TDerived, typename... Args>
  status_t register_type(const std::string& name) {
    static_assert(std::is_base_of<T, TDerived>::value,
                  "releasing_factory::register_type only accepts types derived from the base");
    if (m_release_funcs.end() == m_release_funcs.find(name)) {
      m_release_funcs[name] = &releasing_factory::template do_create_release<TDerived, TypeArgs...>;
      }
    return OK;
  }

  /**
   * @brief Create the requested object, and release ownership to the calling
   * context/application.
   */
  template<typename... Args>
  std::unique_ptr<T> create(const std::string& name, Args... args) {
    auto it = m_release_funcs.find(name);
    if (it != m_release_funcs.end()) {
      return (this->*(it->second))(args...);
    }
    return nullptr;
  }

 private:
  using create_func_type = std::unique_ptr<T> (releasing_factory<T, TypeArgs...>::*)(TypeArgs...);

  template <typename TDerived, typename... Args>
  std::unique_ptr<T> do_create_release(TypeArgs... args) {
    return std::make_unique<TDerived>(std::forward<Args>(args)...);
  }

  std::map<std::string, create_func_type> m_release_funcs;
};

NS_END(factory, patterns, rcppsw);

#endif /* INCLUDE_RCPPSW_PATTERNS_FACTORY_RELEASING_FACTORY_HPP_ */
