#ifndef GRAPH3D_CORE_GRAPH3D_H_
#define GRAPH3D_CORE_GRAPH3D_H_

#ifdef _WIN32
#include <windows.h>
#endif

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <ctime>
#include <initializer_list>
#include <string>

#include <core/context.h>
#include <core/context_type.h>
#include <entity/camera.h>
#include <entity/light.h>
#include <entity/lights/area.h>
#include <entity/lights/directional.h>
#include <entity/lights/point.h>
#include <entity/lights/spotlight.h>
#include <entity/object.h>
#include <opengl/opengl.h>
#include <opengl/shader.h>
#include <opengl/window.h>
#include <util/logger.h>
#include <util/pipe.h>
#include <util/resources.h>
#include <util/sleep.h>

class Graph3D;

namespace graph3d {
class Graph3D : public opengl::OpenGL {
  // Esta clase es implementación interna,
  // por lo que ningún usuario debería usarla directamente.
  // En cambio, se debe extender de la clase homónima en el namespace global
  friend class ::Graph3D;

 private:
  /// Variables
  Context context;

  double deltaTime, targetTime;
  int g_fps;

  std::map<std::string, entity::Object*> scene;
  std::vector<entity::Camera*> cameras;
  std::vector<entity::Light*> lights;

 public:
  Graph3D() : targetTime(1. / g_fps), opengl::OpenGL() {
    onContextChange(&Graph3D::contextChangeListener, this);
    bindPipes();
  }

  ~Graph3D() {
    for (auto& entry : scene) delete entry.second;
    scene.clear();

    for (auto light : lights) delete light;
    lights.clear();

    for (auto camera : cameras) delete camera;
    cameras.clear();
  }

 protected:
  void createContext(const int width, const int height, const char* title, bool fullscreen) {
    util::log("> Crear Contexto", 1);

    util::log("> Asociar Ventana al contexto actual", 3);
    context.setWindow(createWindow(width, height, title, fullscreen));
    util::log("> Asociar Monitor al contexto actual", 3);
    context.setMonitor(((opengl::Window*)context.window)->monitor);
    util::log("> Asociar Viewport al contexto actual", 3);
    context.setViewport(((opengl::Window*)context.window)->mainViewport);
    util::log("> Asociar Delta Time al contexto actual", 3);
    context.setDeltaTime(&deltaTime);

    glfwMakeContextCurrent(context.window.operator->().ref);

    util::log("  < Crear Contexto", 1);
  }

  void preConfiguration() { initGLAD(); }

  void postConfiguration() { validateWindows(); }

 protected:
  Context& getContext() { return context; }

 public:
  entity::Object* createObject(const std::string& alias) { return (scene[alias] = new entity::Object()); }

  entity::Camera* createCamera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) {
    entity::Camera* camera = new entity::Camera(position, up, yaw, pitch);
    cameras.push_back(camera);
    return camera;
  }

  void drawObject(std::string object) { draw(scene[object]); }

  inline void drawObject(entity::Object* object) { draw(object); }

 private:
  int getFps() { return g_fps; }

  const int& setFps(const int& fps) {
    util::log("> Modificar FPS", 4);
    g_fps = fps;
    targetTime = 1. / fps;
    util::log("  < Modificar FPS", 4);
    return fps;
  }

 public:
  util::copy_pipe<int, Graph3D, &getFps, &setFps> fps;

 private:
  /// Implementación
  void contextChangeListener(ContextType type, void* data) {
    switch (type) {
      case G3D_CONTEXT_MONITOR:
        context.setMonitor(static_cast<opengl::Monitor*>(data));
        break;
      case G3D_CONTEXT_WINDOW:
        context.setWindow(static_cast<opengl::Window*>(data));
        break;
      case G3D_CONTEXT_VIEWPORT:
        context.setViewport(static_cast<opengl::Viewport*>(data));
        break;
      case G3D_CONTEXT_SHADER:
        context.setShader(static_cast<opengl::Shader*>(data));
        break;
    }
  }

  void mainLoop() {
    util::log("> Main Loop", 1);
    time_t startTime, lastTime = time(0);
    double elapsedTime, timePool = 0;

    while (shouldRun()) {
      util::log("> Ejecutar Main Loop", 10);
      startTime = time(0);
      deltaTime = difftime(startTime, lastTime);
      timePool += targetTime;

      OpenGL::draw(context);
      glfwPollEvents();

      elapsedTime = difftime(time(0), startTime);
      timePool -= elapsedTime;
      lastTime = startTime;

      if (timePool > 0) {
        util::log("> Dormir", 20);
        util::sleep(static_cast<int32_t>(timePool * 1000));
      }
    }
    util::log("  < Main Loop", 1);
  }

 private:
  void bindPipes() { fps.setParent(this); }
};

}  // namespace graph3d

#endif