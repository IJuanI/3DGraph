#ifndef GRAPH3D_UTIL_RESIZER_H_
#define GRAPH3D_UTIL_RESIZER_H_

#include <utility>

#include <glm/vec2.hpp>

#include <util/bounds.h>
#include <util/dimension.h>

namespace graph3d {
namespace util {
class Resizer {
 private:
  util::bounds (*sizer)(const int, const int) = nullptr;
  glm::vec2 minifier, maxifier;

 public:
  Resizer(const glm::vec2&& minifier, const glm::vec2&& maxifier) : Resizer(minifier, maxifier) {}
  Resizer(const glm::vec2& minifier, const glm::vec2&& maxifier)
      : Resizer(std::forward<const glm::vec2&>(minifier), maxifier) {}
  Resizer(const glm::vec2&& minifier, const glm::vec2& maxifier)
      : Resizer(minifier, std::forward<const glm::vec2&>(maxifier)) {}
  Resizer(const glm::vec2& minifier, const glm::vec2& maxifier) : minifier(minifier), maxifier(maxifier) {}

  Resizer(util::bounds (*sizer)(const int, const int)) : sizer(sizer) {}

 public:
  util::bounds calcSize(const int width, const int height) const {
    if (sizer) return sizer(width, height);
    return util::bounds(glm::vec2(minifier.x * width, minifier.y * height),
                        glm::vec2(maxifier.x * width, maxifier.y * height));
  }

 public:
  inline void modify(const glm::vec2&& minifier, const glm::vec2&& maxifier) { modify(minifier, maxifier); }
  inline void modify(const glm::vec2&& minifier, const glm::vec2& maxifier) {
    modify(minifier, std::forward<const glm::vec2&>(maxifier));
  }
  inline void modify(const glm::vec2& minifier, const glm::vec2&& maxifier) {
    modify(std::forward<const glm::vec2&>(minifier), maxifier);
  }
  void modify(const glm::vec2& minifier, const glm::vec2& maxifier) {
    sizer = nullptr;
    this->minifier = minifier;
    this->maxifier = maxifier;
  }

  void modify(util::bounds (*sizer)(const int, const int)) { this->sizer = sizer; }
};
}  // namespace util
}  // namespace graph3d

#endif