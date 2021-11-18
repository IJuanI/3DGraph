#ifndef GRAPH3D_ENTITY_OBJECT_H_
#define GRAPH3D_ENTITY_OBJECT_H_

#include <entity/entity.h>

namespace graph3d {
namespace opengl {
class OpenGL;
}

namespace entity {
class Object : public Entity {
  friend class graph3d::opengl::OpenGL;

 private:
  std::string modelAlias;

 public:
  void setModel(std::string model) { modelAlias = model; }
};

}  // namespace entity
}  // namespace graph3d

#endif