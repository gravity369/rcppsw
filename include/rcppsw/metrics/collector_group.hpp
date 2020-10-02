/**
 * \file collector_group.hpp
 *
 * \copyright 2018 John Harwell, All rights reserved.
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

#ifndef INCLUDE_RCPPSW_METRICS_COLLECTOR_GROUP_HPP_
#define INCLUDE_RCPPSW_METRICS_COLLECTOR_GROUP_HPP_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <algorithm>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <utility>

#include "rcppsw/common/common.hpp"
#include "rcppsw/metrics/base_metrics_collector.hpp"

/*******************************************************************************
 * Namespaces/Decls
 ******************************************************************************/
NS_START(rcppsw, metrics);

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/

/**
 * \class collector_group
 * \ingroup metrics
 *
 * \brief A group of N collectors, mapped by name, on which collective
 * operations can be performed, in addition to individual collection; used to
 * reduce code duplication.
 */
class collector_group {
 public:
  using key_type = std::string;
  using mapped_type = std::unique_ptr<base_metrics_collector>;

  collector_group(void) : m_collectors() {}
  virtual ~collector_group(void) = default;

  /**
   * \brief Add a collector to the group by constructing it in place.
   *
   * \tparam T The type of the collector
   *
   * \param name The key for the collector in the group. Should be unique,
   *             though it is not a requirement. If it is not unique then the
   *             older collector that was mapped to that name will be
   *             overwritten.
   *
   * \param args 0 or more arguments to the collector constructor.
   */
  template <typename T, typename... Args>
  bool collector_register(const key_type& name, Args&&... args) {
    auto it = m_collectors.find(name);
    if (it == m_collectors.end()) {
      m_collectors[name] = std::make_unique<T>(args...);
      return true;
    }
    return false;
  }
  bool collector_remove(const key_type& name) {
    auto it = m_collectors.find(name);
    if (it != m_collectors.end()) {
      m_collectors.erase(name);
      return true;
    }
    return false;
  }

  /**
   * \brief Collect metrics from the specified collector, passing it the
   * specified metrics set.
   *
   * \param name The registered name of the collector.
   * \param metrics The metrics to collect from.
   *
   * \return \c TRUE if the specified collector is registered and collection was
   * successful, \c FALSE otherwise.
   */
  bool collect(const key_type& name, const base_metrics& metrics) {
    auto it = m_collectors.find(name);
    if (it != m_collectors.end()) {
      it->second->collect(metrics);
      return true;
    }
    return false;
  }

  /**
   * \brief Collect metrics from the specified collector, passing it the
   * specified metrics set, but only if the specified condition is met by the
   * metrics.
   *
   * Useful if you have a "polymorphic" set of metrics that are very similar,
   * but need to be captured in different files, and you don't want to have to
   * derive nearly identical classes to handle it. The predicate is evaluated
   * *after* checking to see if the specified collector is enabled, because that
   * check has to be done anyway, and predicate evaluation is potentially
   * expensive.
   *
   * \param name The name of the collector to collect with.
   * \param metrics The metrics to collect.
   * \param predicate The predicate used to determine if the specified collector
   * should actually be invoked.
   *
   * \return \c TRUE if metrics were collected, \c FALSE otherwise.
   */
  bool collect_if(const key_type& name,
                  const base_metrics& metrics,
                  const std::function<bool(const base_metrics&)>& predicate) {
    auto it = m_collectors.find(name);
    if (it != m_collectors.end()) {
      if (predicate(metrics)) {
        it->second->collect(metrics);
        return true;
      }
    }
    return false;
  }

  /**
   * \brief Get a reference to a collector by name.
   *
   * \param key The mapped name of the collector in the group.
   */
  template <typename T>
  const T* get(const key_type& key) {
    return static_cast<T*>(m_collectors[key].get());
  }

  /**
   * \brief Call the \ref base_metrics_collector::reset() function on all
   * collectors in the group.
   */
  void reset_all(void) {
    std::for_each(m_collectors.begin(),
                  m_collectors.end(),
                  [&](const std::pair<const key_type, mapped_type>& pair) {
                    pair.second->reset();
                  });
  }

  /**
   * \brief Call the \ref base_metrics_collector::interval_reset() function on
   * all collectors in the group.
   */
  void interval_reset_all(void) {
    std::for_each(m_collectors.begin(),
                  m_collectors.end(),
                  [&](const std::pair<const key_type, mapped_type>& pair) {
                    pair.second->interval_reset();
                  });
  }

  /**
   * \brief Call the \ref base_metrics_collector::timestep_inc() function on all
   * collectors in the group.
   */
  void timestep_inc_all(void) {
    std::for_each(m_collectors.begin(),
                  m_collectors.end(),
                  [&](const std::pair<const key_type, mapped_type>& pair) {
                    pair.second->timestep_inc();
                  });
  }

  /**
   * \brief Call the \ref base_metrics_collector::csv_line_write() function on
   * all collectors in the group.
   *
   * \p fail_ok Is it OK if one or more collectors fail to write due to
   * filesystem I/O errors, or not?
   *
   * \return If \p fail_ok is \c FALSE, \c TRUE iff ALL collectors in the group
   * attempted to write out metrics this timestep and were successful, and \c
   * FALSE otherwise. If \p fail_ok is \c TRUE, then \c TRUE if all collectors
   * attempted to write out metrics this timestep, regardless of success, and \c
   * FALSE otherwise.
   */
  bool metrics_write_all(bool fail_ok) {
    return std::all_of(m_collectors.begin(),
                       m_collectors.end(),
                       [&](const std::pair<const key_type, mapped_type>& pair) -> bool {
                         auto res = pair.second->csv_line_write();
                         if (fail_ok) {
                           return !(res & metrics_write_status::ekNO_ATTEMPT);
                         } else {
                           return (res & metrics_write_status::ekSUCCESS);
                         }
                       });
  }

  /**
   * \brief Call the \ref base_metrics_collector::finalize() function on all
   * collectors in the group.
   */
  void finalize_all(void) {
    std::for_each(m_collectors.begin(),
                  m_collectors.end(),
                  [&](const std::pair<const key_type, mapped_type>& pair) {
                    pair.second->finalize();
                  });
  }

 private:
  /* clang-format off */
  std::map<key_type, mapped_type> m_collectors;
  /* clang-format on */
};

NS_END(metrics, rcppsw);

#endif /* INCLUDE_RCPPSW_METRICS_COLLECTOR_GROUP_HPP_ */
