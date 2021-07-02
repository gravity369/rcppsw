/**
 * \file grid3D_metrics_data.hpp
 *
 * \copyright 2017 John Harwell, All rights reserved.
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

#ifndef INCLUDE_RCPPSW_METRICS_SPATIAL_GRID2D_METRICS_DATA_HPP_
#define INCLUDE_RCPPSW_METRICS_SPATIAL_GRID2D_METRICS_DATA_HPP_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "rcppsw/metrics/base_metrics_data.hpp"
#include "rcppsw/ds/grid2D.hpp"

/*******************************************************************************
 * Namespaces/Decls
 ******************************************************************************/
NS_START(rcppsw, metrics, spatial);

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
/**
 * \interface grid2D_metrics_data
 * \ingroup metrics spatial
 *
 * \brief Container for basic metrics gather from \ref rds::grid2D.
 */
struct grid2D_metrics_data : public rmetrics::base_metrics_data {
  explicit grid2D_metrics_data(const math::vector2z& dims) :
      grid(dims.x(), dims.y()) {}

  rcppsw::ds::grid2D<size_t> grid;
  size_t                     total_count{0};

};

NS_END(spatial, metrics, rcppsw);

#endif /* INCLUDE_RCPPSW_METRICS_SPATIAL_GRID2D_METRICS_DATA_HPP_ */
