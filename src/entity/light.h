#ifndef GRAPH3D_ENTITY_LIGHT_H_
#define GRAPH3D_ENTITY_LIGHT_H_

#include <glm/vec3.hpp>

#include <entity/entity.h>

namespace graph3d {
namespace entity {
class Light : public Entity {
 public:
  glm::vec3 color;
  GLfloat intensity;

  Light(glm::vec3 color = {1, 1, 1}, GLfloat intensity = 1) : color(color), intensity(intensity) {}
};
}  // namespace entity
}  // namespace graph3d

#endif