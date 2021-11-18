#ifndef GRAPH3D_UTIL_DIMENSION_H_
#define GRAPH3D_UTIL_DIMENSION_H_

#include <glm/vec2.hpp>

namespace graph3d {
namespace util {

struct dimension {
  int width;
  int height;
  dimension(const int &width, const int &height) : width(width), height(height) {}

  dimension(const glm::vec2 &vec) : width((int)vec.x), height((int)vec.y) {}

  dimension() {}

  dimension &operator=(const glm::vec2 &vec) {
    width = (int)vec.x;
    height = (int)vec.y;
    return *this;
  }

  operator glm::vec2() { return glm::vec2(width, height); }
  operator glm::vec2() const { return glm::vec2(width, height); }
};

}  // namespace util
}  // namespace graph3d

#endif
