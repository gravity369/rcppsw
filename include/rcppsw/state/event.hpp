/**
 * @file event.hpp
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

#ifndef INCLUDE_RCPPSW_STATE_EVENT_HPP_
#define INCLUDE_RCPPSW_STATE_EVENT_HPP_

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
namespace rcppsw {
namespace patterns {

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
/// @brief Unique state machine event data must inherit from this class.
class event_data {
 public:
  virtual ~event_data() {}
};

typedef event_data no_event_data;

} /* namespace patterns */
} /* namespace rcppsw */

#endif /* INCLUDE_RCPPSW_STATE_EVENT_HPP_ */
