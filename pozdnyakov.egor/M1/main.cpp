#include <exception>
#include <iostream>
#include <stdexcept>
#include "arguments.hpp"
#include "monte-carlo.hpp"
#include "shapes.hpp"

namespace
{
  pozdnyakov::circles_t readCircles(std::istream& in)
  {
    pozdnyakov::circles_t circles;
    while (!(in >> std::ws).eof())
    {
      pozdnyakov::circle_t circle{{0.0, 0.0}, 0.0};
      if (!(in >> circle))
      {
        throw std::invalid_argument("figure parameters must be four integers");
      }
      circles.push_back(circle);
    }
    return circles;
  }
}

int main(int argc, char** argv)
{
  const int invalid_input_code = 1;
  const int internal_error_code = 2;
  try
  {
    const pozdnyakov::arguments_t args = pozdnyakov::parseArguments(argc, argv);
    const pozdnyakov::circles_t circles = readCircles(std::cin);
    const pozdnyakov::areas_t areas = pozdnyakov::computeAreas(circles, args);
    std::cout << areas.coverage << ' ' << areas.intersection << '\n';
  }
  catch (const std::invalid_argument& error)
  {
    std::cerr << error.what() << '\n';
    return invalid_input_code;
  }
  catch (const std::exception& error)
  {
    std::cerr << error.what() << '\n';
    return internal_error_code;
  }
  return 0;
}
