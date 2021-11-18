#ifndef GRAPH3D_UTIL_BOUNDS_H_
#define GRAPH3D_UTIL_BOUNDS_H_

#include <cmath>
#include <iostream>

#include <glm/vec2.hpp>

#include <util/dimension.h>
#include <util/pipe.h>

namespace graph3d {
namespace util {
struct bounds {
 public:
  dimension first, second;

 public:
  bounds(bounds&& other) : first(other.first), second(other.second) { bindPipes(); }
  bounds(const bounds& other) : first(other.first), second(other.second) { bindPipes(); }
  bounds() { bindPipes(); }

  bounds(glm::vec2 first, glm::vec2 second) : bounds(dimension(first), dimension(second)) {}
  bounds(dimension first, dimension second) : first(first), second(second) { bindPipes(); }

 private:
  int getWidth() { return abs(second.width - first.width); }
  int getHeight() { return abs(second.height - first.height); }
  dimension getSize() { return dimension(width, height); }

 public:
  copy_pipe_get<int, bounds, &getWidth> width;
  copy_pipe_get<int, bounds, &getHeight> height;
  copy_pipe_get<dimension, bounds, &getSize> size;

 private:
  void bindPipes() {
    width.setParent(this);
    height.setParent(this);
    size.setParent(this);
  }

 public:
  bounds& operator=(const bounds& other) {
    first = other.first;
    second = other.second;
    return *this;
  }
};
}  // namespace util
}  // namespace graph3d

#endif