/**
 * @file s21_transformation_strategy.cpp
 * @brief Model interaction implementation.
 */

#include "s21_transformation_strategy.h"

namespace s21 {

Context::~Context() {
  if (transformStrategy) delete transformStrategy;
}

void Context::SetStrategy(Strategy *strategy) {
  delete transformStrategy;
  transformStrategy = strategy;
}

glm::mat4 Context::TransformModel(const InputData &params, Model *viewerModel) {
  return transformStrategy->Transform(params,
                                      viewerModel->GetTransformMatrices());
}

glm::mat4 RotateStrategy::Transform(const InputData &params,
                                    TransformationMatrices &transform) {
  float x = glm::radians(params.xRotationAngle);
  float y = glm::radians(params.yRotationAngle);
  float z = glm::radians(params.zRotationAngle);

  glm::mat4 mat(
      static_cast<GLfloat>(cos(z) * cos(y)),
      static_cast<GLfloat>(-sin(z) * cos(x) + cos(z) * sin(y) * sin(x)),
      static_cast<GLfloat>(sin(z) * sin(x) + cos(z) * sin(y) * cos(x)), 0.0f,
      static_cast<GLfloat>(sin(z) * cos(y)),
      static_cast<GLfloat>(cos(z) * cos(x) + sin(z) * sin(y) * sin(x)),
      static_cast<GLfloat>(-cos(z) * sin(x) + sin(z) * sin(y) * cos(x)), 0.0f,
      static_cast<GLfloat>(-sin(y)), static_cast<GLfloat>(cos(y) * sin(x)),
      static_cast<GLfloat>(cos(y) * cos(x)), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);

  transform.rotateMatrix = mat;
  glm::mat4 modelMatrix = glm::mat4(1.0f) * transform.translateMatrix *
                          (transform.rotateMatrix * transform.scaleMatrix);
  return modelMatrix;
}

glm::mat4 ScaleStrategy::Transform(const InputData &params,
                                   TransformationMatrices &transform) {
  glm::mat4 mat(params.scale, 0, 0, 0, 0, params.scale, 0, 0, 0, 0,
                params.scale, 0, 0, 0, 0, 1);

  transform.scaleMatrix = mat * transform.scaleMatrix;
  glm::mat4 modelMatrix = glm::mat4(1.0f) * transform.translateMatrix *
                          (transform.rotateMatrix * transform.scaleMatrix);

  return modelMatrix;
}

glm::mat4 MoveStrategy::Transform(const InputData &params,
                                  TransformationMatrices &transform) {
  float x = params.xMoveOffset;
  float y = params.yMoveOffset;
  float z = params.zMoveOffset;

  transform.translateMatrix[3][0] = x;
  transform.translateMatrix[3][1] = y;
  transform.translateMatrix[3][2] = z;
  glm::mat4 modelMatrix = glm::mat4(1.0f) * transform.translateMatrix *
                          (transform.scaleMatrix * transform.rotateMatrix);

  return modelMatrix;
}

}  // namespace s21
