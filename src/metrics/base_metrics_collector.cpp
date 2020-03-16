/**
 * \file base_metric_collector.cpp
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

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "rcppsw/metrics/base_metrics_collector.hpp"

#include <filesystem>
#include <iomanip>
#include <numeric>
#include <sstream>

/*******************************************************************************
 * Namespaces/Decls
 ******************************************************************************/
NS_START(rcppsw, metrics);

/*******************************************************************************
 * Constructors/Destructor
 ******************************************************************************/
base_metrics_collector::base_metrics_collector(const std::string& ofname_stem,
                                               const types::timestep& interval,
                                               const output_mode& mode)
    : ER_CLIENT_INIT("rcppsw.metrics.base_collector"),
      mc_output_mode(mode),
      mc_ofname_stem(ofname_stem),
      m_interval(interval) {}

/*******************************************************************************
 * Member Functions
 ******************************************************************************/
bool base_metrics_collector::csv_line_write(void) {
  if (auto line = csv_line_build()) {
    if (output_mode::ekAPPEND == mc_output_mode) {
      m_ofile << rcppsw::to_string(m_timestep) + mc_separator + *line
              << std::endl;
    } else if (output_mode::ekTRUNCATE == mc_output_mode) {
      std::filesystem::resize_file(mc_ofname_stem + mc_ofname_ext, 0);
      m_ofile.seekp(0);
      csv_header_write();
      m_ofile << *line << std::endl;
    } else if (output_mode::ekCREATE == mc_output_mode) {
      std::stringstream ss;
      /*
       * +1 to get things to come out evenly because we start with
       * timestep 0.
       */
      ss << std::setw(10) << std::setfill('0') << (m_timestep.v() + 1);

      m_ofile.open(mc_ofname_stem + "_" + ss.str() + mc_ofname_ext,
                   std::ios_base::trunc | std::ios_base::out);
      csv_header_write();
      m_ofile << *line << std::endl;
      m_ofile.close();
    } else {
      ER_FATAL_SENTINEL("Bad output mode '%d'",
                        rcppsw::as_underlying(mc_output_mode));
    }
    return true;
  }
  return false;
} /* csv_line_write() */

void base_metrics_collector::csv_header_write(void) {
  auto cols = csv_header_cols();
  std::string header = std::accumulate(std::next(cols.begin()),
                                       cols.end(),
                                       std::string(cols.front()),
                                       [&](const auto& sum, const auto& col) {
                                         return sum + separator() + col;
                                       });
  m_ofile << header + "\n";
  m_ofile.flush();
} /* csv_header_write() */

void base_metrics_collector::reset(void) {
  /* Open output file and truncate */
  if (m_ofile.is_open()) {
    m_ofile.close();
  }
  /*
   * Nothing to do if we are creating a new file each time, because we don't
   * know that the timestep reset() is called on and the timestep that we will
   * be outputting metrics are the same.
   */
  if (output_mode::ekAPPEND == mc_output_mode ||
      output_mode::ekTRUNCATE == mc_output_mode) {
    m_ofile.open(mc_ofname_stem + mc_ofname_ext,
                 std::ios_base::trunc | std::ios_base::out);

    csv_header_write();
  }
} /* reset() */

void base_metrics_collector::interval_reset(void) {
  if (m_timestep > 0U && (m_timestep % m_interval == 0)) {
    reset_after_interval();
  }
} /* interval_reset() */

NS_END(metrics, rcppsw);
