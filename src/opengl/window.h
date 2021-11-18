#ifndef GRAPH3D_OPENGL_WINDOW_H_
#define GRAPH3D_OPENGL_WINDOW_H_

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <algorithm>
#include <functional>
#include <utility>

#include <exceptions/exception.h>
#include <exceptions/messages.h>
#include <opengl/monitor.h>
#include <opengl/viewport.h>
#include <util/dimension.h>
#include <util/logger.h>
#include <util/pipe.h>

namespace graph3d {
struct Context;

namespace opengl {
class OpenGL;

enum WindowEvent {
  EV_WINDOW_CLOSE,
  EV_WINDOW_RESIZE,
  EV_WINDOW_MOVE,
  EV_WINDOW_MINIMIZE,
  EV_WINDOW_MAXIMIZE,
  EV_WINDOW_FOCUS,
  EV_WINDOW_DAMAGE
};

class Window {
  friend class graph3d::opengl::OpenGL;

 private:
  template <WindowEvent>
  struct type {};

  typedef std::pair<Viewport*, int32_t> viewport_entry;

  // Custom events
  typedef std::function<void(const Context&, const Window&, Viewport&)> viewport_draw_func_t;

  // OpenGL event types
  template <typename K>
  using func_type_1 = void (K::*)(const Window&);

  template <typename K>
  using func_type_2 = void (K::*)(const Window&, int, int);

  template <typename K>
  using func_type_3 = void (K::*)(const Window&, bool);

  // OpenGL event listeners
  template <typename K>
  using close_func_t = void (K::*)(const Window&);

  template <typename K>
  using resize_func_t = void (K::*)(const Window&, int, int);

  template <typename K>
  using move_func_t = void (K::*)(const Window&, int, int);

  template <typename K>
  using minimize_func_t = void (K::*)(const Window&, bool);

  template <typename K>
  using maximize_func_t = void (K::*)(const Window&, bool);

  template <typename K>
  using focus_func_t = void (K::*)(const Window&, bool);

  template <typename K>
  using damage_func_t = void (K::*)(const Window&);

 private:
  static bool viewport_comp(const viewport_entry& a, const viewport_entry& b) { return a.second < b.second; }

 private:
  GLFWwindow* g_ref;
  Monitor* g_monitor = nullptr;

  util::dimension g_size;

  std::vector<viewport_entry> viewports;
  Viewport* g_mainViewport;

  std::vector<viewport_draw_func_t> viewportDrawSubscribers;

 public:
  Window& operator=(const Window&) = delete;
  Window(const Window&) = delete;

  Window(const int width, const int height, const char* title, bool fullscreen) : g_size(width, height) {
    util::log("> Crear Ventana", 3);
    g_ref = glfwCreateWindow(width, height, title, NULL, NULL);
    if (g_ref == nullptr) throw exceptions::exception("ERR005", exceptions::ERR005);
    g_mainViewport = viewports.emplace_back(new Viewport(&g_size), 0).first;
    g_mainViewport->onClose(&Window::viewportClose, this);

    setFullscreen(fullscreen);

    bindPipes();
    util::log("  < Crear Ventana", 3);
  }

  ~Window() {
    util::log("> Eliminar Ventana", 3);
    for (const viewport_entry& entry : viewports) delete entry.first;

    viewports.clear();

    glfwDestroyWindow(ref);
    if (g_monitor) delete g_monitor;
    util::log("  < Eliminar Ventana", 3);
  }

 private:
  util::dimension getSize() {
    int width, height;
    glfwGetWindowSize(g_ref, &width, &height);
    return util::dimension(width, height);
  }

  glm::vec2 getPosition() {
    int x, y;
    glfwGetWindowPos(g_ref, &x, &y);
    return glm::vec2(x, y);
  }

  GLFWwindow* getRef() { return g_ref; }

  Monitor* getMonitor() { return g_monitor; }

  Viewport* getMainViewport() { return g_mainViewport; }

 private:
  bool isDecorated() { return glfwGetWindowAttrib(g_ref, GLFW_DECORATED); }

  bool isFullscreen() { return g_monitor; }

 private:
  const util::dimension& setSize(const util::dimension& size) {
    glfwSetWindowSize(g_ref, size.width, size.height);
    return size;
  }

  const glm::vec2& setPosition(const glm::vec2& pos) {
    glfwSetWindowSize(g_ref, (int)pos.x, (int)pos.y);
    return pos;
  }

  const bool& setDecorated(const bool& decorated) {
    glfwSetWindowAttrib(g_ref, GLFW_DECORATED, decorated ? GLFW_TRUE : GLFW_FALSE);
    return decorated;
  }

  const bool& setFullscreen(const bool& fullscreen) {
    util::log("> Chequear pantalla completa", 5);
    if (fullscreen && !g_monitor) {
      util::log("> Cambiar a Pantalla completa", 6);
      g_monitor = new Monitor(*this);
      util::dimension monSize = g_monitor->size;
      int refreshRate = g_monitor->refreshRate;
      glfwSetWindowMonitor(g_ref, g_monitor->getRef(), 0, 0, monSize.width, monSize.height, refreshRate);
      util::log("  < Cambiar a Pantalla completa", 6);
    } else if (!fullscreen && g_monitor) {
      util::log("> Pasar a modo Ventana", 6);
      glm::vec2 monPos = g_monitor->position, monSize = (util::dimension)g_monitor->size;
      delete g_monitor;
      g_monitor = nullptr;
      util::dimension size = this->size, pos;
      pos.width = (int)(monPos.x + (monSize.x - size.width) / 2);
      pos.height = (int)(monPos.y + (monSize.y - size.width) / 2);
      glfwSetWindowMonitor(g_ref, NULL, pos.width, pos.height, (int)monSize.x, (int)monSize.y, GLFW_DONT_CARE);
      util::log("> Pasar a modo ventana", 6);
    }

    util::log("  < Chequear pantalla completa", 5);
    return fullscreen;
  }

 public:
  util::copy_pipe<util::dimension, Window, &getSize, &setSize> size;
  util::copy_pipe<glm::vec2, Window, &getPosition, &setPosition> position;

  util::copy_pipe<bool, Window, &isDecorated, &setDecorated> decorated;
  util::copy_pipe<bool, Window, &isFullscreen, &setFullscreen> fullscreen;

  util::copy_pipe_get<GLFWwindow*, Window, &getRef> ref;
  util::copy_pipe_get<Monitor*, Window, &getMonitor> monitor;
  util::copy_pipe_get<Viewport*, Window, &getMainViewport> mainViewport;

 public:
  void close() const { glfwSetWindowShouldClose(g_ref, GLFW_TRUE); }
  bool shouldClose() const { return glfwWindowShouldClose(g_ref); }
  void minimize() const { glfwIconifyWindow(g_ref); }
  void restore() const { glfwRestoreWindow(g_ref); }
  void maximize() const { glfwMaximizeWindow(g_ref); }
  void focus() const { glfwFocusWindow(g_ref); }
  void hide() const { glfwHideWindow(g_ref); }
  void show() const { glfwShowWindow(g_ref); }

 public:
  Viewport* createViewport(int32_t priority = 0) {
    std::pair<Viewport*, int32_t> item = std::make_pair(new Viewport(&g_size), priority);
    viewports.insert(std::upper_bound(viewports.begin(), viewports.end(), item, viewport_comp), item);
    item.first->onClose(&Window::viewportClose, this);
    return item.first;
  }

 public:
  bool operator==(const Window& other) const { return g_ref == other.ref; }

 private:
  void viewportClose(const Viewport& viewport) {
    auto it = viewports.begin(), end = viewports.end();

    while (it != end) {
      if (*(it->first) == viewport) {
        delete it->first;
        it = viewports.erase(it);
        end = viewports.end();
      } else
        ++it;
    }
  }

  void draw(const Context& context) const {
    glfwMakeContextCurrent(g_ref);

    for (const viewport_entry& entry : viewports) viewportDraw(entry.first, context);

    glfwSwapBuffers(g_ref);
  }

  void viewportDraw(Viewport* viewport, const Context& context) const {
    for (viewport_draw_func_t func : viewportDrawSubscribers) func(context, *this, *viewport);
    viewport->draw(context);
  }

  void bindPipes() {
    size.setParent(this);
    position.setParent(this);

    decorated.setParent(this);
    fullscreen.setParent(this);

    ref.setParent(this);
    monitor.setParent(this);
    mainViewport.setParent(this);
  }

 public:
  template <typename Parent>
  void onDrawViewport(void (Parent::*func)(const Context&, const Window&, Viewport&), Parent* parent) {
    viewportDrawSubscribers.push_back([func, parent](const Context& context, const Window& window, Viewport& viewport) {
      (parent->*func)(context, window, viewport);
    });
  }

 public:
  template <WindowEvent Event, typename Parent>
  void registerEvent(func_type_1<Parent> listener, const Parent* parent) {
    registerEvent_impl(type<Event>, listener, parent);
  }

  template <WindowEvent Event, typename Parent>
  void registerEvent(func_type_2<Parent> listener, const Parent* parent) {
    registerEvent_impl(type<Event>, listener, parent);
  }

  template <WindowEvent Event, typename Parent>
  void registerEvent(func_type_3<Parent> listener, const Parent* parent) {
    registerEvent_impl(type<Event>, listener, parent);
  }

 private:
  template <typename K>
  void registerEvent_impl(type<EV_WINDOW_CLOSE> ev, close_func_t<K> func, const K* parent) {
    glfwSetWindowCloseCallback(ref, [func, parent](const Window& window) { (parent->*func)(window); });
  }

  template <typename K>
  void registerEvent_impl(type<EV_WINDOW_RESIZE> ev, resize_func_t<K> func, const K* parent) {
    glfwSetFramebufferSizeCallback(
        ref, [func, parent](const Window& window, int& width, int& height) { (parent->*func)(window, width, height); });
  }

  template <typename K>
  void registerEvent_impl(type<EV_WINDOW_MOVE> ev, move_func_t<K> func, const K* parent) {
    glfwSetWindowPosCallback(ref,
                             [func, parent](const Window& window, int& x, int& y) { (parent->*func)(window, x, y); });
  }

  template <typename K>
  void registerEvent_impl(type<EV_WINDOW_MINIMIZE> ev, minimize_func_t<K> func, const K* parent) {
    glfwSetWindowIconifyCallback(
        ref, [func, parent](const Window& window, bool& iconified) { (parent->*func)(window, iconified); });
  }

  template <typename K>
  void registerEvent_impl(type<EV_WINDOW_MAXIMIZE> ev, maximize_func_t<K> func, const K* parent) {
    glfwSetWindowMaximizeCallback(
        ref, [func, parent](const Window& window, bool& maximized) { (parent->*func)(window, maximized); });
  }

  template <typename K>
  void registerEvent_impl(type<EV_WINDOW_FOCUS> ev, focus_func_t<K> func, const K* parent) {
    glfwSetWindowFocusCallback(
        ref, [func, parent](const Window& window, bool& focused) { (parent->*func)(window, focused); });
  }

  template <typename K>
  void registerEvent_impl(type<EV_WINDOW_DAMAGE> ev, damage_func_t<K> func, const K* parent) {
    glfwSetWindowRefreshCallback(ref, [func, parent](const Window& window) { (parent->*func)(window); });
  }
};
}  // namespace opengl
}  // namespace graph3d

#endif