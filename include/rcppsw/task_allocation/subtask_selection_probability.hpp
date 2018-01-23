/**
 * @file subtask_selection_probability.hpp
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

#ifndef INCLUDE_RCPPSW_TASK_ALLOCATION_SUBTASK_SELECTION_PROBABILITY_HPP_
#define INCLUDE_RCPPSW_TASK_ALLOCATION_SUBTASK_SELECTION_PROBABILITY_HPP_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <string>

#include "rcppsw/math/expression.hpp"
#include "rcppsw/task_allocation/time_estimate.hpp"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NS_START(rcppsw, task_allocation);

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
/**
 * @class subtask_selection_probability
 * @ingroup task_allocation
 *
 * @brief Calculates the probability that a robot selects subtask 2 when it has
 * most recently executed subtask 1 (assuming partitioning is employed).
 *
 * Taken/adapted from Brutschy2014, harwell2018. Note that Brutschy2014 using
 * the interface wait time for subtask as the input time estimates, and
 * harwell2018 using the estimate of the overall execution time of
 * subtasks. Mixing the two approaches will not give good results.
 *
 * Depends on:
 *
 * - The robot's time estimates of how long it takes to complete each of the two
 *   subtasks/estimates of interface time for each subtask.
 *
 * - The reactivity parameter: how quickly should the increase in selection
 *   probability be for an increasing difference between subtask time estimates.
 *
 * - The offset parameter: how quickly should robots react to an increasing
 *   difference between subtask time estimates?
 *
 * - 0 < reactivity < 1.
 * - offset > 1.
 * - 0 < gamma < 1.
 *
 */
class subtask_selection_probability : public rcppsw::math::expression<double> {
 public:
  explicit subtask_selection_probability(std::string method)
      : mc_method(std::move(method)), m_reactivity(0), m_offset(0), m_gamma(0) {}

  static constexpr char kBrutschy2014[] = "brutschy2014";
  static constexpr char kHarwell2018[] = "harwell2018";

  void init_sigmoid(double reactivity, double offset, double gamma);
  double calc(const time_estimate* subtask1, const time_estimate* subtask2);

  const std::string& method(void) const { return mc_method; }

 private:
  double calc_random(void);

  /**
   * @brief Calculate the probability of switching from subtask 1 to subtask 2
   * using the piecewise method described in Brutschy2014.
   *
   * @param int_est1 Estimate of \a interface time for subtask1.
   * @param int_est2 Estimate of \a interface time for subtask2.
   *
   * @return Probability of switching.
   */
  double calc_brutschy2014(const time_estimate& int_est1,
                           const time_estimate& int_est2);

  /**
   * @brief Calculate the probability of switching from subtask 1 to subtask 2
   * using the piecewise method described in Brutschy2014.
   *
   * @param int_est1 Estimate of \a exec time for subtask1.
   * @param int_est2 Estimate of \a exec time for subtask2.
   *
   * @return Probability of switching.
   */
  double calc_harwell2018(const time_estimate& exec_est1,
                          const time_estimate& exec_est2);

  /**
   * @brief Calculate the sigmoid activation for a pair of time estimates using
   * the equation from Brutschy2014.
   *
   * @param est1 Exec/interface estimate 1.
   * @param est2 Exec/interface estimate 2.
   *
   * @return Sigmoid value.
   */
  double calc_sigmoid(const time_estimate& est1, const time_estimate& est2);

  // clang-format off
  const std::string mc_method;
  double            m_reactivity;
  double            m_offset;
  double            m_gamma;
  // clang-format on
};

NS_END(task_allocation, rcppsw);

#endif /* INCLUDE_RCPPSW_TASK_ALLOCATION_SUBTASK_SELECTION_PROBABILITY_HPP_ */
