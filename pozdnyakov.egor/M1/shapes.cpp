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
  rectangle_t frame = getFrame(circles.front());
  for (const circle_t& circle : circles)
  {
    const rectangle_t next = getFrame(circle);
    frame.left_bottom.x = std::min(frame.left_bottom.x, next.left_bottom.x);
    frame.left_bottom.y = std::min(frame.left_bottom.y, next.left_bottom.y);
    frame.right_top.x = std::max(frame.right_top.x, next.right_top.x);
    frame.right_top.y = std::max(frame.right_top.y, next.right_top.y);
  }
  return frame;
}

double pozdnyakov::getArea(const rectangle_t& rectangle)
{
  return (rectangle.right_top.x - rectangle.left_bottom.x) * (rectangle.right_top.y - rectangle.left_bottom.y);
}
