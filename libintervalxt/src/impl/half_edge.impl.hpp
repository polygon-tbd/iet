/**********************************************************************
 *  This file is part of intervalxt.
 *
 *        Copyright (C) 2019 Vincent Delecroix
 *        Copyright (C) 2019-2020 Julian Rüth
 *
 *  intervalxt is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  intervalxt is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with intervalxt. If not, see <https://www.gnu.org/licenses/>.
 *********************************************************************/

#ifndef LIBINTERVALXT_HALF_EDGE_IMPL_HPP
#define LIBINTERVALXT_HALF_EDGE_IMPL_HPP

#include <memory>

#include "../../intervalxt/component.hpp"
#include "../../intervalxt/half_edge.hpp"
#include "../../intervalxt/label.hpp"
#include "implementation_of_decomposition.hpp"

namespace intervalxt {

template <>
class ImplementationOf<HalfEdge> : public ImplementationOfDecomposition {
 public:
  enum class Contour {
    TOP = -1,
    BOTTOM = 1,
  };

  ImplementationOf(const DynamicalDecomposition&, DecompositionState::Component*, Label, Contour);
  static HalfEdge make(const DynamicalDecomposition&, DecompositionState::Component*, Label, Contour);

  DecompositionState::Component* component;
  Label label;
  Contour contour;
};

template <typename... Args>
HalfEdge::HalfEdge(PrivateConstructor, Args&&... args) :
  self(spimpl::make_impl<ImplementationOf<HalfEdge>>(std::forward<Args>(args)...)) {}

}  // namespace intervalxt

#endif
