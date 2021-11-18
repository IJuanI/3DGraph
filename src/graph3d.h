#ifndef GRAPH3D_GRAPH3D_H_
#define GRAPH3D_GRAPH3D_H_

#include <exception>
#include <initializer_list>
#include <iostream>
#include <string>

#include <core/graph3d.h>
#include <util/logger.h>

class Graph3D : public graph3d::Graph3D {
  int width;
  int height;
  const char *title;
  bool fullscreen;

 protected:
  /// Constructor
  Graph3D(const int width, const int height, const char *title, bool fullscreen = false)
      : width(width), height(height), title(title), fullscreen(fullscreen), graph3d::Graph3D() {}

 public:
  void start() {
    try {
      runLifecycle();
    } catch (graph3d::exceptions::exception ex) {
      ex.print();
    } catch (std::exception ex) {
      std::cerr << "/* ** Uncaught Exception Ocurred ** */\n"
                << "Error: " << ex.what() << '\n'
                << " -- --------------------------------------- -- \n";
    }
  }

 private:
  void runLifecycle() {
    graph3d::util::log("> Hola", 1);

    graph3d::util::log("> Setup", 2);
    setup();
    graph3d::util::log("  < Setup", 2);

    createContext(width, height, title, fullscreen);

    graph3d::util::log("> Configuracion", 2);
    preConfiguration();
    configure();
    postConfiguration();
    graph3d::util::log("  < Configuracion", 2);

    graph3d::util::log("> Pre Inicializacion", 2);
    preinitialize();
    graph3d::util::log("> Inicializacion", 2);
    initialize();
    graph3d::util::log("  < Inicializacion", 2);

    mainLoop();

    graph3d::util::log("  < Adios", 1);
  }

 protected:
  /// Ciclos de vida
  virtual void setup() {}

  virtual void configure() {}

  virtual void preinitialize() {}

  virtual void initialize() = 0;

 private:
  void mainLoop() { graph3d::Graph3D::mainLoop(); }

 public:
  /// Interfaz
  void addShaderFolders(const std::initializer_list<std::string> &&folders) {
    for (const std::string &folder : folders) addShaderFolder(folder);
  }
  void addShaderFolder(const std::string &&folder) { addShaderFolder(folder); }
  void addShaderFolder(const std::string &folder) {
    graph3d::Graph3D::addShaderFolder(std::forward<const std::string &>(folder));
  }

  void addModelFolders(const std::initializer_list<std::string> &&folders) {
    for (const std::string &folder : folders) addModelFolder(folder);
  }
  void addModelFolder(const std::string &&folder) { addModelFolder(folder); }
  void addModelFolder(const std::string &folder) {
    graph3d::Graph3D::addModelFolder(std::forward<const std::string &>(folder));
  }

  void addTextureFolders(const std::initializer_list<std::string> &&folders) {
    for (const std::string &folder : folders) addTextureFolder(folder);
  }
  void addTextureFolder(const std::string &&folder) { addTextureFolder(folder); }
  void addTextureFolder(const std::string &folder) {
    graph3d::Graph3D::addTextureFolder(std::forward<const std::string &>(folder));
  }

  void loadShaders(const std::initializer_list<std::string> &&shaders) {
    for (const std::string &shader : shaders) loadShader(shader);
  }
  void loadShader(const std::string &&shader) { loadShader(shader); }
  void loadShader(const std::string &shader) {
    graph3d::Graph3D::loadShader(std::forward<const std::string &>(shader));
  }

  void loadModels(const std::initializer_list<std::string> &&models) {
    for (const std::string &model : models) loadModel(model);
  }
  void loadModel(const std::string &&model) { loadModel(model); }
  void loadModel(const std::string &model) { graph3d::Graph3D::loadModel(std::forward<const std::string &>(model)); }
};

#endif