/**
 * @file state_entry_action.hpp
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

#ifndef INCLUDE_RCPPSW_PATTERNS_STATE_MACHINE_STATE_ENTRY_ACTION_HPP_
#define INCLUDE_RCPPSW_PATTERNS_STATE_MACHINE_STATE_ENTRY_ACTION_HPP_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <assert.h>
#include "rcppsw/common/common.hpp"
#include "rcppsw/patterns/state_machine/event.hpp"
#include "rcppsw/patterns/state_machine/state_entry.hpp"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NS_START(rcppsw, patterns, state_machine);
class base_fsm;

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
/**
 * @brief state_entry_action takes two template arguments:
 *
 * - A state machine class
 * - A state function event data type (derived from event_data).
 * - A state machine  member function pointer that takes ZERO arguments.
*/
template <class SM, void (SM::*Func)(void)>
class state_entry_action0 : public state_entry {
 public:
  virtual ~state_entry_action0(void) {}
  void invoke_entry_action(base_fsm* sm, const event_data*) const override {
    SM* derived_fsm = static_cast<SM*>(sm);
    (derived_fsm->*Func)();
  }
};

/**
 * @brief state_entry_action takes three template arguments:
 *
 * - A state machine class
 * - A state function event data type (derived from event_data).
 * - A state machine  member function pointer that takes ONE argument.
 */
template <class SM, class Event, void (SM::*Func)(const Event*)>
class state_entry_action1 : public state_entry {
 public:
  virtual ~state_entry_action1(void) {}
  void invoke_entry_action(base_fsm* sm,
                           const event_data* data) const override {
    SM* derived_fsm = static_cast<SM*>(sm);
    const Event* derived_event = NULL;
    if (data) {
      derived_event = dynamic_cast<const Event*>(data);
      assert(derived_event);
    }
    (derived_fsm->*Func)(derived_event);
  }
};

NS_END(state_machine, patterns, rcppsw);

#endif /* INCLUDE_RCPPSW_PATTERNS_STATE_MACHINE_STATE_ENTRY_ACTION_HPP_ */
