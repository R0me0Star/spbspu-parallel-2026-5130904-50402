#include "shapes.hpp"
#include <algorithm>
#include <istream>

std::istream& pozdnyakov::operator>>(std::istream& in, circle_t& circle)
{
  int radius = 0;
  int ignored = 0;
  int x = 0;
  int y = 0;
  if (in >> radius >> ignored >> x >> y)
  {
    circle.center.x = x;
    circle.center.y = y;
    circle.radius = radius;
  }
  return in;
}

bool pozdnyakov::isInside(const circle_t& circle, const point_t& point)
{
  const double dx = point.x - circle.center.x;
  const double dy = point.y - circle.center.y;
  return (dx * dx + dy * dy) <= (circle.radius * circle.radius);
}

pozdnyakov::rectangle_t pozdnyakov::getFrame(const circle_t& circle)
{
  const point_t left_bottom{circle.center.x - circle.radius, circle.center.y - circle.radius};
  const point_t right_top{circle.center.x + circle.radius, circle.center.y + circle.radius};
  return {left_bottom, right_top};
}

pozdnyakov::rectangle_t pozdnyakov::getFrame(const circles_t& circles)
{
  const rectangle_t first = getFrame(circles.front());
  double low = std::min(first.left_bottom.x, first.left_bottom.y);
  double high = std::max(first.right_top.x, first.right_top.y);
  for (const circle_t& circle : circles)
  {
    const rectangle_t next = getFrame(circle);
    low = std::min({low, next.left_bottom.x, next.left_bottom.y});
    high = std::max({high, next.right_top.x, next.right_top.y});
  }
  return {{low, low}, {high, high}};
}

double pozdnyakov::getArea(const rectangle_t& rectangle)
{
  return (rectangle.right_top.x - rectangle.left_bottom.x) * (rectangle.right_top.y - rectangle.left_bottom.y);
}
