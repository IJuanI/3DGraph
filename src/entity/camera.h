#ifndef GRAPH3D_ENTITY_CAMERA_H_
#define GRAPH3D_ENTITY_CAMERA_H_

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>

#include <vector>

#include <entity/entity.h>
#include <util/bounds.h>

namespace graph3d {
namespace entity {

enum class Space { World, Camera };

// Si bien es técnicamente una entidad,
// era mas sencillo reescribir toda las funcionalidades para adaptarlas a la cámara
class Camera : public Entity {
 private:
  template <Space>
  struct type {};

 private:
  // Camera Attributes
  glm::vec3 g_pos, g_worldUp;

  // World Space -> Camera Space
  glm::mat4 transformation{1};

 public:
  // Euler Angles. Gimbal lock on world up
  float g_yaw, g_pitch;

 public:
  // Camera options
  float g_speed, g_sensitivity, g_zoom;

 public:
  // Constructor with vectors
  Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) : g_speed(2.5f), g_sensitivity(.1f), g_zoom(45.f) {
    g_pos = position;
    g_worldUp = up;
    g_yaw = yaw;
    g_pitch = pitch;
    updateRotation();
    bindPipes();
  }

 public:
  inline void move(const glm::vec3& translation) override { move<Space::Camera>(translation); }

  template <Space Target>
  void move(const glm::vec3& translation) {
    move_impl(type<Target>(), translation);
  }

  // @TODO ESTO NO VA A FUNCIONAR
  void rotate(const glm::quat& rotation) override { transformation *= glm::toMat4(rotation); }
  void rotate(const glm::vec3& axis, const GLfloat& radians) override {
    float cos = glm::cos(radians * .5f), sin = glm::sin(radians * .5f);
    transformation *= glm::toMat4(glm::quat(cos, axis.x * sin, axis.y * sin, axis.z * sin));
  }
  void rotateAround(const glm::quat& rotation, const glm::vec3& position) override {
    const glm::mat4& rotMat = glm::toMat4(rotation);
    glm::vec4 displacement{position, 0};
    glm::vec3 targetPos{displacement + rotMat * (transformation[3] - displacement)};
    glm::mat4 noTranslate = transformation;
    noTranslate[3] = {0, 0, 0, 1};
    transformation = rotMat * noTranslate;
    transformation[3] = glm::vec4(targetPos, transformation[3].w);
  }

 public:
  glm::mat4 createProjectionMatrix(util::bounds bounds) {
    return glm::perspective(glm::radians(g_zoom), (float)bounds.width / (float)bounds.height, 0.1f, 100.0f);
  }

 private:
  const glm::vec3& setPosition(const glm::vec3& position) override { return g_pos = position; }
  glm::vec3 getPosition() override { return g_pos; }

 private:
  void move_impl(type<Space::Camera>, const glm::vec3& translation) {
    g_pos += glm::vec3(transformation * glm::vec4(translation, 0));
  }

  void move_impl(type<Space::World>, const glm::vec3& translation) { g_pos += translation; }

 private:
  glm::mat4 getViewMatrix() {
    return glm::lookAt(g_pos, g_pos + glm::vec3(transformation[2]), glm::vec3(transformation[1]));
  }

  glm::vec3 getRight() { return glm::vec3(transformation[0]); }
  glm::vec3 getUp() { return glm::vec3(transformation[1]); }
  glm::vec3 getFront() { return glm::vec3(transformation[2]); }

 public:
  // Returns the view matrix calculated using Euler Angles and the LookAt Matrix
  util::copy_pipe_get<glm::mat4, Camera, &getViewMatrix> view;

  util::copy_pipe_get<glm::vec3, Camera, &getRight> right;
  util::copy_pipe_get<glm::vec3, Camera, &getUp> up;
  util::copy_pipe_get<glm::vec3, Camera, &getFront> front;

 public:
  // Calculates the front vector from the Camera's (updated) Euler Angles
  void updateRotation() {
    // Calculate the new Front vector
    glm::vec3 front, right;
    front.x = cos(glm::radians(g_yaw)) * cos(glm::radians(g_pitch));
    front.y = sin(glm::radians(g_pitch));
    front.z = sin(glm::radians(g_yaw)) * cos(glm::radians(g_pitch));
    front = glm::normalize(front);

    // Los vectores se acercan a cero cuando front se acerca a worldUp, lo que reduce la velocidad del movimiento.
    // Para arreglar esto, se normaliza por todos lados

    // El Camera Up safa de normalizarse porque el producto cruz de dos vectores normalizadas está normalizado.
    transformation[0] = glm::vec4(right = glm::normalize(glm::cross(front, g_worldUp)), 0);
    transformation[1] = glm::vec4(glm::cross(right, front), 0);
    transformation[2] = glm::vec4(front, 0);
  }

 private:
  void bindPipes() {
    view.setParent(this);
    right.setParent(this);
    up.setParent(this);
    front.setParent(this);
  }
};
}  // namespace entity
}  // namespace graph3d

#endif