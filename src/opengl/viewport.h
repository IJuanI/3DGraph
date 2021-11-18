#ifndef GRAPH3D_OPENGL_VIEWPORT_H_
#define GRAPH3D_OPENGL_VIEWPORT_H_

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <algorithm>
#include <functional>
#include <vector>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include <entity/camera.h>
#include <entity/light.h>
#include <opengl/drawer.h>
#include <util/bounds.h>
#include <util/dimension.h>
#include <util/pipe.h>
#include <util/resizer.h>

namespace graph3d {
namespace opengl {
class Window;

class Viewport {
  friend class graph3d::opengl::Window;

 private:
  typedef std::pair<drawer*, int32_t> drawer_entry;
  typedef std::function<void(const Viewport&)> close_func_t;

 private:
  static uint32_t lastId;
  static bool drawer_comp(const drawer_entry& a, const drawer_entry& b) { return a.second < b.second; }

 private:
  uint32_t id = lastId++;
  GLbitfield g_clearMask;
  glm::vec4 clearColor;

  util::dimension* window;
  util::bounds g_bounds;

  util::Resizer g_resizer;
  std::vector<drawer_entry> drawers;

  std::vector<entity::Light*> lights;

 public:
  entity::Camera* camera = nullptr;
  int32_t zindex = 0;

 private:
  std::vector<close_func_t> closeSubscribers;

 public:
  Viewport& operator=(const Viewport&) = delete;
  Viewport(const Viewport&) = delete;

  Viewport(util::dimension* window)
      : g_resizer(glm::vec2(0, 0), glm::vec2(1, 1)), window(window), g_clearMask(GL_COLOR_BUFFER_BIT) {
    g_bounds = g_resizer.calcSize(window->width, window->height);

    bindPipes();
  }

 private:
  glm::vec4& getBackgroundColor() { return clearColor; }
  GLbitfield& getClearMask() { return g_clearMask; }
  util::Resizer& getResizer() { return g_resizer; }
  util::bounds getBounds() { return g_bounds; }

 private:
  bool isDepthTesting() { return g_clearMask & GL_DEPTH_BUFFER_BIT; }
  bool isStencilTesting() { return g_clearMask & GL_STENCIL_BUFFER_BIT; }

 private:
  const glm::vec4& setBackgroundColor(const glm::vec4& backColor) {
    clearColor = backColor;
    return backColor;
  }

  const GLbitfield& setClearMask(const GLbitfield& clearMask) {
    g_clearMask = clearMask;
    return clearMask;
  }

  const bool& setDepthTesting(const bool& depthTesting) {
    if (depthTesting)
      g_clearMask |= GL_DEPTH_BUFFER_BIT;
    else if (g_clearMask & GL_DEPTH_BUFFER_BIT)
      g_clearMask &= ~GL_DEPTH_BUFFER_BIT;

    return depthTesting;
  }

  const bool& setStencilTesting(const bool& stencilTesting) {
    if (stencilTesting)
      g_clearMask |= GL_STENCIL_BUFFER_BIT;
    else if (g_clearMask & GL_STENCIL_BUFFER_BIT)
      g_clearMask &= ~GL_STENCIL_BUFFER_BIT;

    return stencilTesting;
  }

  const util::Resizer& setResizer(const util::Resizer& resizer) {
    g_resizer = resizer;
    updateSize();
    return resizer;
  }

 public:
  util::fwd_pipe<glm::vec4, Viewport, &getBackgroundColor, &setBackgroundColor> backgroundColor;
  util::fwd_pipe<GLbitfield, Viewport, &getClearMask, &setClearMask> clearMask;
  util::copy_pipe<bool, Viewport, &isDepthTesting, &setDepthTesting> depthTesting;
  util::copy_pipe<bool, Viewport, &isStencilTesting, &setStencilTesting> stencilTesting;
  util::fwd_pipe<util::Resizer, Viewport, &getResizer, &setResizer> resizer;
  util::copy_pipe_get<util::bounds, Viewport, &getBounds> bounds;

 public:
  void close() const {
    for (close_func_t func : closeSubscribers) func(*this);
  }

  void attachDrawer(drawer* listener, int32_t priority = 0) {
    drawer_entry item = std::make_pair(listener, priority);
    drawers.insert(std::upper_bound(drawers.begin(), drawers.end(), item, drawer_comp), item);
  }

  void detachDrawer(const drawer* drawer) {
    auto it = drawers.begin(), end = drawers.end();

    while (it != end) {
      if (it->first == drawer) {
        it = drawers.erase(it);
        end = drawers.end();
      } else
        ++it;
    }
  }

  void attachLight(entity::Light* light) { lights.push_back(light); }

  void detachLight(entity::Light* light) {
    auto it = lights.begin(), end = lights.end();

    while (it != end) {
      if (*it == light) {
        it = lights.erase(it);
        end = lights.end();
      } else
        ++it;
    }
  }

 public:
  template <typename Parent>
  void onClose(void (Parent::*func)(const Viewport&), Parent* parent) {
    closeSubscribers.push_back([func, parent](const Viewport& viewport) { (parent->*func)(viewport); });
  }

  bool operator==(const Viewport& other) const { return id == other.id; }

 private:
  void updateSize() { g_bounds = g_resizer.calcSize(window->width, window->height); }

  void draw(const Context& context) const {
    glViewport(g_bounds.first.width, g_bounds.first.height, g_bounds.second.width, g_bounds.second.height);

    glEnable(GL_SCISSOR_TEST);
    glScissor(g_bounds.first.width, g_bounds.first.height, g_bounds.width, g_bounds.height);

    if (depthTesting)
      glEnable(GL_DEPTH_TEST);
    else
      glDisable(GL_DEPTH_TEST);

    if (stencilTesting)
      glEnable(GL_STENCIL_TEST);
    else
      glDisable(GL_STENCIL_TEST);

    glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
    glClear(g_clearMask);

    for (std::pair<drawer*, int32_t> drawer : drawers) drawer.first->draw(context);
  }

  void bindPipes() {
    backgroundColor.setParent(this);
    clearMask.setParent(this);
    depthTesting.setParent(this);
    stencilTesting.setParent(this);
    resizer.setParent(this);
    bounds.setParent(this);
  }
};

}  // namespace opengl
}  // namespace graph3d

#endif