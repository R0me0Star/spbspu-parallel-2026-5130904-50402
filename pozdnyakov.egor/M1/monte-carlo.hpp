#ifndef MONTE_CARLO_HPP
#define MONTE_CARLO_HPP

#include "arguments.hpp"
#include "shapes.hpp"

namespace pozdnyakov
{
  struct areas_t
  {
    double coverage;
    double intersection;
  };

  areas_t computeAreas(const circles_t& circles, const arguments_t& args);
}

#endif
