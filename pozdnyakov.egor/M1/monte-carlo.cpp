#include "monte-carlo.hpp"
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <random>
#include <thread>
#include <vector>
#include "arguments.hpp"
#include "shapes.hpp"

namespace
{
  struct hits_t
  {
    std::size_t covered;
    std::size_t intersected;
  };

  using engine_t = std::mt19937_64;
  using pool_t = std::vector< std::thread >;

  engine_t makeEngine(std::size_t seed, std::size_t stream)
  {
    const unsigned int word_bits = 32;
    const std::uint64_t wide_seed = seed;
    const std::uint64_t wide_stream = stream;
    std::seed_seq sequence{wide_seed, wide_seed >> word_bits, wide_stream, wide_stream >> word_bits};
    return engine_t(sequence);
  }

  void countHits(const pozdnyakov::circles_t& circles, const pozdnyakov::rectangle_t& frame, std::size_t tries,
      engine_t engine, hits_t& hits)
  {
    const double range = static_cast< double >(engine_t::max());
    const double width = (frame.right_top.x - frame.left_bottom.x) / range;
    const double height = (frame.right_top.y - frame.left_bottom.y) / range;
    hits_t local{0, 0};
    for (std::size_t i = 0; i < tries; ++i)
    {
      const double x = frame.left_bottom.x + width * static_cast< double >(engine());
      const double y = frame.left_bottom.y + height * static_cast< double >(engine());
      const pozdnyakov::point_t point{x, y};
      std::size_t owners = 0;
      for (const pozdnyakov::circle_t& circle : circles)
      {
        if (pozdnyakov::isInside(circle, point))
        {
          ++owners;
        }
      }
      if (owners > 0)
      {
        ++local.covered;
      }
      if (owners == circles.size())
      {
        ++local.intersected;
      }
    }
    hits = local;
  }

  std::size_t getWorkers(std::size_t threads, std::size_t tries)
  {
    std::size_t workers = threads;
    if (workers == 0)
    {
      workers = std::max< std::size_t >(std::thread::hardware_concurrency(), 1);
    }
    return std::min(workers, tries);
  }

  std::size_t getShare(std::size_t tries, std::size_t workers, std::size_t index)
  {
    const std::size_t extra = (index < (tries % workers)) ? 1 : 0;
    return tries / workers + extra;
  }

  void joinAll(pool_t& pool)
  {
    for (std::thread& worker : pool)
    {
      worker.join();
    }
  }
}

pozdnyakov::areas_t pozdnyakov::computeAreas(const circles_t& circles, const arguments_t& args)
{
  const std::size_t tries = args.tries;
  const std::size_t seed = args.seed;
  if (circles.empty())
  {
    return {0.0, 0.0};
  }
  const rectangle_t frame = getFrame(circles);
  const std::size_t workers = getWorkers(args.threads, tries);
  std::vector< hits_t > hits(workers, hits_t{0, 0});
  pool_t pool;
  pool.reserve(workers - 1);
  try
  {
    for (std::size_t i = 1; i < workers; ++i)
    {
      const std::size_t share = getShare(tries, workers, i);
      pool.emplace_back(countHits, std::cref(circles), std::cref(frame), share, makeEngine(seed, i), std::ref(hits[i]));
    }
    countHits(circles, frame, getShare(tries, workers, 0), makeEngine(seed, 0), hits[0]);
  }
  catch (...)
  {
    joinAll(pool);
    throw;
  }
  joinAll(pool);
  hits_t total{0, 0};
  for (const hits_t& part : hits)
  {
    total.covered += part.covered;
    total.intersected += part.intersected;
  }
  const double trial = getArea(frame) / static_cast< double >(tries);
  return {trial * static_cast< double >(total.covered), trial * static_cast< double >(total.intersected)};
}
