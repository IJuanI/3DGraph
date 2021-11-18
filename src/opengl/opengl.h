#ifndef GRAPH3D_OPENGL_OPENGL_H_
#define GRAPH3D_OPENGL_OPENGL_H_

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <vector>

#include <core/context.h>
#include <core/context_type.h>
#include <entity/camera.h>
#include <entity/object.h>
#include <opengl/model.h>
#include <opengl/shader.h>
#include <opengl/window.h>
#include <util/bounds.h>
#include <util/logger.h>

namespace graph3d {
class Graph3D;

// @TODO Support input modes
// @TODO Support mouse events
// @TODO Process input
namespace opengl {
class OpenGL {
  friend class graph3d::Graph3D;

 private:
  typedef std::function<void(ContextType, void *)> context_change_func_t;

 private:
  /// Variables
  std::map<std::string, Shader *> shaderPrograms;
  std::map<std::string, Model *> models;
  std::vector<Window *> windows;

  Shader *activeShader;

  std::vector<context_change_func_t> contextChangeSubscribers;

 protected:
  /// Constructor
  OpenGL() {
    util::log("> Inicializar OpenGL", 2);
    initGLFW();
    initialize();
    util::log("  < Inicializar OpenGL", 2);
  }

  /// Destructor
  ~OpenGL() {
    util::log("> Liberar recursos de OpenGL", 2);

    for (auto &entry : models) delete entry.second;
    models.clear();

    for (auto &entry : shaderPrograms) delete entry.second;
    shaderPrograms.clear();

    for (Window *window : windows) delete window;
    windows.clear();

    glfwTerminate();
    util::log("  < Liberar recursos de OpenGL", 2);
  }

 protected:
  virtual Context &getContext() = 0;

 public:
  /// Interfaz
  void addShaderFolder(const std::string &folder) {
    util::log("> Agregar carpeta de Shaders", 3);
    graph3d::util::addResourceFolder(graph3d::util::G3D_RESOURCE_SHADER, folder);
    util::log("  < Agregar carpeta de Shaders", 3);
  }

  void addModelFolder(const std::string &folder) {
    util::log("> Agregar carpeta de Modelos", 3);
    graph3d::util::addResourceFolder(graph3d::util::G3D_RESOURCE_MODEL, folder);
    util::log("  < Agregar carpeta de Modelos", 3);
  }

  void addTextureFolder(const std::string &folder) {
    util::log("> Agregar carpeta de Texturas", 3);
    graph3d::util::addResourceFolder(graph3d::util::G3D_RESOURCE_TEXTURE, folder);
    util::log("  < Agregar carpeta de Texturas", 3);
  }

  void loadShader(const std::string &shader) { shaderPrograms[shader] = new Shader(shader.c_str()); }

  void loadModel(const std::string &model) { models[model] = new Model(model.c_str()); }

 public:
  void useShader(const std::string &shader) {
    if (shaderPrograms.find(shader) == shaderPrograms.end())
      throw exceptions::exception("ERR010", exceptions::format(exceptions::ERR010, shader.c_str()));
    activeShader = shaderPrograms[shader];
    activeShader->use();
  }

 public:
  opengl::Window *createWindow(const int width, const int height, const char *title, bool fullscreen = false) {
    opengl::Window *window = windows.emplace_back(new Window(width, height, title, fullscreen));

    window->onDrawViewport(&OpenGL::drawViewport, this);

    return window;
  }

  virtual entity::Camera *createCamera(glm::vec3 position = G3D_ZERO, glm::vec3 up = G3D_UP, float yaw = -90.0f,
                                       float pitch = .0f) = 0;

  void draw(entity::Object *object) {
    Viewport *viewport = getContext().viewport;
    entity::Camera *camera = viewport->camera;

    util::bounds bounds = viewport->bounds;

    glm::mat4 projection = camera->createProjectionMatrix(viewport->bounds);
    activeShader->set("projection", projection);
    activeShader->set("view", camera->view);
    activeShader->set("model", object->transformation);
    models[object->modelAlias]->Draw(*activeShader);
  }

 protected:
  void validateWindows() {
    for (const opengl::Window *window : windows) {
      for (auto &entry : window->viewports) {
        if (!entry.first->camera) entry.first->camera = createCamera();
      }
    }
  }

 private:
  void drawViewport(const Context &context, const Window &window, Viewport &viewport) {
    requestContextChange(G3D_CONTEXT_VIEWPORT, &viewport);
  }

 private:
  /// Implementación
  void initGLFW() {
    util::log("> Inicializar GLFW", 2);
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    util::log("  < Inicializar GLFW", 2);
  }

  void initGLAD() {
    util::log("> Inicializar GLAD", 2);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) throw exceptions::exception("ERR004", exceptions::ERR004);
    util::log("  < Inicializar GLAD", 2);
  }

  void initialize() { glfwSwapInterval(1); }

 private:
  bool shouldRun() {
    bool run = false;
    for (std::vector<Window *>::iterator it = windows.begin(), end = windows.end(); it != end;) {
      if ((*it)->shouldClose()) {
        delete *it;
        it = windows.erase(it);
        end = windows.end();
      } else {
        run = true;
        ++it;
      }
    }
    return run;
  }

  void draw(const Context &context) {
    util::log("> Dibujar Ventanas", 12);
    for (const opengl::Window *window : windows) window->draw(context);
    util::log("  < Dibujar Ventanas", 12);
  }

 public:
  template <typename Parent>
  void onContextChange(void (Parent::*func)(ContextType type, void *data), Parent *parent) {
    contextChangeSubscribers.push_back([func, parent](ContextType type, void *data) { (parent->*func)(type, data); });
  }

 private:
  void requestContextChange(ContextType type, void *data) {
    for (context_change_func_t func : contextChangeSubscribers) func(type, data);
  }
};
}  // namespace opengl
}  // namespace graph3d

#endif