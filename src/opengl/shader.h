#ifndef GRAPH3D_OPENGL_SHADER_H
#define GRAPH3D_OPENGL_SHADER_H

#include <glad/glad.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat2x2.hpp>
#include <glm/mat2x3.hpp>
#include <glm/mat2x4.hpp>
#include <glm/mat3x2.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat3x4.hpp>
#include <glm/mat4x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <exceptions/exception.h>
#include <exceptions/messages.h>
#include <exceptions/warning.h>
#include <util/logger.h>
#include <util/resources.h>

namespace graph3d {
namespace opengl {
class Shader {
 private:
  /// Variables
  bool linked = false;
  unsigned int ID;
  std::vector<GLuint> shaders;
  std::string shaderNames;  // Used for debugging errors in linking phase

 public:
  /// Constructores
  Shader() { ID = glCreateProgram(); }

  Shader(const char *folder) : Shader() {
    std::filesystem::path folderPath = util::getResourceFolderPath(util::G3D_RESOURCE_SHADER, folder);
    util::log("> Cargar programa de shaders: " + folderPath.generic_string(), 3);
    for (const auto &file : std::filesystem::directory_iterator(folderPath)) {
      const auto &filepath = file.path();
      if (filepath.has_extension()) {
        const GLenum &type = getType(filepath.extension());
        if (type != 0) addShader(type, filepath.relative_path());
      }
    }
    util::log("> Linkeo de programa de shaders" + folderPath.generic_string(), 5);

    if (shaders.empty())
      exceptions::warning("WAR004", exceptions::format(exceptions::WAR004, folderPath.generic_string().c_str()));
    else
      link();

    util::log("  < Cargar programa de shaders: " + folderPath.generic_string(), 3);
  }

  Shader(const char *vertexPath, const char *fragmentPath, const char *geometryPath = nullptr) : Shader() {
    addShader(GL_VERTEX_SHADER, vertexPath);
    addShader(GL_FRAGMENT_SHADER, fragmentPath);
    if (geometryPath) addShader(GL_GEOMETRY_SHADER, geometryPath);
  }

  /// Interfaz
  void addShader(GLenum type, const std::filesystem::path &filename) {
    addShader(type, std::ifstream(filename), filename);
  }

  void addShader(GLenum type, const char *path) {
    std::filesystem::path filename = path;
    addShader(type, util::getResourceFile(util::G3D_RESOURCE_SHADER, filename), filename);
  }

  void link() {
    glLinkProgram(ID);
    checkLinkingErrors();
    for (GLuint &shader : shaders) glDeleteShader(shader);
    linked = true;
  }

  bool isLinked() { return linked; }

  void use() { glUseProgram(ID); }

 private:
  /// Implementación
  void addShader(GLenum type, std::ifstream &&file, const std::filesystem::path &filename) {
    addShader(type, file, std::forward<const std::filesystem::path &>(filename));
  }

  void addShader(GLenum type, std::ifstream &file, const std::filesystem::path &filename) {
    std::string src_str;
    const GLchar *src;

    try {
      std::stringstream stream;
      file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

      stream << file.rdbuf();
      file.close();
      src_str = stream.str();
    } catch (std::ifstream::failure e) {
      throw exceptions::exception("ERR001", exceptions::format(exceptions::ERR001, filename.generic_string().c_str()),
                                  e.what());
    }

    src = src_str.c_str();

    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);
    checkCompileErrors(shader, filename);
    glAttachShader(ID, shader);
    shaders.push_back(shader);

    if (!shaderNames.empty()) shaderNames.append(", ");

    shaderNames.append(filename.u8string());
  }

 public:
  /// Muchos, muchos setters
  void set(const char *name, const GLfloat &&f1) const { glUniform1f(glGetUniformLocation(ID, name), f1); }
  void set(const char *name, const GLfloat &f1) const { glUniform1f(glGetUniformLocation(ID, name), f1); }

  void set(const char *name, const GLboolean &&v1) const { glUniform1i(glGetUniformLocation(ID, name), (int)v1); }
  void set(const char *name, const GLboolean &v1) const { glUniform1i(glGetUniformLocation(ID, name), (int)v1); }

  void set(const char *name, const GLint &&v1) const { glUniform1i(glGetUniformLocation(ID, name), v1); }
  void set(const char *name, const GLint &v1) const { glUniform1i(glGetUniformLocation(ID, name), v1); }

  void set(const char *name, const GLuint &&v1) const { glUniform1ui(glGetUniformLocation(ID, name), v1); }
  void set(const char *name, const GLuint &v1) const { glUniform1ui(glGetUniformLocation(ID, name), v1); }

  void set(const char *name, const GLfloat &&f1, const GLfloat &&f2) const {
    glUniform2f(glGetUniformLocation(ID, name), f1, f2);
  }
  void set(const char *name, const GLfloat &f1, const GLfloat &f2) const {
    glUniform2f(glGetUniformLocation(ID, name), f1, f2);
  }

  void set(const char *name, const GLboolean &&v1, const GLboolean &&v2) const {
    glUniform2i(glGetUniformLocation(ID, name), (int)v1, (int)v2);
  }
  void set(const char *name, const GLboolean &v1, const GLboolean &v2) const {
    glUniform2i(glGetUniformLocation(ID, name), (int)v1, (int)v2);
  }

  void set(const char *name, const GLint &&v1, const GLint &&v2) const {
    glUniform2i(glGetUniformLocation(ID, name), v1, v2);
  }
  void set(const char *name, const GLint &v1, const GLint &v2) const {
    glUniform2i(glGetUniformLocation(ID, name), v1, v2);
  }

  void set(const char *name, const GLuint &&v1, const GLuint &&v2) const {
    glUniform2ui(glGetUniformLocation(ID, name), v1, v2);
  }
  void set(const char *name, const GLuint &v1, const GLuint &v2) const {
    glUniform2ui(glGetUniformLocation(ID, name), v1, v2);
  }

  void set(const char *name, const GLfloat &&f1, const GLfloat &&f2, const GLfloat &&f3) const {
    glUniform3f(glGetUniformLocation(ID, name), f1, f2, f3);
  }
  void set(const char *name, const GLfloat &f1, const GLfloat &f2, const GLfloat &f3) const {
    glUniform3f(glGetUniformLocation(ID, name), f1, f2, f3);
  }

  void set(const char *name, const GLboolean &&v1, const GLboolean &&v2, const GLboolean &&v3) const {
    glUniform3i(glGetUniformLocation(ID, name), (int)v1, (int)v2, (int)v3);
  }
  void set(const char *name, const GLboolean &v1, const GLboolean &v2, const GLboolean &v3) const {
    glUniform3i(glGetUniformLocation(ID, name), (int)v1, (int)v2, (int)v3);
  }

  void set(const char *name, const GLint &&v1, const GLint &&v2, const GLint &&v3) const {
    glUniform3i(glGetUniformLocation(ID, name), v1, v2, v3);
  }
  void set(const char *name, const GLint &v1, const GLint &v2, const GLint &v3) const {
    glUniform3i(glGetUniformLocation(ID, name), v1, v2, v3);
  }

  void set(const char *name, const GLuint &&v1, const GLuint &&v2, const GLuint &&v3) const {
    glUniform3ui(glGetUniformLocation(ID, name), v1, v2, v3);
  }
  void set(const char *name, const GLuint &v1, const GLuint &v2, const GLuint &v3) const {
    glUniform3ui(glGetUniformLocation(ID, name), v1, v2, v3);
  }

  void set(const char *name, const GLfloat &&f1, const GLfloat &&f2, const GLfloat &&f3, const GLfloat &&f4) const {
    glUniform4f(glGetUniformLocation(ID, name), f1, f2, f3, f4);
  }
  void set(const char *name, const GLfloat &f1, const GLfloat &f2, const GLfloat &f3, const GLfloat &f4) const {
    glUniform4f(glGetUniformLocation(ID, name), f1, f2, f3, f4);
  }

  void set(const char *name, const GLboolean &&v1, const GLboolean &&v2, const GLboolean &&v3,
           const GLboolean &&v4) const {
    glUniform4i(glGetUniformLocation(ID, name), (int)v1, (int)v2, (int)v3, (int)v4);
  }
  void set(const char *name, const GLboolean &v1, const GLboolean &v2, const GLboolean &v3, const GLboolean &v4) const {
    glUniform4i(glGetUniformLocation(ID, name), (int)v1, (int)v2, (int)v3, (int)v4);
  }

  void set(const char *name, const GLint &&v1, const GLint &&v2, const GLint &&v3, const GLint &&v4) const {
    glUniform4i(glGetUniformLocation(ID, name), v1, v2, v3, v4);
  }
  void set(const char *name, const GLint &v1, const GLint &v2, const GLint &v3, const GLint &v4) const {
    glUniform4i(glGetUniformLocation(ID, name), v1, v2, v3, v4);
  }

  void set(const char *name, const GLuint &&v1, const GLuint &&v2, const GLuint &&v3, const GLuint &&v4) const {
    glUniform4ui(glGetUniformLocation(ID, name), v1, v2, v3, v4);
  }
  void set(const char *name, const GLuint &v1, const GLuint &v2, const GLuint &v3, const GLuint &v4) const {
    glUniform4ui(glGetUniformLocation(ID, name), v1, v2, v3, v4);
  }

  void set(const char *name, const glm::vec2 &&value) {
    glUniform2fv(glGetUniformLocation(ID, name), 1, glm::value_ptr(value));
  }
  void set(const char *name, const glm::vec2 &value) {
    glUniform2fv(glGetUniformLocation(ID, name), 1, glm::value_ptr(value));
  }

  void set(const char *name, const glm::vec3 &&value) {
    glUniform3fv(glGetUniformLocation(ID, name), 1, glm::value_ptr(value));
  }
  void set(const char *name, const glm::vec3 &value) {
    glUniform3fv(glGetUniformLocation(ID, name), 1, glm::value_ptr(value));
  }

  void set(const char *name, const glm::vec4 &&value) {
    glUniform4fv(glGetUniformLocation(ID, name), 1, glm::value_ptr(value));
  }
  void set(const char *name, const glm::vec4 &value) {
    glUniform4fv(glGetUniformLocation(ID, name), 1, glm::value_ptr(value));
  }

  void set(const char *name, const glm::mat2x2 &&value) {
    glUniformMatrix2fv(glGetUniformLocation(ID, name), 1, GL_FALSE, glm::value_ptr(value));
  }
  void set(const char *name, const glm::mat2x2 &value) {
    glUniformMatrix2fv(glGetUniformLocation(ID, name), 1, GL_FALSE, glm::value_ptr(value));
  }

  void set(const char *name, const glm::mat3x3 &&value) {
    glUniformMatrix3fv(glGetUniformLocation(ID, name), 1, GL_FALSE, glm::value_ptr(value));
  }
  void set(const char *name, const glm::mat3x3 &value) {
    glUniformMatrix3fv(glGetUniformLocation(ID, name), 1, GL_FALSE, glm::value_ptr(value));
  }

  void set(const char *name, const glm::mat4x4 &&value) {
    glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, GL_FALSE, glm::value_ptr(value));
  }
  void set(const char *name, const glm::mat4x4 &value) {
    glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, GL_FALSE, glm::value_ptr(value));
  }

 private:
  /// Funciones internas
  void checkLinkingErrors() {
    int success;
    char log[2048];
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(ID, 2048, NULL, log);
      throw exceptions::exception("ERR002", exceptions::format(exceptions::ERR002, shaderNames), log);
    }
  }

  void checkCompileErrors(const GLuint &shader, const std::filesystem::path &filename) const {
    int success;
    char log[2048];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(shader, 2048, NULL, log);
      throw exceptions::exception(
          "ERR003", exceptions::format(exceptions::ERR003, filename.relative_path().generic_string().c_str()), log);
    }
  }

  const GLenum getType(const std::filesystem::path &extension) {
    if (extension == ".vshader")
      return GL_VERTEX_SHADER;
    else if (extension == ".fshader")
      return GL_FRAGMENT_SHADER;
    else if (extension == ".gshader")
      return GL_GEOMETRY_SHADER;
    else
      return 0;
  }
};
}  // namespace opengl
}  // namespace graph3d
#endif