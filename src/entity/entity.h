#ifndef GRAPH3D_ENTITY_ENTITY_H_
#define GRAPH3D_ENTITY_ENTITY_H_

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include <util/pipe.h>

namespace graph3d {

extern glm::vec3 G3D_ZERO;
extern glm::vec3 G3D_FORWARD;
extern glm::vec3 G3D_BACKWARD;
extern glm::vec3 G3D_UP;
extern glm::vec3 G3D_DOWN;
extern glm::vec3 G3D_LEFT;
extern glm::vec3 G3D_RIGHT;

namespace entity {
class Entity {
 protected:
  glm::mat4 transformation{1};

 public:
  Entity() { bindPipes(); }

 public:
  inline void scale(const glm::vec3&& factor) { scale(factor); }
  virtual void scale(const glm::vec3& factor) { transformation = glm::scale(transformation, factor); }

  inline void move(const glm::vec3&& translation) { move(translation); }
  virtual void move(const glm::vec3& translation) { transformation = glm::translate(transformation, translation); }

  inline void rotate(const glm::quat&& rotation) { rotate(rotation); }
  virtual void rotate(const glm::quat& rotation) { transformation *= glm::toMat4(rotation); }

  inline void rotate(const glm::vec3&& axis, const GLfloat&& radians) { rotate(axis, radians); }
  virtual void rotate(const glm::vec3& axis, const GLfloat& radians) {
    transformation = glm::rotate(transformation, radians, axis);
  }

  inline void rotateAround(const glm::quat&& rotation, const glm::vec3&& position) { rotateAround(rotation, position); }
  virtual void rotateAround(const glm::quat& rotation, const glm::vec3& position) {
    const glm::mat4& rotMat = glm::toMat4(rotation);
    glm::vec4 displacement{position, 0};
    glm::vec3 targetPos{displacement + rotMat * (transformation[3] - displacement)};
    glm::mat4 noTranslate = transformation;
    noTranslate[3] = {0, 0, 0, 1};
    transformation = rotMat * noTranslate;
    transformation[3] = glm::vec4(targetPos, transformation[3].w);
  }

  inline void rotateAround(const glm::vec3&& axis, const GLfloat&& radians, const glm::vec3&& position) {
    rotateAround(axis, radians, position);
  }
  void rotateAround(const glm::vec3& axis, const GLfloat& radians, const glm::vec3& position) {
    const float &cos = glm::cos(radians * .5f), &sin = glm::sin(radians * .5f);
    rotateAround(glm::quat(cos, axis.x * sin, axis.y * sin, axis.z * sin), position);
  }

 private:
  virtual glm::vec3 getPosition() { return transformation[3]; }
  virtual const glm::vec3& setPosition(const glm::vec3& position) {
    transformation[3] = glm::vec4(position, transformation[3].w);
    return position;
  }

 private:
  glm::vec3 getPosition_impl() { return getPosition(); }
  const glm::vec3& setPosition_impl(const glm::vec3& position) { return setPosition(position); }

 public:
  util::copy_pipe<glm::vec3, Entity, &getPosition_impl, &setPosition_impl> position;

 private:
  void bindPipes() { position.setParent(this); }
};
}  // namespace entity
}  // namespace graph3d

#endif