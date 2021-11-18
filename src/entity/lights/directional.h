#ifndef GRAPH3D_ENTITY_LIGHTS_DIRECTIONAL_H_
#define GRAPH3D_ENTITY_LIGHTS_DIRECTIONAL_H_

#include <glm/vec3.hpp>

#include <entity/light.h>

namespace graph3d {
namespace entity {
class DirectionalLight : public Light {
 public:
  glm::vec3 direction;

  DirectionalLight(glm::vec3 direction, glm::vec3 color = {1, 1, 1}, GLfloat intensity = 1)
      : Light(color, intensity), direction(direction) {}
};
}  // namespace entity
}  // namespace graph3d

#endif