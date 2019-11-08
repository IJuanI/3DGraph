#include <initializer_list>
#include <string>

class Graph3D {
  /// Variables

 protected:
  /// Constructor
  Graph3D(const int width, const int height, const char *title) {}

 public:
  /// Ciclos de vida
  virtual void configure() {}

  virtual void preinitialize() {}

  virtual void initialize() {}

  virtual void run() {}

 public:
  /// Interfaz de la API
  void loadShaders(const std::initializer_list<std::string> &&shaders) {}
  void loadShader(const std::string &&shader) {}

  void loadModels(const std::initializer_list<std::string> &&models) {}
  void loadModel(const std::string &&model) {}

  void addShaderFolders(const std::initializer_list<std::string> &&folders) {}
  void addShaderFolder(const std::string &&folder) {}

  void addModelFolders(const std::initializer_list<std::string> &&folders) {}
  void addModelFolder(const std::string &&folder) {}

  void addTextureFolders(const std::initializer_list<std::string> &&folders) {}
  void addTextureFolder(const std::string &&folder) {}
};