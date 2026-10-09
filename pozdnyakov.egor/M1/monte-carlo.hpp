#ifndef MONTE_CARLO_HPP
#define MONTE_CARLO_HPP

#include <cstddef>
#include "arguments.hpp"
#include "shapes.hpp"

namespace pozdnyakov {
  struct hits_t {
    std::size_t covered;
    std::size_t intersected;
  };

  struct areas_t {
    double coverage;
    double intersection;
  };

  hits_t countHits(const circles_t& circles, const rectangle_t& frame, std::size_t tries, std::size_t seed);
  areas_t computeAreas(const circles_t& circles, const arguments_t& arguments);
}

#endif
