/**
 * @file phototaxis_force_parser.hpp
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

#ifndef INCLUDE_RCPPSW_ROBOTICS_STEER2D_CONFIG_XML_PHOTOTAXIS_FORCE_PARSER_HPP_
#define INCLUDE_RCPPSW_ROBOTICS_STEER2D_CONFIG_XML_PHOTOTAXIS_FORCE_PARSER_HPP_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <string>
#include <memory>

#include "rcppsw/robotics/steer2D/config/phototaxis_force_config.hpp"
#include "rcppsw/config/xml/xml_config_parser.hpp"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NS_START(rcppsw, robotics, steer2D, config, xml);

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
/**
 * @class phototaxis_force_parser
 * @ingroup rcppsw robotics steer2D config xml
 *
 * @brief Parses XML parameters for related to \ref phototaxis_force objects
 * into \ref phototaxis_force_config.
 */
class phototaxis_force_parser : public rcppsw::config::xml::xml_config_parser {
 public:
  explicit phototaxis_force_parser(uint level) : xml_config_parser(level) {}

  /**
   * @brief The root tag that all phototaxis_force parameters should lie under
   * in the XML tree.
   */
  static constexpr char kXMLRoot[] = "phototaxis_force";

  void parse(const ticpp::Element& node) override;
  bool validate(void) const override;

  std::string xml_root(void) const override { return kXMLRoot; }
  bool parsed(void) const override { return m_parsed; }

  std::shared_ptr<phototaxis_force_config> config_get(void) const {
    return m_config;
  }

 private:
  std::shared_ptr<rcppsw::config::base_config> config_get_impl(void) const override {
    return m_config;
  }

  /* clang-format on */
  bool m_parsed{false};
  std::shared_ptr<phototaxis_force_config> m_config{nullptr};
  /* clang-format off */
};

NS_END(xml, config, steer2D, robotics, rcppsw);

#endif /* INCLUDE_RCPPSW_ROBOTICS_STEER2D_CONFIG_XML_PHOTOTAXIS_FORCE_PARSER_HPP_ */
