#ifndef SHAPES_HPP
#define SHAPES_HPP

#include <iosfwd>
#include <vector>

namespace pozdnyakov {
  struct point_t {
    double x;
    double y;
  };

  struct circle_t {
    point_t center;
    double radius;
  };

  struct rectangle_t {
    point_t leftBottom;
    point_t rightTop;
  };

  using circles_t = std::vector< circle_t >;

  std::istream& operator>>(std::istream& in, circle_t& circle);

  bool isInside(const circle_t& circle, const point_t& point);
  rectangle_t getFrame(const circle_t& circle);
  rectangle_t getFrame(const circles_t& circles);
  double getArea(const rectangle_t& rectangle);
}

#endif
