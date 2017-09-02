/**
 * @file pid_loop.hpp
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

#ifndef INCLUDE_RCPPSW_CONTROL_PID_LOOP_HPP_
#define INCLUDE_RCPPSW_CONTROL_PID_LOOP_HPP_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "rcppsw/common/common.hpp"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NS_START(rcppsw, control);

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
/**
 * @brief A simple PID loop class.
 *
 *  Kp -  proportional gain
 *  Ki -  Integral gain
 *  Kd -  derivative gain
 *  dt -  loop interval time
 *  max - maximum value of manipulated variable
 *  min - minimum value of manipulated variable
 */
class pid_loop {
 public:
  pid_loop(double Kp, double Kd, double Ki, double dt, double min, double max) :
      m_Kp(Kp),
      m_Kd(Kd),
      m_Ki(Ki),
      m_dt(dt),
      m_min(min),
      m_max(max),
      m_pre_error(0),
      m_integral(0) {}
  ~pid_loop() {}

  /**
   * @brief Calculate a new value for the manipulated variable.
   *
   * @param setpoint Desired value of the manipulated variable.
   * @param pv Current value of the variable.
   *
   * @return Correctional term (I think).
   */
  double calculate(double setpoint, double pv);

 private:
  double m_Kp;
  double m_Kd;
  double m_Ki;
  double m_dt;
  double m_min;
  double m_max;
  double m_pre_error;
  double m_integral;
};

NS_END(control, rcppsw);

#endif /* INCLUDE_RCPPSW_CONTROL_PID_LOOP_HPP_ */
