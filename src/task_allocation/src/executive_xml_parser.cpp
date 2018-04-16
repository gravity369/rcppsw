/**
 * @file executive_parser.cpp
 *
 * @copyright 2018 John Harwell, All rights reserved.
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

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <argos3/core/utility/configuration/argos_configuration.h>
#include "rcppsw/task_allocation/executive_xml_parser.hpp"
#include "rcppsw/utils/line_parser.hpp"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NS_START(rcppsw, task_allocation);

/*******************************************************************************
 * Global Variables
 ******************************************************************************/
constexpr char executive_xml_parser::kXMLRoot[];

/*******************************************************************************
 * Member Functions
 ******************************************************************************/
void executive_xml_parser::parse(const ticpp::Element& node) {
  ticpp::Element enode =
      argos::GetNode(const_cast<ticpp::Element&>(node), kXMLRoot);

  XML_PARSE_PARAM(enode, m_params, estimation_alpha);
  XML_PARSE_PARAM(enode, m_params, abort_reactivity);
  XML_PARSE_PARAM(enode, m_params, abort_offset);
  XML_PARSE_PARAM(enode, m_params, partition_reactivity);
  XML_PARSE_PARAM(enode, m_params, partition_offset);
  XML_PARSE_PARAM(enode, m_params, subtask_selection_method);
  XML_PARSE_PARAM(enode, m_params, partition_method);
  XML_PARSE_PARAM(enode, m_params, always_partition);
  XML_PARSE_PARAM(enode, m_params, never_partition);
} /* parse() */

void executive_xml_parser::show(std::ostream& stream) const {
  stream << build_header()
         << XML_PARAM_STR(m_params, estimation_alpha) << std::endl
         << XML_PARAM_STR(m_params, abort_reactivity) << std::endl
         << XML_PARAM_STR(m_params, abort_offset) << std::endl
         << XML_PARAM_STR(m_params, partition_reactivity) << std::endl
         << XML_PARAM_STR(m_params, partition_offset) << std::endl
         << XML_PARAM_STR(m_params, subtask_selection_method) << std::endl
         << XML_PARAM_STR(m_params, partition_method) << std::endl
         << XML_PARAM_STR(m_params, always_partition) << std::endl
         << XML_PARAM_STR(m_params, never_partition) << std::endl
         << build_footer();
} /* show() */

__pure bool executive_xml_parser::validate(void) const {
  return !(m_params.estimation_alpha <= 0.0 ||
           m_params.abort_reactivity <= 0.0 || m_params.abort_offset <= 0.0 ||
           m_params.partition_reactivity <= 0.0 ||
           m_params.partition_offset <= 0.0);
} /* validate() */

NS_END(task_allocation, rcppsw);
