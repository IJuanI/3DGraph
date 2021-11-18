#ifndef GRAPH3D_CORE_CONTEXT_H_
#define GRAPH3D_CORE_CONTEXT_H_

#include <opengl/monitor.h>
#include <opengl/viewport.h>
#include <opengl/window.h>
#include <opengl/shader.h>
#include <util/pipe.h>

namespace graph3d {
class Graph3D;

struct Context {
  friend class graph3d::Graph3D;

 private:
  opengl::Window* g_window;
  opengl::Viewport* g_viewport;
  opengl::Monitor* g_monitor;
  opengl::Shader* g_shader;
  double* g_deltaTime;

 private:
  Context() { bindPipes(); }

 private:
  opengl::Monitor* getMonitor() { return g_monitor; }
  opengl::Window* getWindow() { return g_window; }
  opengl::Viewport* getViewport() { return g_viewport; }
  opengl::Shader* getShader() { return g_shader; }
  double& getDeltaTime() { return *g_deltaTime; }

 public:
  util::copy_pipe_get<opengl::Monitor*, Context, &getMonitor> monitor;
  util::copy_pipe_get<opengl::Window*, Context, &getWindow> window;
  util::copy_pipe_get<opengl::Viewport*, Context, &getViewport> viewport;
  util::copy_pipe_get<opengl::Shader*, Context, &getShader> shader;
  util::fwd_pipe_get<double, Context, &getDeltaTime> deltaTime;

 private:
  void setMonitor(opengl::Monitor* monitor) { g_monitor = monitor; }
  void setWindow(opengl::Window* window) { g_window = window; }
  void setViewport(opengl::Viewport* viewport) { g_viewport = viewport; }
  void setShader(opengl::Shader* shader) { g_shader = shader; }
  void setDeltaTime(double* deltaTime) { g_deltaTime = deltaTime; }

 private:
  void bindPipes() {
    monitor.setParent(this);
    window.setParent(this);
    viewport.setParent(this);
    shader.setParent(this);
    deltaTime.setParent(this);
  }
};
}  // namespace graph3d

#endif