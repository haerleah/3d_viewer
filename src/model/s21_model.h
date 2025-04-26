/**
 * @file s21_model.h
 * @brief Model header file.
 */

#ifndef S21_MODEL_H
#define S21_MODEL_H

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

#include "s21_obj_loader.h"

namespace s21 {

/**
 * @brief Structure for storing view and projection matrices.
 */
struct VPmatrices {
  const glm::mat4& viewMatrix;        ///< Reference to the view matrix.
  const glm::mat4& projectionMatrix;  ///< Reference to the projection matrix.
};

/**
 * @brief Structure for storing transformation matrices.
 */
struct TransformationMatrices {
  glm::mat4 rotateMatrix;     ///< Rotation matrix.
  glm::mat4 scaleMatrix;      ///< Scaling matrix.
  glm::mat4 translateMatrix;  ///< Translation matrix.
};

/**
 * @brief Enumeration for defining projection types.
 */
enum ProjectionType {
  Orthogonal,  ///< Orthogonal projection.
  Frustum      ///< Frustum projection.
};

/**
 * @brief Class representing a 3D model.
 */
class Model {
 public:
  Model();             ///< Constructor of the Model class.
  ~Model() = default;  ///< Default destructor of the Model class.

  /**
   * @brief Gets the vertices of the model.
   * @return Constant reference to the vector of vertices.
   */
  const std::vector<GLfloat>& GetVertices() const;

  /**
   * @brief Gets the indices of the model.
   * @return Constant reference to the vector of indices.
   */
  const std::vector<GLuint>& GetIndices() const;

  /**
   * @brief Gets the transformation matrices.
   * @return Reference to the transformation matrices.
   */
  TransformationMatrices& GetTransformMatrices();

  /**
   * @brief Gets the view and projection matrices.
   * @return VPmatrices structure containing the view and projection matrices.
   */
  VPmatrices GetVP() const;

  /**
   * @brief Sets the projection matrix based on the specified type and
   * dimensions.
   * @param type Projection type (Orthogonal or Frustum).
   * @param width Width of the viewing area.
   * @param height Height of the viewing area.
   */
  void SetProjection(ProjectionType type, int width, int height);

  /**
   * @brief Creates buffers for the model.
   */
  void CreateBuffers();

  /**
   * @brief Resets the model transformations to default values.
   */
  void ResetToDefault();

 private:
  std::vector<GLfloat> vertices;  ///< Vector for storing the model's vertices.
  std::vector<GLuint> indices;    ///< Vector for storing the model's indices.
  glm::mat4 viewMatrix;           ///< View transformation matrix.
  glm::mat4 projectionMatrix;     ///< Projection transformation matrix.
  TransformationMatrices transform;  ///< Transformation matrices for the model.

  /**
   * @brief Normalizes the given vertices.
   * @param vertices Vertices to normalize.
   * @return Vector of normalized vertices.
   */
  std::vector<GLfloat> NormalizeVertices(const std::vector<GLfloat>& vertices);
};

}  // namespace s21

#endif  // S21_MODEL_H
