#ifndef GRAPH3D_OPENGL_DRAWER_H_
#define GRAPH3D_OPENGL_DRAWER_H_

namespace graph3d {
struct Context;

namespace opengl {
class drawer {
 public:
  virtual void draw(const Context&) = 0;
};

}  // namespace opengl
}  // namespace graph3d

#endif