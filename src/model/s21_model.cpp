/**
 * @file s21_model.cpp
 * @brief Model header implementation.
 */

#include "s21_model.h"

namespace s21 {

Model::Model() {
  ResetToDefault();

  glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
  glm::vec3 targetPos = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);

  viewMatrix = glm::lookAt(cameraPos, targetPos, upVector);

  projectionMatrix = glm::mat4(1.0f);
}

void Model::CreateBuffers() {
  ObjLoader &objLoaderInstance = ObjLoader::Instance();

  vertices = NormalizeVertices(objLoaderInstance.GetVertices());
  indices = objLoaderInstance.GetFaces();
}

void Model::ResetToDefault() {
  transform = {glm::mat4(1.0f), glm::mat4(1.0f), glm::mat4(1.0f)};
}

std::vector<GLfloat> Model::NormalizeVertices(
    const std::vector<GLfloat> &vertices) {
  std::vector<GLfloat> result;
  ObjLoader &objLoaderInstance = ObjLoader::Instance();

  for (size_t i = 0; i < vertices.size(); i += 3) {
    result.push_back((vertices[i] - objLoaderInstance.GetCenters().X) *
                     objLoaderInstance.GetScaleFactor());
    result.push_back((vertices[(i + 1)] - objLoaderInstance.GetCenters().Y) *
                     objLoaderInstance.GetScaleFactor());
    result.push_back((vertices[(i + 2)] - objLoaderInstance.GetCenters().Z) *
                     objLoaderInstance.GetScaleFactor());
  }
  return result;
}

const std::vector<GLfloat> &Model::GetVertices() const { return vertices; }

const std::vector<GLuint> &Model::GetIndices() const { return indices; }

TransformationMatrices &Model::GetTransformMatrices() { return transform; }

VPmatrices Model::GetVP() const { return {viewMatrix, projectionMatrix}; }

void Model::SetProjection(ProjectionType type, int width, int height) {
  float aspect = static_cast<float>(width) / static_cast<float>(height);
  if (type == Orthogonal) {
    if (width >= height) {
      projectionMatrix =
          glm::ortho(-0.5f * aspect, 0.5f * aspect, -0.5f, 0.5f, 0.1f, 100.0f);
    } else {
      projectionMatrix =
          glm::ortho(-0.5f, 0.5f, -0.5f / aspect, 0.5f / aspect, 0.1f, 100.0f);
    }
  } else if (type == Frustum) {
    projectionMatrix =
        glm::perspective(glm::radians(45.0f), aspect, 0.75f, 100.0f);
  }
}

}  // namespace s21
