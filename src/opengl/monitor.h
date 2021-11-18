#ifndef GRAPH3D_OPENGL_MONITOR_H_
#define GRAPH3D_OPENGL_MONITOR_H_

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <util/dimension.h>
#include <util/pipe.h>

namespace graph3d {
namespace opengl {
class Window;

class Monitor {
 private:
  GLFWmonitor *ref;
  const GLFWvidmode *mode;

 public:
  Monitor& operator=(const Monitor&) = delete;
  Monitor(const Monitor&) = delete;

  Monitor();
  Monitor(int);
  Monitor(const Window &);

 public:
  GLFWmonitor *getRef() { return ref; }

 private:
  util::dimension getSize() { return util::dimension(mode->width, mode->height); }

  int getRefreshRate() { return mode->refreshRate; }

  glm::vec2 getPosition() {
    int x, y;
    glfwGetMonitorPos(ref, &x, &y);
    return glm::vec2(x, y);
  }

 public:
  util::copy_pipe_get<util::dimension, Monitor, &getSize> size;
  util::copy_pipe_get<int, Monitor, &getRefreshRate> refreshRate;
  util::copy_pipe_get<glm::vec2, Monitor, &getPosition> position;

 private:
  void init(GLFWmonitor *ref) {
    this->ref = ref;
    mode = glfwGetVideoMode(ref);
    bindPipes();
  }

  void bindPipes() {
    size.setParent(this);
    refreshRate.setParent(this);
    position.setParent(this);
  }
};
}  // namespace opengl
}  // namespace graph3d

#endif
